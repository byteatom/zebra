#include "OnOffDlg.h"
#include "ui_OnOffDlg.h"
#include <QCheckBox>
#include <QMessageBox>
#include "Message.h"

OnOffDlg::OnOffDlg(IMainWnd *mainWnd_, QWidget *parent) :
	QDialog(parent),
	mainWnd(mainWnd_),
	ui(new Ui::OnOffDlg)
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setWindowIcon(QIcon(QPixmap("image/icon/onoff_48.png")));

	if (!mainWnd) return;

	INode *node = mainWnd->currentNode(INode::SCREEN);
	if (!screen) screen = dynamic_cast<IScreen*>(node);
	if (screen) onOff = screen->onOff();

	ui->textEdit->setText(tr("If use Udisk to change power and time, please save config"));
	ui->checkBoxTimerSwitch->setChecked(onOff.timerSwitch);
	ui->timeEditOpenTime->setTime(onOff.openTime);
	ui->timeEditCloseTime->setTime(onOff.closeTime);
	if (!ui->checkBoxTimerSwitch->checkState()) {
		ui->timeEditOpenTime->setEnabled(false);
		ui->timeEditCloseTime->setEnabled(false);
	}

	connect(ui->checkBoxTimerSwitch, &QCheckBox::stateChanged, this,
			[this](int state)->void {
				if (state)
					onOff.timerSwitch = true;
				else
					onOff.timerSwitch = false;

				if (state) {
					ui->timeEditOpenTime->setEnabled(true);
					ui->timeEditCloseTime->setEnabled(true);
				} else {
					ui->timeEditOpenTime->setEnabled(false);
					ui->timeEditCloseTime->setEnabled(false);
				}
			});
	connect(ui->pushButtonCancel, &QPushButton::clicked, this, &QDialog::close);
	connect(ui->pushButtonSave, &QPushButton::clicked, this, &OnOffDlg::save);
	connect(ui->timeEditOpenTime, &QTimeEdit::timeChanged, this,
			[this](const QTime &time)->void {
				onOff.openTime = time;
			});
	connect(ui->timeEditCloseTime, &QTimeEdit::timeChanged, this,
			[this](const QTime &time)->void {
				onOff.closeTime = time;
			});
	connect(ui->pushButtonOpen, &QPushButton::clicked, this, &OnOffDlg::openScren);
	connect(ui->pushButtonClose, &QPushButton::clicked, this, &OnOffDlg::closeScren);
	connect(ui->pushButtonSend, &QPushButton::clicked, this, &OnOffDlg::send);
}

OnOffDlg::~OnOffDlg()
{
	if (screen) screen->protocol()->close();
	delete ui;
}

void OnOffDlg::save()
{
	if (screen) {
		IScreen::OnOff &scrSetOnOff = screen->onOff();
		scrSetOnOff = onOff;
	}
	QDialog::close();
}

void OnOffDlg::openScren()
{
	if (!screen || !screen->protocol()) {
		QDialog::close();
		return;
	}

	DisplaySetReqMsg *msg = new DisplaySetReqMsg;
	msg->setOpenClose(true);

	IMsgSp req(msg);
	screen->protocol()->open();
	screen->protocol()->send(req, [this](bool ok) {
		screen->protocol()->close();
		return 0;
	});
}

void OnOffDlg::closeScren()
{
	if (!screen || !screen->protocol()) {
		QDialog::close();
		return;
	}

	DisplaySetReqMsg *msg = new DisplaySetReqMsg;
	msg->setOpenClose(false);

	IMsgSp req(msg);
	screen->protocol()->open();
	screen->protocol()->send(req, [this](bool ok) {
		screen->protocol()->close();
		return 0;
	});
}

void OnOffDlg::send()
{
	if (!screen || !screen->protocol()) {
		QDialog::close();
		return;
	}

	DisplaySetReqMsg *msgSet = new DisplaySetReqMsg;
	msgSet->setOpenTime(onOff.timerSwitch, onOff.openTime);
	msgSet->setCloseTime(onOff.timerSwitch, onOff.closeTime);

	DisplaySaveReqMsg *msgSave = new DisplaySaveReqMsg;

	IMsgSp reqSet(msgSet);
	IMsgSp reqSave(msgSave);
	screen->protocol()->open();
	screen->protocol()->send(reqSet, [this](bool ok) {
		return 0;
	});
	screen->protocol()->send(reqSave, [this](bool ok) {
		screen->protocol()->close();
		if (ok) save();
		return 0;
	});
}
