#include "KsdkImpl.h"

#include <windows.h>

#include <QStandardPaths>
#include <QUuid>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QDirIterator>
#include <QLibrary>

#include "Message.h"
#include "Json.h"
#include "IInit.h"

extern "C" {
#include "kaler/parse_top.h"
#include "kaler/file_ops.h"
}

KsdkImpl::KsdkImpl(const char* serialName):
	sendProgressRecver{std::bind(&KsdkImpl::sendProgress, this, std::placeholders::_1)}
{
	if (!serialName)
		throw KSDK_ERR_PARA_SERIAL_PORT_NAME;

	QLibrary libUdp{"udp.dll"};
	PluginCreator creatorUdp = (PluginCreator)libUdp.resolve("createKpg");
	if (!creatorUdp) {
		throw KSDK_ERR_CREATE_UDP;
	} else
		plugins.push_back(creatorUdp());

	QLibrary libSerial{"serial.dll"};
	PluginCreator creatorSerial= (PluginCreator)libSerial.resolve("createKpg");
	if (!creatorSerial) {
		throw KSDK_ERR_CREATE_SERIAL;
	} else
		plugins.push_back(creatorSerial());

	QLibrary libProtocol{"protocol.dll"};
	PluginCreator creatorProtocol = (PluginCreator)libProtocol.resolve("createKpg");
	if (!creatorProtocol) {
		throw KSDK_ERR_CREATE_PROTOCOL;
	}
	protocolPlugin = creatorProtocol();
	IProtocolFactory* protocolFactory = dynamic_cast<IProtocolFactory*>(protocolPlugin);
	IInit* init = dynamic_cast<IInit*>(protocolFactory);
	init->init(plugins);

	protocol = protocolFactory->create(nullptr);
	protocol->bind({ICarrier::SERIAL, {"", "", serialName}});
	protocol->open();

	return;
}

KsdkImpl::~KsdkImpl()
{
	rmTmpDir();
	if (protocol) {
		protocol->close();
		delete protocol;
		protocol = nullptr;
	}
	if (protocolPlugin) {
		delete protocolPlugin;
		protocolPlugin = nullptr;
	}
	for (auto plugin: plugins) {
		delete plugin;
	}
}

void KsdkImpl::rmTmpDir()
{
	QDir dir{tmpDir};
	if (dir.exists(tmpDir))
		dir.removeRecursively();
}

void KsdkImpl::send(const KsdkProgram* programArray,
					const unsigned int programCount,
					KsdkProgressCb progressCb,
					KsdkResultCb resultCb)
{
	sendProgressCb = progressCb;
	sendResultCb = resultCb;

	if (!programArray || programCount == 0)
		return sendResult(KSDK_ERR_PARA_PROGRAM);

	for (unsigned int i = 0; i < programCount; ++i) {
		if (!programArray[i].pageArray || !programArray[i].pageCount)
			return sendResult(KSDK_ERR_PARA_PAGE);
		for (unsigned int j = 0; j < programArray[i].pageCount; ++j) {
			if (!programArray[i].pageArray[j].imagePath)
				return sendResult(KSDK_ERR_PARA_PAGE_IMAGE_PATH);
		}
	}

	rmTmpDir();
	do {
		tmpDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation)
					+ "/ksdk/"
					+ QUuid::createUuid().toString();
	} while(QFileInfo(tmpDir).exists());
	QDir().mkpath(tmpDir);

	HMODULE module;
	::GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
						 (wchar_t*)this, &module);
	wchar_t dllPathChars[1024];
	GetModuleFileNameW(module, dllPathChars, 1024);
	QString dllPath = QString::fromStdWString(dllPathChars);
	QString dllDir = QFileInfo(dllPath).dir().absolutePath();
	QFile templateFile{dllDir + "/" + "template.json"};
	if (!templateFile.open(QIODevice::ReadOnly))
		return sendResult(KSDK_ERR_OPEN_TEMPLATE_FILE);
	QByteArray templateData = templateFile.readAll();
	if (templateData.isEmpty())
		return sendResult(KSDK_ERR_READ_TEMPLATE_FILE);

	std::string coreJson;

	try {
		Json jScreen = Json::parse(templateData.data());
		jScreen["topLevelConfig"]["outFilePath"] = tmpDir;

		Json jProgramTemplate = jScreen["programDescribe"][0];
		Json jPageTemplate = jProgramTemplate["areaArray"][0]["areaDescribe"]["pictureFuseArray"][0];

		for (unsigned int i = 0; i < programCount; ++i) {
			Json jProgram = jProgramTemplate;
			for (unsigned int j = 0; j < programArray[i].pageCount; ++j) {
				Json jPage = jPageTemplate;
				KsdkPage& page = programArray[i].pageArray[j];
				jPage["content"] = QString::fromStdWString(page.imagePath).toStdString();
				jPage["enterMode"] = page.style;
				jPage["speed"] = page.speed;
				jPage["delay"] = page.stay;
				jProgram["areaArray"][0]["areaDescribe"]["pictureFuseArray"][j] = jPage;
			}
			jScreen["programDescribe"][i] = jProgram;
		}
		coreJson = jScreen.dump(4);
	} catch (...) {
		return sendResult(KSDK_ERR_PARSE_TEMPLATE_FILE);
	}

	QFile coreJsonFile{tmpDir + "/ALL.json"};
	if (!coreJsonFile.open(QIODevice::WriteOnly))
		return sendResult(KSDK_ERR_OPEN_JSON_FILE);
	coreJsonFile.write(coreJson.c_str(), coreJson.size());
	coreJsonFile.flush();

	AllConfig_t AllConfig;
	Record_t Record;

	memset(&Record, 0, sizeof(Record_t));

	if (JsonParseAll(coreJson.c_str(), &AllConfig, &Record) != 0)
	{
		return sendResult(KSDK_ERR_PARSE_JSON_FILE);
	}

	CreateKalerFile(AllConfig.TopLevelConfig.OutFilePath, &AllConfig, &Record);

	FreeMemoryAll(&AllConfig, &Record);

	scf.clear();
	sef.clear();
	dcf.clear();
	contents.clear();
	sendProgressMax = 6;
	sendProgressValue = 0;

	QDirIterator dirIt(tmpDir, QDir::Files);
	while (dirIt.hasNext()) {
		QString file{dirIt.next()};
		QString name = dirIt.fileName();
		qint64 size = dirIt.fileInfo().size();
		if (FileMsgFactory::isScf(name) && scf.isEmpty()) {
			scf = file;
			sendProgressMax += size;
		} else if (FileMsgFactory::isSef(name) && sef.isEmpty()) {
			sef = file;
			sendProgressMax += size;
		} else if (FileMsgFactory::isDcf(name) && dcf.isEmpty()) {
			dcf = file;
			sendProgressMax += size;
		} else if (FileMsgFactory::isContentFile(name)) {
			contents.push_back(file);
			sendProgressMax += size;
		}
	}

	sendScf();

	return;
}

void KsdkImpl::play(const unsigned int index,
					KsdkResultCb resultCb)
{
	ProgramPlayReqMsg* msg = new ProgramPlayReqMsg{index};
	protocol->send(IMsgSp{msg}, [this, resultCb](bool ok) {
		if (!ok) {
			resultCb(KSDK_ERR_PLAY);
		} else {
			resultCb(KSDK_OK);
		}
	});
}

void KsdkImpl::setBrightness(const unsigned int brightness,
			KsdkResultCb resultCb)
{
	DisplaySetReqMsg* msg = new DisplaySetReqMsg;
	msg->setBrightnessMode(true);
	msg->setBrightnessLevel(brightness);
	protocol->send(IMsgSp{msg}, [this, resultCb](bool ok) {
		if (!ok) {
			resultCb(KSDK_ERR_SET_BRIGHTNESS);
		} else {
			resultCb(KSDK_OK);
		}
	});
}

void KsdkImpl::sendProgress(qint64 chunk)
{
	sendProgressValue += chunk;
	if (!sendProgressCb) return;
	sendProgressCb(sendProgressMax, sendProgressValue);
}

void KsdkImpl::sendResult(KsdkResult result)
{
	if (!sendResultCb) return;
	sendResultCb(result);
}

void KsdkImpl::sendScf()
{
	QFileInfo info{scf};

	if (!info.exists()) {
		sendSef();
		return;
	}

	protocol->sendFile(scf, [this](bool ok) {
		if (!ok) {
			sendResult(KSDK_ERR_SEND_SCF);
		} else {
			sendSef();
		}
	}, sendProgressRecver);
}

void KsdkImpl::sendSef()
{
	QFileInfo info{sef};

	if (!info.exists()) {
		sendDcf();
		return;
	}

	protocol->sendFile(sef, [this](bool ok) {
		if (!ok) {
			sendResult(KSDK_ERR_SEND_SEF);
		} else {
			sendDcf();
		}
	}, sendProgressRecver);
}

void KsdkImpl::sendDcf()
{
	QFileInfo info{dcf};

	if (!info.exists()) {
		sendSyncTime();
		return;
	}

	protocol->sendFile(dcf, [this](bool ok) {
		if (!ok) {
			sendResult(KSDK_ERR_SEND_DCF);
		} else {
			sendSyncTime();
		}
	}, sendProgressRecver);
}

void KsdkImpl::sendSyncTime()
{
	DisplaySetReqMsg* msg = new DisplaySetReqMsg;
	msg->setDateTimeNow();
	protocol->send(IMsgSp{msg}, [this](bool ok) {
		if (!ok) {
			sendResult(KSDK_ERR_SYNC_TIME);
		} else {
			sendProgress(1);
			sendDisplaySave();
		}
	});
}

void KsdkImpl::sendDisplaySave()
{
	protocol->send(IMsgSp{new DisplaySaveReqMsg}, [this](bool ok) {
		if (!ok) {
			sendResult(KSDK_ERR_SAVE_DISPLAY);
		} else {
			sendProgress(1);
			sendScreenClose();
		}
	});
}

void KsdkImpl::sendScreenClose()
{
	DisplaySetReqMsg* msg = new DisplaySetReqMsg;
	msg->setOpenClose(false);
	protocol->send(IMsgSp{msg}, [this](bool ok) {
		if (!ok) {
			sendResult(KSDK_ERR_CLOSE_SCREEN);
		} else {
			sendProgress(1);
			sendContentClear();
		}
	});
}

void KsdkImpl::sendContentClear()
{
	protocol->send(IMsgSp{new ContentCtrlReqMsg{false}}, [this](bool ok) {
		if (!ok) {
			sendResult(KSDK_ERR_CLEAR_CONTENT);
		} else {
			sendProgress(1);
			sendContentFile();
		}
	});
}

void KsdkImpl::sendContentFile()
{
	if (contents.empty()) {
		sendContentSave();
		return;
	}

	QString path = contents.front();
	contents.pop_front();

	protocol->sendFile(path, [this](bool ok) {
		if (!ok) {
			sendResult(KSDK_ERR_SEND_CONTENT);
		} else {
			sendContentFile();
		}
	}, sendProgressRecver);
}

void KsdkImpl::sendContentSave()
{
	protocol->send(IMsgSp{new ContentCtrlReqMsg{true}}, [this](bool ok) {
		if (!ok) {
			sendResult(KSDK_ERR_SAVE_CONTENT);
		} else {
			sendProgress(1);
			sendScreenOpen();
		}
	});
}

void KsdkImpl::sendScreenOpen()
{
	DisplaySetReqMsg* msg = new DisplaySetReqMsg;
	msg->setOpenClose(true);
	protocol->send(IMsgSp{msg}, [this](bool ok) {
		if (!ok) {
			sendResult(KSDK_ERR_OPEN_SCREEN);
		} else {
			sendProgress(1);
			sendResult(KSDK_OK);
		}
	});
}
