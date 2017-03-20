#include "BrightDlg.h"
#include "ui_BrightDlg.h"
#include "Message.h"

BrightDlg::BrightDlg(IMainWnd *mainWnd_, QWidget *parent) :
	QDialog(parent),
	mainWnd(mainWnd_),
	ui(new Ui::BrightDlg)
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setWindowIcon(QIcon(QPixmap("image/icon/bright_48.png")));

	if (!mainWnd) return;

	INode *node = mainWnd->currentNode(INode::SCREEN);
	if (!screen) screen = dynamic_cast<IScreen*>(node);
	if (screen) bright = screen->bright();

	radios = new QButtonGroup(this);
	radios->addButton(ui->radioButtonManual);
	radios->addButton(ui->radioButtonTiming);
	radios->addButton(ui->radioButtonAuto);

	ui->horizontalSliderManual->setValue(bright.manualBright);
	ui->horizontalSliderTiming1->setValue(bright.timingBright[0]);
	ui->horizontalSliderTiming2->setValue(bright.timingBright[1]);
	ui->horizontalSliderTiming3->setValue(bright.timingBright[2]);
	ui->horizontalSliderTiming4->setValue(bright.timingBright[3]);
	ui->horizontalSliderTimingDefault->setValue(bright.defaultBright);

	ui->checkBoxTiming1->setChecked(bright.timingCheck[0]);
	ui->checkBoxTiming2->setChecked(bright.timingCheck[1]);
	ui->checkBoxTiming3->setChecked(bright.timingCheck[2]);
	ui->checkBoxTiming4->setChecked(bright.timingCheck[3]);

	ui->timeEditStart1->setTime(bright.startTime[0]);
	ui->timeEditStart2->setTime(bright.startTime[1]);
	ui->timeEditStart3->setTime(bright.startTime[2]);
	ui->timeEditStart4->setTime(bright.startTime[3]);

	ui->timeEditEnd1->setTime(bright.endTime[0]);
	ui->timeEditEnd2->setTime(bright.endTime[1]);
	ui->timeEditEnd3->setTime(bright.endTime[2]);
	ui->timeEditEnd4->setTime(bright.endTime[3]);

	selectMode([this]()->QAbstractButton*{
					if (bright.manualSel) return ui->radioButtonManual;
					if (bright.timingSel) return ui->radioButtonTiming;
					return ui->radioButtonAuto; }() );

	connect(radios, static_cast<void (QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),
			this, &BrightDlg::selectMode);
	connect(ui->pushButtonCancel, &QPushButton::clicked, this, &QDialog::close);
	connect(ui->pushButtonSave, &QPushButton::clicked, this, &BrightDlg::save);
	connect(ui->pushButtonSend, &QPushButton::clicked, this, &BrightDlg::send);
	connect(ui->checkBoxTiming1, &QCheckBox::toggled, [this](bool checked) {
		ui->timeEditStart1->setEnabled(checked);
		ui->line1->setEnabled(checked);
		ui->timeEditEnd1->setEnabled(checked);
		ui->horizontalSliderTiming1->setEnabled(checked);
	});
	connect(ui->checkBoxTiming2, &QCheckBox::toggled, [this](bool checked) {
		ui->timeEditStart2->setEnabled(checked);
		ui->line2->setEnabled(checked);
		ui->timeEditEnd2->setEnabled(checked);
		ui->horizontalSliderTiming2->setEnabled(checked);
	});
	connect(ui->checkBoxTiming3, &QCheckBox::toggled, [this](bool checked) {
		ui->timeEditStart3->setEnabled(checked);
		ui->line3->setEnabled(checked);
		ui->timeEditEnd3->setEnabled(checked);
		ui->horizontalSliderTiming3->setEnabled(checked);
	});
	connect(ui->checkBoxTiming4, &QCheckBox::toggled, [this](bool checked) {
		ui->timeEditStart4->setEnabled(checked);
		ui->line4->setEnabled(checked);
		ui->timeEditEnd4->setEnabled(checked);
		ui->horizontalSliderTiming4->setEnabled(checked);
	});
}

BrightDlg::~BrightDlg()
{
	if (screen) screen->protocol()->close();
	delete ui;
}

void BrightDlg::selectMode(QAbstractButton *button)
{
	if (button == ui->radioButtonManual) {
		ui->textEdit->setText(tr("Adjust to the left will reduce brightness, to the right will increase the brightness"));
		ui->radioButtonManual->setChecked(bright.manualSel);
		ui->frameManual->setEnabled(true);
		ui->frameTiming->setEnabled(false);
	} else if (button == ui->radioButtonTiming) {
		ui->textEdit->setText(tr("Set the brightness of different time periods, other use the default brightness"));
		ui->radioButtonTiming->setChecked(bright.timingSel);
		ui->frameManual->setEnabled(false);
		ui->frameTiming->setEnabled(true);
	} else if (button == ui->radioButtonAuto) {
		ui->textEdit->setText(tr("Auto bright need use luminance sensor"));
		ui->radioButtonAuto->setChecked(bright.autoSel);
		ui->frameManual->setEnabled(false);
		ui->frameTiming->setEnabled(false);
	}
}

void BrightDlg::getUiAttr()
{
	bright.manualSel = ui->radioButtonManual->isChecked();
	bright.timingSel = ui->radioButtonTiming->isChecked();
	bright.autoSel = ui->radioButtonAuto->isChecked();

	bright.manualBright = ui->horizontalSliderManual->value();
	bright.timingBright[0] = ui->horizontalSliderTiming1->value();
	bright.timingBright[1] = ui->horizontalSliderTiming2->value();
	bright.timingBright[2] = ui->horizontalSliderTiming3->value();
	bright.timingBright[3] = ui->horizontalSliderTiming4->value();
	bright.defaultBright = ui->horizontalSliderTimingDefault->value();

	bright.timingCheck[0] = ui->checkBoxTiming1->isChecked();
	bright.timingCheck[1] = ui->checkBoxTiming2->isChecked();
	bright.timingCheck[2] = ui->checkBoxTiming3->isChecked();
	bright.timingCheck[3] = ui->checkBoxTiming4->isChecked();

	bright.startTime[0] = ui->timeEditStart1->time();
	bright.startTime[1] = ui->timeEditStart2->time();
	bright.startTime[2] = ui->timeEditStart3->time();
	bright.startTime[3] = ui->timeEditStart4->time();

	bright.endTime[0] = ui->timeEditEnd1->time();
	bright.endTime[1] = ui->timeEditEnd2->time();
	bright.endTime[2] = ui->timeEditEnd3->time();
	bright.endTime[3] = ui->timeEditEnd4->time();
}

void BrightDlg::save()
{
	if (screen) {
		getUiAttr();
		IScreen::Bright &scrSetBright = screen->bright();
		scrSetBright = bright;
	}
	QDialog::close();
}

void BrightDlg::send()
{
	if (!screen || !screen->protocol()) {
		QDialog::close();
		return;
	}

	getUiAttr();

	DisplaySetReqMsg *msgSet = new DisplaySetReqMsg;
	if (bright.manualSel) {
		msgSet->setBrightnessMode(true);
		msgSet->setBrightnessLevel(ui->horizontalSliderManual->value());
	} else {
		msgSet->setBrightnessMode(false);
	}

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
