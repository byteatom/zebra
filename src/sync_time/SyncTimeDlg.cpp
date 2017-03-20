#include "SyncTimeDlg.h"
#include "ui_SyncTimeDlg.h"
#include <QCheckBox>
#include <QTimer>
#include "Message.h"

SyncTimeDlg::SyncTimeDlg(IMainWnd *mainWnd_, QWidget *parent) :
	QDialog(parent),
	mainWnd(mainWnd_),
	ui(new Ui::SyncTimeDlg)
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setWindowIcon(QIcon(QPixmap("image/icon/sync_time_48.png")));

	if (!mainWnd) return;

	INode *node = mainWnd->currentNode(INode::SCREEN);
	if (!screen) screen = dynamic_cast<IScreen*>(node);
	if (screen) setTime = screen->syncTime();

	selectRadios = new QButtonGroup(this);
	selectRadios->addButton(ui->radioButtonManual);
	selectRadios->addButton(ui->radioButtonSystem);

	ui->timeEdit->setTime(setTime.time);
	ui->dateEdit->setDate(setTime.date);

	showSystemTime();
	selectMode([this]()->QAbstractButton*{
					if (setTime.manualSel) return ui->radioButtonManual;
					return ui->radioButtonSystem; }() );

	connect(selectRadios, static_cast<void (QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),
			this, &SyncTimeDlg::selectMode);
	connect(ui->pushButtonGetTime, &QPushButton::clicked, this,
			[this]() {
				ui->timeEdit->setTime(QDateTime::currentDateTime().time());
				ui->dateEdit->setDate(QDateTime::currentDateTime().date());
			});
	connect(ui->pushButtonCancel, &QPushButton::clicked, this, &QDialog::close);
	connect(ui->pushButtonSave, &QPushButton::clicked, this, &SyncTimeDlg::save);
	connect(ui->pushButtonSend, &QPushButton::clicked, this, &SyncTimeDlg::send);
}

SyncTimeDlg::~SyncTimeDlg()
{
	if (screen) screen->protocol()->close();
	delete ui;
}

void SyncTimeDlg::selectMode(QAbstractButton *button)
{
	if (button == ui->radioButtonManual) {
		ui->textEdit->setText(tr("Please set date and time you want to send, click <Get Time> will get system time"));
		ui->radioButtonManual->setChecked(setTime.manualSel);
		ui->frameManual->setEnabled(true);
		ui->frameSystem->setEnabled(false);
	} else if (button == ui->radioButtonSystem) {
		ui->textEdit->setText(tr("Automatically update the time from system when send data or save in Udisk"));
		ui->radioButtonSystem->setChecked(setTime.systemSel);
		ui->frameManual->setEnabled(false);
		ui->frameSystem->setEnabled(true);
		showSystemTime();
		if (!timer) {
			timer = new QTimer(this);
			connect(timer, &QTimer::timeout, this, &SyncTimeDlg::showSystemTime);
		}
		timer->start(1000);
	}
}

void SyncTimeDlg::getUiAttr()
{
	setTime.manualSel = ui->radioButtonManual->isChecked();
	setTime.systemSel = ui->radioButtonSystem->isChecked();
	setTime.date = ui->dateEdit->date();
	setTime.time = ui->timeEdit->time();
}

void SyncTimeDlg::showSystemTime()
{
	ui->labelSystemTime->setText(QDateTime::currentDateTime().toString(Qt::DefaultLocaleLongDate));
}

void SyncTimeDlg::save()
{
	if (screen) {
		getUiAttr();
		IScreen::SyncTime &scrSetTime = screen->syncTime();
		scrSetTime = setTime;
	}
	QDialog::close();
}

void SyncTimeDlg::send()
{
	if (!screen || !screen->protocol()) {
		QDialog::close();
		return;
	}

	getUiAttr();
	screen->protocol()->open();

	DisplaySetReqMsg *msgSet = new DisplaySetReqMsg;
	if (setTime.systemSel) {
		msgSet->setDate(QDateTime::currentDateTime().date());
		msgSet->setTime(QDateTime::currentDateTime().time());
	} else {
		msgSet->setDate(ui->dateEdit->date());
		msgSet->setTime(ui->timeEdit->time());
	}

	DisplaySaveReqMsg *msgSave = new DisplaySaveReqMsg;

	IMsgSp reqSet(msgSet);
	IMsgSp reqSave(msgSave);
	screen->protocol()->send(reqSet, [this](bool ok) {
		return 0;
	});

	screen->protocol()->send(reqSave, [this](bool ok) {
		screen->protocol()->close();
		if (ok) save();
		return 0;
	});
}
