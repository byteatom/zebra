#include "Message.h"

#include <string>
#include <math.h>

#include <QFileInfo>
#include <QDateTime>

enum MsgType {
	PARA_FILE_SET = 0,
	PARA_FILE_GET = 1,
	DISPLAY_FILE_SET = 2,
	DISPLAY_FILE_GET = 3,
	CONTENT_FILE_SET = 4,
	CONTENT_FILE_GET = 5,
	POWERON_FILE_SET = 6,
	INFO_FILE_GET = 7,
	UPDATE_FILE_SET = 8,
	LOG_FILE_GET = 9,
	SCAN_FILE_SET = 10,
	PARA_EXT_FILE_GET = 11,
	NET_PARA_FILE_SET = 12,
	NET_PARA_FILE_GET = 13,
	BOX_INFO_FILE_SET = 14,
	BOX_INFO_FILE_GET = 15,
	WIFI_SET = 17,
	CONTENT_CLEAR = 20,
	CONTENT_SAVE = 21,
	DISPLAY_SET = 50,
	DISPLAY_GET = 51,
	DISPLAY_SAVE = 52,
	INFO_SET = 53,
	INFO_GET = 54,
	TEMPERATURE_SET = 55,
	CHARACTER_SET = 57,
	CHARACTER_NOREPLY_SET = 58,
	UART_CTRL_SET = 59,
	SCANDOT_TEST = 60,
	ENTERMOZDE_SET = 61,
	ENTERMOZDE_GET = 62,
	RELATIVETIME_SET = 70,
	UART_TEST = 80,
	RTC_TEST = 81,
	PROGRAM_PLAY = 91,
	BUTTON_SET = 98,
	DEVICE_RESET = 99,
};

const char Msg::PROLOGUE[Msg::LOGUE_SIZE]{'K', 'L', 'B'};
const char Msg::EPILOGUE[Msg::LOGUE_SIZE]{'K', 'L', 'E'};
const char Msg::HOST_ADDR[Msg::ADDR_SIZE]{'0', '0', '0', '0'};
const char Msg::CARD_ADDR_DEFAULT[Msg::ADDR_SIZE]{'0', '0', '0', '1'};
const char Msg::CARD_ADDR_BROADCAST[Msg::ADDR_SIZE]{'9', '9', '9', '9'};

Msg::Msg(int size):
	head{(Head*)new char[size]},
	alloc{true}
{
	dataLen = size - (int)(sizeof(Head) + sizeof(Tail));
}

Msg::Msg(const char *buf):
	head{(Head*)buf},
	alloc{false}
{
	dataLen = stoi(head->dataSize, sizeof(head->dataSize), 10);
}

Msg::Msg(const char *buf, int size):
	Msg{size}
{
	memcpy(head, buf, size);
}

Msg::Msg(int type, const char *data, int dataSize) :
	Msg{sizeof(Head) + dataSize + sizeof(Tail)}
{
	memcpy(head->prologue, PROLOGUE, sizeof(head->prologue));
	memcpy(head->dst, CARD_ADDR_DEFAULT, sizeof(head->dst));
	memcpy(head->src, HOST_ADDR, sizeof(head->src));
	ito4b10(head->type, type);
	ito4b10(head->dataSize, dataSize);
	if (nullptr != data && dataSize > 0)
		memcpy(head->data, data, dataSize);
	memcpy(tail()->epilogue, EPILOGUE, sizeof(tail()->epilogue));
	setSum();
}

Msg::~Msg()
{
	if (alloc) delete[] (char*)head;
}

char* Msg::buf()
{
	return (char*)head;
}

int Msg::size()
{
	return dataLen  + sizeof(Head) + sizeof(Tail);
}

char* Msg::data()
{
	return (char*)(head + 1);
}

int Msg::dataSize()
{
	return dataLen;
}

int Msg::type()
{
	return stoi(head->type, sizeof(head->type), 10);
}

Msg::Tail* Msg::tail()
{
	return (Tail*)(data() + dataSize());
}

uint8_t Msg::sum()
{
	uint8_t sum = 0;
	uint8_t *buf = (uint8_t *)head;
	for (unsigned int i = 0; i < size() - sizeof(tail()->sum); ++i)
		sum += buf[i];
	return sum;
}

void Msg::setSum()
{
	ito2b16(tail()->sum, sum());
}

bool Msg::valid()
{
	Tail* tail = this->tail();
	return  ((_strnicmp(head->prologue, PROLOGUE, sizeof(head->prologue)) == 0)
			&& (size() == (sizeof(Head) + stoi(head->dataSize, sizeof(head->dataSize), 10) + sizeof(Tail)))
			&& (_strnicmp(tail->epilogue, EPILOGUE, sizeof(tail->epilogue)) == 0)
			&& (sum() == stoi(tail->sum, sizeof(tail->sum), 16)));
}

bool Msg::isRspOk()
{
	int msgType = type() - 5000;

	if (/*msgType == PARA_FILE_SET ||*/	//0 1, 8?
		msgType == DISPLAY_FILE_SET ||
		msgType == CONTENT_FILE_SET ||
		msgType == POWERON_FILE_SET ||
		msgType == UPDATE_FILE_SET ||
		msgType == SCAN_FILE_SET ||
		msgType == NET_PARA_FILE_SET ||
		msgType == BOX_INFO_FILE_SET ||
		msgType == CONTENT_CLEAR ||
		msgType == CONTENT_SAVE ||
		msgType == DISPLAY_SET ||
		msgType == DISPLAY_SAVE ||
		msgType == INFO_SET ||
		msgType == RELATIVETIME_SET ||
		msgType == PROGRAM_PLAY)
		return (*data() == '0');
	else
		return true;
}

bool Msg::isRspFor(IMsgSp req)
{
	if (!valid() || !isRspOk()) return false;
	Head* reqHead = (Head*)req->buf();
	return (head->type[0] == '5'
			&& reqHead->type[1] == head->type[1]
			&& reqHead->type[2] == head->type[2]
			&& reqHead->type[3] == head->type[3]);
}

char* Msg::ito4b10(char *str, int value)
{
	str[0] = value/1000 +'0';
	str[1] = value%1000/100 +'0';
	str[2] = value%100/10 +'0';
	str[3] = value%10 +'0';

	return str;
}

char* Msg::ito2b16(char *str, int value)
{
	static const char d2x[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	str[0] = d2x[value/16];
	str[1] = d2x[value%16];
	return str;
}

int Msg::stoi(char *str, int len, int base)
{
	return std::stoi(std::string(str, len), nullptr, base);
}

EmptyReqMsg::EmptyReqMsg(int type):
	Msg{type, nullptr, 0}
{
}

FileSetReqMsg::FileSetReqMsg(int type, char* name, int offset, int fileDataSize) :
	Msg{type, nullptr, sizeof(FileMsgDataHead) + fileDataSize},
	fileDataSize{fileDataSize}
{
	FileMsgDataHead *fileDataHead = (FileMsgDataHead*)data();

	strncpy(fileDataHead->name, name, sizeof(fileDataHead->name));
	fileDataHead->name[sizeof(fileDataHead->name) - 1] = 0;
	fileDataHead->offset = offset;
}

char* FileSetReqMsg::fileData()
{
	return data() + sizeof(FileMsgDataHead);
}

#define SCF_EXT "SC"
#define SEF_EXT "SE"
#define DCF_EXT "DC"
#define CCF_EXT "CCF"
#define PCF_EXT "PCF"
#define ACF_EXT "ACF"
#define BCF_EXT "BCF"
#define RCF_EXT "RC"
#define NCF_EXT "NC"

bool FileMsgFactory::isScf(const QString& path)
{
	return  QFileInfo(path).suffix().toUpper().startsWith(SCF_EXT);
}

bool FileMsgFactory::isSef(const QString& path)
{
	return  QFileInfo(path).suffix().toUpper().startsWith(SEF_EXT);
}

bool FileMsgFactory::isDcf(const QString& path)
{
	return  QFileInfo(path).suffix().toUpper().startsWith(DCF_EXT);
}

bool FileMsgFactory::isContentFile(const QString& path)
{
	QString suffix = QFileInfo(path).suffix().toUpper();
	return  suffix.startsWith(CCF_EXT)
			|| suffix.startsWith(PCF_EXT)
			|| suffix.startsWith(ACF_EXT)
			|| suffix.startsWith(BCF_EXT);
}

bool FileMsgFactory::isRcf(const QString& path)
{
	return  QFileInfo(path).suffix().toUpper().startsWith(RCF_EXT);
}

bool FileMsgFactory::isNcf(const QString& path)
{
	return  QFileInfo(path).suffix().toUpper().startsWith(NCF_EXT);
}


FileMsgFactory::FileMsgFactory(const QString& path):
	file{path}, eof{false}
{
	static struct FileMapItem {
		const char* suffix;
		MsgType type;
	} fileTypeMap[] = {
		{SCF_EXT, PARA_FILE_SET},
		{SEF_EXT, SCAN_FILE_SET},
		{DCF_EXT, DISPLAY_FILE_SET},
		{CCF_EXT, CONTENT_FILE_SET},
		{PCF_EXT, CONTENT_FILE_SET},
		{ACF_EXT, CONTENT_FILE_SET},
		{BCF_EXT, CONTENT_FILE_SET},
		{RCF_EXT, CONTENT_FILE_SET},
		{NCF_EXT, NET_PARA_FILE_SET}
	};

	msgType = -1;
	QString suffix = QFileInfo(path).suffix().toUpper();
	for (int i = 0; i < sizeof(fileTypeMap) / sizeof(FileMapItem); ++i) {
		if (suffix.startsWith(fileTypeMap[i].suffix))
			msgType =  fileTypeMap[i].type;
	}
	if (msgType < 0) return;

	if (!file.exists()) return;

	QByteArray fileName = QFileInfo(path).fileName().toUtf8();
	if (QString::compare(fileName, "ALL.SEX", Qt::CaseInsensitive) == 0)
		strncpy(name, "ALL.sef", sizeof(name));
	else
		strncpy(name, fileName.data(), sizeof(name));
	name[sizeof(name) - 1] = 0;

	file.open(QFile::ReadOnly);
	size = file.size();
}

IMsgSp FileMsgFactory::next()
{
	if (!file.isOpen()) return nullptr;

	if (eof) {
		FileSetReqMsg *msg = new FileSetReqMsg(msgType, name, 0, 0);
		eof = false;
		return IMsgSp {msg};
	}

	if (file.atEnd()) return nullptr;

	qint64 pos = file.pos();
	int unit = std::min((qint64)FileSetReqMsg::FILE_DATA_MAX_SIZE, size - pos);
	FileSetReqMsg *msg = new FileSetReqMsg(msgType, name, pos, unit);
	IMsgSp msgsp{msg};
	int readed = file.read(msg->fileData(), unit);
	if (readed < unit) return nullptr;
	msg->setSum();
	if (!file.seek(pos + unit)) {
		file.seek(size - 1);
		if (unit == FileSetReqMsg::FILE_DATA_MAX_SIZE) eof = true;
	}
	return msgsp;
}

ScanDotReqMsg::ScanDotReqMsg(const char *send, int size) :
	Msg{SCANDOT_TEST, send, size}
{
	setSum();
}

WifiSetReqMsg::WifiSetReqMsg(char* ssid, char* pwd) :
	Msg{WIFI_SET, nullptr, DATA_MAX_SIZE}
{
	sprintf(data(), "%c%s%c%s", strlen(ssid), ssid, strlen(pwd), pwd);
	setSum();
}

ContentCtrlReqMsg::ContentCtrlReqMsg(bool save) :
	Msg{save ? CONTENT_SAVE : CONTENT_CLEAR, nullptr, 16}
{
	strcpy(data(), save ? "SAVE_CONTENT" : "CLEAR_CONTENT");
	setSum();
}

RelativeTimeSetReqMsg::RelativeTimeSetReqMsg() :
	Msg{RELATIVETIME_SET, nullptr, 12}
{
	*data() = 0;
}
void RelativeTimeSetReqMsg::setTime(QTime time)
{
	sprintf(data() + strlen(data()), "T%d:%d:%d,",
		time.hour(), time.minute(), time.second());
	setSum();
}
DisplaySetReqMsg::DisplaySetReqMsg() :
	Msg{DISPLAY_SET, nullptr, DATA_MAX_SIZE}
{
	*data() = 0;
	//memset(data(), 0, dataSize());
}

void DisplaySetReqMsg::setDate(QDate date)
{
	sprintf(data() + strlen(data()), "D%d-%d-%d-%d,",
		date.year(), date.month(), date.day(), date.dayOfWeek());
	setSum();
}

void DisplaySetReqMsg::setTime(QTime time)
{
	sprintf(data() + strlen(data()), "T%d:%d:%d,",
		time.hour(), time.minute(), time.second());
	setSum();
}

void DisplaySetReqMsg::setDateTimeNow()
{
	QDateTime now = QDateTime::currentDateTime();
	setDate(now.date());
	setTime(now.time());
}

void DisplaySetReqMsg::setOpenClose(bool open)
{
	strcpy(data() + strlen(data()), open ? "S1," : "S0,");
	setSum();
}

void DisplaySetReqMsg::setOpenTime(bool open, QTime time)
{
	sprintf(data() + strlen(data()), "N%d-0-0-0-1-1-1-1-1-1-1-1-1-1-2000-01-01-%d-%d-%d,",
			open ? 1 : 0,
			time.hour(),
			time.minute(),
			time.second());
	setSum();
}

void DisplaySetReqMsg::setCloseTime(bool close, QTime time)
{
	sprintf(data() + strlen(data()), "F%d-0-0-0-1-1-1-1-1-1-1-1-1-1-2000-01-01-%d-%d-%d,",
			close ? 1 : 0,
			time.hour(),
			time.minute(),
			time.second());
	setSum();
}

void DisplaySetReqMsg::setBrightnessMode(bool manual)
{
	strcpy(data() + strlen(data()), manual ? "l0," : "l1,");
	setSum();
}

void DisplaySetReqMsg::setBrightnessLevel(int level)
{
	sprintf(data() + strlen(data()), "L%d,", level);
	setSum();
}

DisplaySaveReqMsg::DisplaySaveReqMsg():
	Msg{DISPLAY_SAVE, nullptr, 32}
{
	strcpy(data(), "SAVE_DISPLAY_PARA");
	setSum();
}

InfoGetReqMsg::InfoGetReqMsg() :
	Msg{INFO_GET, nullptr, 32}
{
	strcpy(data(), "X,P,V,D,d,w,b,T,I,i,S,W,H,c,g,");
	setSum();
}

InfoGetRspData::InfoGetRspData(const QByteArray& data)
{
	QList<QByteArray> infos = data.split(',');
	for (QByteArray& info: infos) {
		const char tag = info[0];
		info.remove(0, 1);
		if ('X' == tag)
			model = info;
		else if ('P' == tag)
			product = info;
		else if ('V' == tag)
			version = info;
		else if ('D' == tag)
			totalSpace = info;
		else if ('d' == tag)
			freeSpace = info;
		else if ('w' == tag)
			powerVoltage = info;
		else if ('b' == tag)
			batteryVoltage = info;
		else if ('T' == tag)
			temperature = info;
		else if ('I' == tag)
			intfStats08 = info;
		else if ('i' == tag)
			intfStats12 = info;
		else if ('S' == tag)
			id = info;
		else if ('W' == tag)
			width = info;
		else if ('H' == tag)
			height = info;
		else if ('c' == tag)
			colorSpace = info;
	}
}

ProgramPlayReqMsg::ProgramPlayReqMsg(int id) :
	Msg{PROGRAM_PLAY, nullptr, id < 10 ? 2 : 3}
{
	std::string index = std::to_string(id);
	*data() = 'P';
	memcpy(data() + 1, index.c_str(), index.size());
	setSum();
}
