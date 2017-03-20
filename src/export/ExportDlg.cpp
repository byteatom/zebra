#include "ExportDlg.h"
#include "ui_ExportDlg.h"

#include <QDir>
#include <QStorageInfo>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDesktopServices>

#include <UtilWin.h>

ExportDlg::ExportDlg(QWidget *parent, IScreen* screen) :
	QDialog(parent, Qt::MSWindowsFixedSizeDialogHint),
	ui(new Ui::ExportDlg),
	screen{screen},
	para{screen->screenPara()},
	onOff{screen->onOff()},
	sync{screen->syncTime()},
	bright{screen->bright()}
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setWindowIcon(QIcon{"image/icon/export_64.png"});

	timer.setInterval(1000);
	connect(&timer, &QTimer::timeout, [this]() {
		if (ui->timing->isChecked() && ui->timingNow->isChecked())
				ui->timingValue->setDateTime(QDateTime::currentDateTime());
		setUsbList();
	});
	timer.start();

	btgTiming.addButton(ui->timingNow, 0);
	btgTiming.addButton(ui->timingFixed, 1);
	connect(ui->timing, &QCheckBox::toggled, this, &ExportDlg::setWidgetStatus);
	connect(ui->timingNow, &QCheckBox::toggled, this, &ExportDlg::setWidgetStatus);
	connect(ui->timingFixed, &QCheckBox::toggled, this, &ExportDlg::setWidgetStatus);
	ui->timingNow->setChecked(sync.systemSel);
	ui->timingFixed->setChecked(sync.manualSel);
	ui->timingValue->setDate(sync.date);
	ui->timingValue->setTime(sync.time);

	connect(ui->onOff, &QCheckBox::toggled, this, &ExportDlg::setWidgetStatus);
	ui->onOff->setChecked(onOff.timerSwitch);
	ui->onValue->setTime(onOff.openTime);
	ui->offValue->setTime(onOff.closeTime);

	btgBright.addButton(ui->brightFixed, 0);
	btgBright.addButton(ui->brightSchedule, 1);
	btgBright.addButton(ui->brightAuto, 2);
	connect(ui->bright, &QCheckBox::toggled, this, &ExportDlg::setWidgetStatus);
	connect(ui->brightFixed, &QCheckBox::toggled, this, &ExportDlg::setWidgetStatus);
	connect(ui->brightSchedule, &QCheckBox::toggled, this, &ExportDlg::setWidgetStatus);
	connect(ui->bright0, &QCheckBox::toggled, this, &ExportDlg::setWidgetStatus);
	connect(ui->bright1, &QCheckBox::toggled, this, &ExportDlg::setWidgetStatus);
	connect(ui->bright2, &QCheckBox::toggled, this, &ExportDlg::setWidgetStatus);
	connect(ui->bright3, &QCheckBox::toggled, this, &ExportDlg::setWidgetStatus);
	connect(ui->brightAuto, &QCheckBox::toggled, this, &ExportDlg::setWidgetStatus);
	ui->brightFixed->setChecked(bright.manualSel);
	ui->brightFixedValue->setValue(bright.manualBright);
	ui->brightSchedule->setChecked(bright.timingSel);
	ui->bright0->setChecked(bright.timingCheck[0]);
	ui->bright0Begin->setTime(bright.startTime[0]);
	ui->bright0End->setTime(bright.endTime[0]);
	ui->bright0Value->setValue(bright.timingBright[0]);
	ui->bright1->setChecked(bright.timingCheck[1]);
	ui->bright1Begin->setTime(bright.startTime[1]);
	ui->bright1End->setTime(bright.endTime[1]);
	ui->bright1Value->setValue(bright.timingBright[1]);
	ui->bright2->setChecked(bright.timingCheck[2]);
	ui->bright2Begin->setTime(bright.startTime[2]);
	ui->bright2End->setTime(bright.endTime[2]);
	ui->bright2Value->setValue(bright.timingBright[2]);
	ui->bright3->setChecked(bright.timingCheck[3]);
	ui->bright3Begin->setTime(bright.startTime[3]);
	ui->bright3End->setTime(bright.endTime[3]);
	ui->bright3Value->setValue(bright.timingBright[3]);
	ui->brightDefaultValue->setValue(bright.defaultBright);
	ui->brightAuto->setChecked(bright.autoSel);

	ui->toUsb->setEnabled(false);
	connect(ui->toUsb, &QPushButton::clicked, this, &ExportDlg::toUsb);
	connect(ui->toDir, &QPushButton::clicked, this, &ExportDlg::toDir);
	connect(ui->close, &QPushButton::clicked, this, &ExportDlg::reject);
}



void ExportDlg::setUsbList()
{
	ui->usbList->clear();
	QList<QStorageInfo> drives = QStorageInfo::mountedVolumes();
	for (QStorageInfo& info: drives) {
		QString rootPath = info.rootPath();
		std::wstring letters = rootPath.toStdWString();
		if (!UtilWin::isUsbDrive(letters[0])) continue;
		rootPath.chop(1);
		//QString name = info.name();
		ui->usbList->addItem(rootPath/*.append(name)*/);
	}
	ui->toUsb->setEnabled(ui->usbList->count() > 0);
}

void ExportDlg::setWidgetStatus(bool b)
{
	ui->timingNow->setEnabled(ui->timing->isChecked());
	ui->timingFixed->setEnabled(ui->timing->isChecked());
	ui->timingValue->setEnabled(ui->timing->isChecked()
								&& ui->timingFixed->isChecked());

	ui->onTag->setEnabled(ui->onOff->isChecked());
	ui->onValue->setEnabled(ui->onOff->isChecked());
	ui->offTag->setEnabled(ui->onOff->isChecked());
	ui->offValue->setEnabled(ui->onOff->isChecked());

	ui->brightFixed->setEnabled(ui->bright->isChecked());
	ui->brightFixedValue->setEnabled(ui->bright->isChecked()
									 && ui->brightFixed->isChecked());
	ui->brightSchedule->setEnabled(ui->bright->isChecked());
	ui->beginTag->setEnabled(ui->bright->isChecked()
							 && ui->brightSchedule->isChecked());
	ui->endTag->setEnabled(ui->bright->isChecked()
						   && ui->brightSchedule->isChecked());
	ui->valueTag->setEnabled(ui->bright->isChecked()
							 && ui->brightSchedule->isChecked());
	ui->bright0->setEnabled(ui->bright->isChecked()
							&& ui->brightSchedule->isChecked());
	ui->bright0Begin->setEnabled(ui->bright->isChecked()
								 && ui->brightSchedule->isChecked()
								 && ui->bright0->isChecked());
	ui->bright0End->setEnabled(ui->bright->isChecked()
							   && ui->brightSchedule->isChecked()
							   && ui->bright0->isChecked());
	ui->bright0Value->setEnabled(ui->bright->isChecked()
								 && ui->brightSchedule->isChecked()
								 && ui->bright0->isChecked());
	ui->bright1->setEnabled(ui->bright->isChecked()
							&& ui->brightSchedule->isChecked());
	ui->bright1Begin->setEnabled(ui->bright->isChecked()
								 && ui->brightSchedule->isChecked()
								 && ui->bright1->isChecked());
	ui->bright1End->setEnabled(ui->bright->isChecked()
							   && ui->brightSchedule->isChecked()
							   && ui->bright1->isChecked());
	ui->bright1Value->setEnabled(ui->bright->isChecked()
								 && ui->brightSchedule->isChecked()
								 && ui->bright1->isChecked());
	ui->bright2->setEnabled(ui->bright->isChecked()
							&& ui->brightSchedule->isChecked());
	ui->bright2Begin->setEnabled(ui->bright->isChecked()
								 && ui->brightSchedule->isChecked()
								 && ui->bright2->isChecked());
	ui->bright2End->setEnabled(ui->bright->isChecked()
							   && ui->brightSchedule->isChecked()
							   && ui->bright2->isChecked());
	ui->bright2Value->setEnabled(ui->bright->isChecked()
								 && ui->brightSchedule->isChecked()
								 && ui->bright2->isChecked());
	ui->bright3->setEnabled(ui->bright->isChecked()
							&& ui->brightSchedule->isChecked());
	ui->bright3Begin->setEnabled(ui->bright->isChecked()
								 && ui->brightSchedule->isChecked()
								 && ui->bright3->isChecked());
	ui->bright3End->setEnabled(ui->bright->isChecked()
							   && ui->brightSchedule->isChecked()
							   && ui->bright3->isChecked());
	ui->bright3Value->setEnabled(ui->bright->isChecked()
								 && ui->brightSchedule->isChecked()
								 && ui->bright3->isChecked());
	ui->brightDefaultTag->setEnabled(ui->bright->isChecked()
									 && ui->brightSchedule->isChecked());
	ui->brightDefaultValue->setEnabled(ui->bright->isChecked()
									   && ui->brightSchedule->isChecked());
	ui->brightAuto->setEnabled(ui->bright->isChecked());
}

ExportDlg::~ExportDlg()
{
	delete ui;
}

void ExportDlg::toUsb()
{
	build(ui->usbList->currentText());
}

void ExportDlg::toDir()
{
	QString dirPath = QFileDialog::getExistingDirectory(this,
									tr("Export to directory"),
									QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
	if (dirPath.isEmpty())	return;
	QDir dir{dirPath};
	if (dir.count() >= 3 ) {
		if (QMessageBox::Yes != QMessageBox::question(this, tr("Overwrite"),
							 tr("The selected directory is not empty, existing files will be overwritten! Would you like to continue?")))
			return;
	}
	build(dirPath);
}

void ExportDlg::build(QString dir)
{
	QDir().mkpath(dir);

	if (ui->timing->isChecked()) {
		sync.systemSel = ui->timingNow->isChecked();
		sync.manualSel = ui->timingFixed->isChecked();
		sync.date = ui->timingValue->date();
		sync.time = ui->timingValue->time();
	}
	if (onOff.timerSwitch = ui->onOff->isChecked())
	{
		onOff.openTime = ui->onValue->time();
		onOff.closeTime = ui->offValue->time();
	}
	if (ui->bright->isChecked()) {
		if (bright.manualSel = ui->brightFixed->isChecked())
			bright.manualBright = ui->brightFixedValue->value();
		if (bright.timingSel = ui->brightSchedule->isChecked()) {
			if (bright.timingCheck[0] = ui->bright0->isChecked()) {
				bright.startTime[0] = ui->bright0Begin->time();
				bright.endTime[0] = ui->bright0End->time();
				bright.timingBright[0] = ui->bright0Value->value();
			}
			if (bright.timingCheck[1] = ui->bright1->isChecked()) {
				bright.startTime[1] = ui->bright1Begin->time();
				bright.endTime[1] = ui->bright1End->time();
				bright.timingBright[1] = ui->bright1Value->value();
			}
			if (bright.timingCheck[2] = ui->bright2->isChecked()) {
				bright.startTime[2] = ui->bright2Begin->time();
				bright.endTime[2] = ui->bright2End->time();
				bright.timingBright[2] = ui->bright2Value->value();
			}
			if (bright.timingCheck[3] = ui->bright3->isChecked()) {
				bright.startTime[3] = ui->bright3Begin->time();
				bright.endTime[3] = ui->bright3End->time();
				bright.timingBright[3] = ui->bright3Value->value();
			}
			bright.defaultBright = ui->brightDefaultValue->value();
			bright.autoSel = ui->brightAuto->isChecked();
		}
	}

	QString ret = screen->buildFiles(dir, true);
	if (!ret.isEmpty()) {
		QMessageBox::critical(this, tr("Error"), ret);
		return;
	}

	/*if (QMessageBox::Yes == QMessageBox::question(this,
											tr("Success"),
											tr("Export succeed. Open it?")))
		QDesktopServices::openUrl(QUrl::fromLocalFile(dir));*/
	QMessageBox::information(this, tr("Success"), tr("Export succeed."));
}
