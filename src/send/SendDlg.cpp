#include "SendDlg.h"
#include "ui_SendDlg.h"

#include <QUuid>
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QTimer>

#include "Message.h"
#include "UtilQt.h"

SendDlg::SendDlg(QWidget *parent, IScreen* screen, ISetting* setting) :
	QDialog(parent, Qt::MSWindowsFixedSizeDialogHint),
	ui(new Ui::SendDlg),
	screen{screen},
	setting{setting},
	progressRecver{std::bind(&SendDlg::setProgress, this, std::placeholders::_1)},
	progressMax{0},
	progress{0}
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setWindowIcon(QIcon{"image/icon/send_64.png"});

	protocol = screen->protocol();
	protocol->open();

	ui->peer->setText(protocol->description() + " " + screen->screenPara().cardModel);

	connect(ui->close, &QPushButton::clicked, this, &SendDlg::reject);
	connect(ui->send, &QPushButton::clicked, this, &SendDlg::start);

	QTimer::singleShot(0, [this](){start();});
}

SendDlg::~SendDlg()
{
	delete ui;
}

void SendDlg::done(int ret)
{
	protocol->close();
	QDialog::done(ret);
}

void SendDlg::start()
{
	scf.clear();
	sef.clear();
	dcf.clear();
	contents.clear();
	progressMax = 8;	//step function number
	progress = 0;

	ui->progress->setStyleSheet("QProgressBar::chunk {background-color: gray;}");
	ui->progress->setMaximum(222);
	ui->progress->setValue(0);
	ui->send->setEnabled(false);

	sendInfoGet();
}

void SendDlg::setProgress(qint64 chunk)
{
	ui->progress->setValue(progress+=chunk);
}

void SendDlg::fail(const QString& status)
{
	ui->status->setText(status);
	ui->send->setEnabled(true);
}

void SendDlg::finish()
{
	ui->progress->setStyleSheet("QProgressBar::chunk {background-color: #00AA00;}");
	ui->status->setText(tr("Finished!"));
	ui->send->setEnabled(true);
}

void SendDlg::sendInfoGet()
{
	ui->status->setText(tr("Getting card information......"));

	protocol->send(IMsgSp{new InfoGetReqMsg}, [this](QByteArray data) {
		bool ok = (data.size() > 0);
		if (ok) {
			InfoGetRspData rsp{data};
			screen->screenPara().cardModel = rsp.model;
			ui->peer->setText(protocol->description() + " " + screen->screenPara().cardModel);
			setProgress(1);
			build();
		} else {
			fail(tr("Failed to get card information!"));
		}
	});
}

void SendDlg::build()
{
	ui->status->setText(tr("Preparing file......"));

	QString dir = UtilQt::uuidDir(setting->tempDir());

	QString ret = screen->buildFiles(dir, false);
	if (!ret.isEmpty()) {
		fail(ret);
		return;
	}

	QDirIterator dirIt(dir, QDir::Files);
	while (dirIt.hasNext()) {
		QString file{dirIt.next()};
		QString name = dirIt.fileName();
		qint64 size = dirIt.fileInfo().size();
		if (FileMsgFactory::isScf(name)) {
			scf = file;
			progressMax += size;
		} else if (FileMsgFactory::isSef(name)) {
			sef = file;
			progressMax += size;
		} else if (FileMsgFactory::isDcf(name)) {
			dcf = file;
			progressMax += size;
		} else if (FileMsgFactory::isContentFile(name)) {
			contents.push_back(file);
			progressMax += size;
		}
	}

	ui->progress->setMaximum(progressMax);
	setProgress(1);

	sendScf();
}

void SendDlg::sendScf()
{
	QFileInfo info{scf};

	if (!info.exists()) {
		sendSef();
		return;
	}

	QString name = info.fileName();
	ui->status->setText(statusFileSending.arg(name));

	protocol->sendFile(scf, [this, name](bool ok) {
		if (!ok) {
			fail(statusFileFailed.arg(name));
		} else {
			sendSef();
		}
	}, progressRecver);
}

void SendDlg::sendSef()
{
	QFileInfo info{sef};

	if (!info.exists()) {
		sendDcf();
		return;
	}

	QString name = info.fileName();
	ui->status->setText(statusFileSending.arg(name));

	protocol->sendFile(sef, [this, name](bool ok) {
		if (!ok) {
			fail(statusFileFailed.arg(name));
		} else {
			sendDcf();
		}
	}, progressRecver);
}

void SendDlg::sendDcf()
{
	QFileInfo info{dcf};

	if (!info.exists()) {
		sendSyncTime();
		return;
	}

	QString name = info.fileName();
	ui->status->setText(statusFileSending.arg(name));

	protocol->sendFile(dcf, [this, name](bool ok) {
		if (!ok) {
			fail(statusFileFailed.arg(name));
		} else {
			sendSyncTime();
		}
	}, progressRecver);
}

void SendDlg::sendSyncTime()
{
	ui->status->setText(tr("Synchronizing time......"));

	DisplaySetReqMsg* msg = new DisplaySetReqMsg;
	msg->setDateTimeNow();
	protocol->send(IMsgSp{msg}, [this](bool ok) {
		if (!ok) {
			fail(tr("Failed to synchronize time!"));
		} else {
			setProgress(1);
			sendDisplaySave();
		}
	});
}

void SendDlg::sendDisplaySave()
{
	ui->status->setText(tr("Saving display file......"));

	protocol->send(IMsgSp{new DisplaySaveReqMsg}, [this](bool ok) {
		if (!ok) {
			fail(tr("Failed to save display file!"));
		} else {
			setProgress(1);
			sendScreenClose();
		}
	});
}

void SendDlg::sendScreenClose()
{
	ui->status->setText(tr("Closing screen......"));

	DisplaySetReqMsg* msg = new DisplaySetReqMsg;
	msg->setOpenClose(false);
	protocol->send(IMsgSp{msg}, [this](bool ok) {
		if (!ok) {
			fail(tr("Failed to close scree!"));
		} else {
			setProgress(1);
			sendContentClear();
		}
	});
}

void SendDlg::sendContentClear()
{
	ui->status->setText(tr("Clearing content......"));

	protocol->send(IMsgSp{new ContentCtrlReqMsg{false}}, [this](bool ok) {
		if (!ok) {
			fail(tr("Failed to clear content!"));
		} else {
			setProgress(1);
			sendContentFile();
		}
	});
}

void SendDlg::sendContentFile()
{
	if (contents.empty()) {
		sendContentSave();
		return;
	}

	QString path = contents.front();
	contents.pop_front();
	QString name = QFileInfo(path).fileName();

	ui->status->setText(statusFileSending.arg(name));

	protocol->sendFile(path, [this, name](bool ok) {
		if (!ok) {
			fail(statusFileFailed.arg(name));
		} else {
			sendContentFile();
		}
	}, progressRecver);
}

void SendDlg::sendContentSave()
{
	ui->status->setText(tr("Saving content file......"));

	protocol->send(IMsgSp{new ContentCtrlReqMsg{true}}, [this](bool ok) {
		if (!ok) {
			fail(tr("Failed to save content file!"));
		} else {
			setProgress(1);
			sendScreenOpen();
		}
	});
}

void SendDlg::sendScreenOpen()
{
	ui->status->setText(tr("Opening screen......"));

	DisplaySetReqMsg* msg = new DisplaySetReqMsg;
	msg->setOpenClose(true);
	protocol->send(IMsgSp{msg}, [this](bool ok) {
		if (!ok) {
			fail(tr("Failed to open screen!"));
		} else {
			setProgress(1);
			//sendBrightness();
			finish();
		}
	});
}

void SendDlg::sendBrightness()
{
	ui->status->setText(tr("Setting brightness......"));

	DisplaySetReqMsg* msg = new DisplaySetReqMsg;
	msg->setBrightnessLevel(screen->bright().manualBright);
	protocol->send(IMsgSp{msg}, [this](bool ok) {
		if (!ok) {
			fail(tr("Failed to set brightness!"));
		} else {
			setProgress(1);
			finish();
		}
	});
}
