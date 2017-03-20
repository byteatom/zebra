#include <QString>
#include <QModelIndex>
#include <QInputDialog>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMouseEvent>
#include "ModuleWizard.h"
#include "ui_ModuleWizard.h"
#include "Message.h"

int dotyx[32][64];

extern const QHash<QString, enum IScreen::BaseColor> baseColorJsonHash;
extern const QHash<QString, enum IScreen::WarpMode> warpModeJsonHash;
extern const QHash<QString, enum IScreen::DecodeMode> decodeModeJsonHash;
extern const QHash<int, int> scanLinesViewHash;
extern const QHash<int, int> moduleLinesViewHash;
extern const QHash<int, int> warpBitsViewHash;
extern const QString moduleType;
extern const QString moduleHeader;
extern QString usrModulePath;

ModuleWizard::ModuleWizard(const ISetting &setting_,
							 IScreen::Module &ledModule_,
							 IProtocol *protocol_,
							 bool newModule_,
							 QWidget *parent) :
	setting(setting_),
	ledModule(ledModule_),
	protocol(protocol_),
	newModule(newModule_),
	QWizard(parent),
	ui(new Ui::ModuleWizard)
{
	ui->setupUi(this);
	//setWindowFlags(Qt::Dialog | Qt::WindowMinMaxButtonsHint);

	isSave = false;
	leftButtonPress = false;
	for (int i = 0; i < 32*64; i++)
		dotyx[i/64][i%64] = -1;

	ui->tableWidget_pageDotCoord->viewport()->installEventFilter(this);
	//ui->tableWidget_pageDotCoord->installEventFilter(this);
	ui->tableWidget_pageDotCoord->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#d2fff0;}");
	ui->tableWidget_pageDotCoord->verticalHeader()->setStyleSheet("QHeaderView::section{background:#d2fff0;}");
	ui->pushButton_pageCompleteTest->hide();

	setButtonText(QWizard::NextButton, tr("Next->"));
	setButtonText(QWizard::BackButton, tr("<-Back"));
	setButtonText(QWizard::CancelButton, tr("Cancel"));
	setButtonText(QWizard::FinishButton, tr("Finish"));

	connect(this, &QWizard::currentIdChanged, this, &ModuleWizard::scanDotWizardCurrentIdChanged);
}

ModuleWizard::~ModuleWizard()
{
	delete ui;
}

void ModuleWizard::showEmptyDotConut(int count)
{
	ui->label_pageDotCoordEmptyConut->setText(tr("Blank dot:") + QString::number(count));
}

void ModuleWizard::showPageCompleteHint(const QString &str, Qt::GlobalColor color)
{
	ui->label_pageCompleteHint->setText(str);
	QPalette pe;
	pe.setColor(QPalette::WindowText, color);
	ui->label_pageCompleteHint->setPalette(pe);
}

void ModuleWizard::saveScanDataFile(void)
{
	QString tmpModel;

	bool clickOk;

	if (newModule) {
		tmpModel = QInputDialog::getText(this, tr("Get Name"), tr("New LED module name:"),
										 QLineEdit::Normal, tr("new"), &clickOk);
	} else {
		tmpModel = QInputDialog::getText(this, tr("Get Name"), tr("LED module name:"),
										 QLineEdit::Normal, ledModule.model, &clickOk);
	}
	if (!clickOk || tmpModel.isEmpty()) return;

	if (tmpModel.left(moduleHeader.length()).compare(moduleHeader) != 0)
		tmpModel = moduleHeader + tmpModel;

	QFile file(usrModulePath + "/" + tmpModel + "." + moduleType);
	if (file.exists()) {
		int ret = QMessageBox::information(this, tr("Warning"), tr("Previous led module will be covered by new"),
										   QMessageBox::Yes, QMessageBox::No);
		if (ret == QMessageBox::No)
			return;
	}

	if (!newModule)
		QFile::remove(usrModulePath + "/" + ledModule.model + "." + moduleType);

	ledModule.model	= tmpModel;

	file.open(QIODevice::WriteOnly);
	QJsonObject jsonLedModule;
	QJsonObject json;

	if(ledModule.scanLines == 0 || ledModule.scanLines > 32)
		ledModule.scanLines = 4;
	if(ledModule.moduleLines == 0 || ledModule.moduleLines > 32)
		ledModule.moduleLines = 16;
	if(ledModule.warpBits == 0 || ledModule.warpBits > 64)
		ledModule.warpBits = 8;

	ledModule.scanDot = true;
	if (newModule) ledModule.change = true;
	json.insert("model", ledModule.model);
	json.insert("change", ledModule.change);
	json.insert("baseColor", baseColorJsonHash.key((IScreen::BaseColor)ledModule.baseColor.get()));
	json.insert("moduleLines", ledModule.moduleLines);
	json.insert("scanLines", ledModule.scanLines);
	json.insert("warpBits", ledModule.warpBits);
	json.insert("warpTimes", ledModule.warpTimes);
	json.insert("warpMode", warpModeJsonHash.key((IScreen::WarpMode)ledModule.warpMode));
	json.insert("decodeMode", decodeModeJsonHash.key((IScreen::DecodeMode)ledModule.decodeMode));
	json.insert("dataPolarity", ledModule.dataPolarity);
	json.insert("strPolarity", ledModule.strPolarity);
	json.insert("oePolarity", ledModule.oePolarity);
	json.insert("mirror", ledModule.mirror);
	json.insert("split", ledModule.split);
	json.insert("warpTurn", ledModule.warpTurn);
	json.insert("scanDot", ledModule.scanDot);
	json.insert("moduleWidth", ledModule.moduleWidth);
	json.insert("moduleHeight", ledModule.moduleHeight);
	json.insert("emptyDot", ledModule.emptyDot);
	json.insert("dotIndex", ledModule.dotIndex);
	json.insert("emptyDotIndex", ledModule.emptyDotIndex);
	json.insert("color1", ledModule.color1);
	json.insert("color2", ledModule.color2);
	json.insert("color3", ledModule.color3);
	json.insert("color4", ledModule.color4);
	json.insert("expandRGB", ledModule.expandRGB);
	json.insert("complete", ledModule.complete);
	QJsonArray arrayDotMap;
	for (int i = 0; i < ledModule.dotIndex/ledModule.scanLines; i++) {
		QJsonObject obj;
		obj.insert("index", i);
		obj.insert("empty", ledModule.dotMap[i].empty);
		obj.insert("x", ledModule.dotMap[i].x);
		obj.insert("y", ledModule.dotMap[i].y);
		arrayDotMap.insert(i, obj);
	}
	json.insert("dotMap", arrayDotMap);

	QJsonArray arrayLineMap;
	for (int i = 0; i < ledModule.scanLines; i++) {
		arrayLineMap.insert(i, ledModule.lineMap[i]);
	}
	json.insert("lineMap", arrayLineMap);

	jsonLedModule.insert("ledModule", json);
	QJsonDocument document;
	document.setObject(jsonLedModule);
	QString jsonString(document.toJson(QJsonDocument::Indented));
	file.seek(0);
	file.write(jsonString.toUtf8().data());
	file.close();
}

void ModuleWizard::clearDotCoordData()
{
	ledModule.dotIndex = 0;
	ledModule.emptyDotIndex = 0;
	showEmptyDotConut(ledModule.emptyDotIndex);
	for (int i = 0; i < 2048; i++) {
		ledModule.dotMap[i].x = -1;
		ledModule.dotMap[i].y = -1;
		ledModule.dotMap[i].empty = false;
	}
	for (int i = 0; i < 32*64; i++) {
		dotyx[i/64][i%64] = -1;
	}
}

void ModuleWizard::initScanData()
{
	ledModule.moduleWidth = 32;
	//ledModule.moduleHeight = 16;
	ledModule.baseColor = 1;
	ledModule.moduleLines = 16;
	ledModule.scanLines = 4;
	ledModule.warpTimes = 4;
	ledModule.decodeMode = 0;
	ledModule.emptyDot = 0;
	ledModule.dotIndex = 0;
	ledModule.emptyDotIndex = 0;
	ledModule.expandRGB = false;
	ledModule.dataPolarity = true;
	ledModule.oePolarity = true;
	ledModule.color1 = 0;
	ledModule.color2 = 1;
	ledModule.color3 = 2;
	ledModule.color4 = 3;
	ledModule.complete = false;
}

void ModuleWizard::setScanDataToScanDotWizard()
{
	ui->comboBox_pageBaseParaBaseColor->setCurrentIndex(ledModule.baseColor);
	ui->spinBox_pageBaseParaWidth->setValue(ledModule.moduleWidth);
	ui->spinBox_pageBaseParaHeight->setValue(ledModule.moduleHeight);
	ui->comboBox_pageBaseParaDecodeMode->setCurrentIndex(ledModule.decodeMode);
	ui->checkBox_pageBaseParaExpandInterface->setCheckState(ledModule.expandRGB ? Qt::Checked : Qt::Unchecked);
	ui->spinBox_pageBaseParaEmptyDot->setValue(ledModule.emptyDot);
	ui->comboBox_pageDataPolarSelect->setCurrentIndex(ledModule.dataPolarity ? 0 : 1);
	ui->comboBox_pageOePolarSelect->setCurrentIndex(ledModule.oePolarity ? 0 : 1);
	ui->spinBox_pageInterfaceLinesSet->setValue(ledModule.moduleLines);
	ui->spinBox_pageWarpTimesSet->setValue(ledModule.warpTimes);
	ui->tableWidget_pageDotCoord->setColumnCount(ledModule.moduleWidth);
	ui->tableWidget_pageDotCoord->setRowCount(ledModule.moduleLines);

	for (int i = 0; i < ledModule.dotIndex; i++) {
		if (ledModule.dotMap[i].empty == false)
			ui->tableWidget_pageDotCoord->setItem(ledModule.dotMap[i].y, ledModule.dotMap[i].x, new QTableWidgetItem(QString::number(i + 1)));
	}
}

void ModuleWizard::dotCoordCellSelected(int row, int column)
{
	if (ledModule.dotIndex >= std::extent<decltype(ledModule.dotMap)>::value)
		return;

	if (dotyx[row][column] == -1) {
		int scanLen = (ledModule.moduleWidth + ledModule.emptyDot) * ledModule.warpTimes;
		int x, y;
		if ((ledModule.dotIndex >= 0) && (ledModule.dotIndex < scanLen)) {
			ledModule.lineMap[0] = 0;
			x = ledModule.dotMap[ledModule.dotIndex].x = column;
			y = ledModule.dotMap[ledModule.dotIndex].y = row;
			dotyx[y][x] = ledModule.dotIndex;
			ui->tableWidget_pageDotCoord->setItem(y, x, new QTableWidgetItem(QString::number(ledModule.dotIndex + 1)));
			ledModule.dotIndex++;

			if (ledModule.dotIndex == scanLen) {
				scanDotSend.type = 0x07;
				scanDotSend.dotIndex = 256 - scanLen + ledModule.dotIndex%scanLen;
				scanDotSend.currentLine++;
				sendScanDotSend(&scanDotSend);
			} else {
				scanDotSend.type = 0x07;
				scanDotSend.dotIndex = 256 - scanLen + ledModule.dotIndex%scanLen;
				scanDotSend.currentLine = 0;
				sendScanDotSend(&scanDotSend);
			}
		} else if (ledModule.dotIndex >= scanLen) {
			int dot0Y = 0;
			for (int i = 0; i < scanLen; ++i)
				if (ledModule.dotMap[i].empty == false) {
					dot0Y = ledModule.dotMap[i].y; break;
				}
			ledModule.lineMap[ledModule.dotIndex/scanLen] = row - dot0Y;
			for (int i = 0; i < scanLen; ++i) {
				if (ledModule.dotMap[ledModule.dotIndex - scanLen].empty == true) {
					ledModule.dotMap[ledModule.dotIndex].empty = true;
					ledModule.dotIndex++;
				} else {
					x = ledModule.dotMap[ledModule.dotIndex].x
					  = ledModule.dotMap[ledModule.dotIndex - scanLen].x;
					y = ledModule.dotMap[ledModule.dotIndex].y
					  = ledModule.dotMap[i].y + (row - dot0Y);
					ledModule.dotMap[ledModule.dotIndex].empty = false;
					dotyx[y][x] = ledModule.dotIndex;
					ui->tableWidget_pageDotCoord->setItem(y, x, new QTableWidgetItem(QString::number(ledModule.dotIndex + 1)));
					ledModule.dotIndex++;
				}
			}
			scanDotSend.type = 0x07;
			scanDotSend.dotIndex = 256 - scanLen + ledModule.dotIndex%scanLen;
			scanDotSend.currentLine++;
			sendScanDotSend(&scanDotSend);
		}
	}
}

void ModuleWizard::sendScanDotSend(ScanDotSend *send)
{
	if (!protocol) return;

	IMsgSp req(new ScanDotReqMsg((const char *)send, sizeof(ScanDotSend)));
	BoolRecver recver = [](bool ok){ (void)ok; };
	protocol->send(req, recver, 0, 0);
}

void ModuleWizard::scanDotWizardCurrentIdChanged(int id)
{
	int preid = id - 1;

	if (preid == -1)
	{
		memset(&scanDotSend, 0, sizeof(scanDotSend));
		if (newModule == true) {
			clearDotCoordData();
			ui->tableWidget_pageDotCoord->clearContents();
		} else {
			ui->tableWidget_pageDotCoord->clearContents();
			setScanDataToScanDotWizard();
			showEmptyDotConut(ledModule.emptyDotIndex);
		}
	}
	else if (preid == ScanDotWizardPageStart)
	{
	}
	else if (preid == ScanDotWizardPageBasePara)
	{
		if (ui->spinBox_pageBaseParaWidth->value() != ledModule.moduleWidth) {
			ledModule.moduleWidth = ui->spinBox_pageBaseParaWidth->value();
			ledModule.moduleHeight = ui->spinBox_pageBaseParaHeight->value();
			clearDotCoordData();
			ui->tableWidget_pageDotCoord->clearContents();
			ui->tableWidget_pageDotCoord->setColumnCount(ledModule.moduleWidth);
		} else {
			ledModule.moduleWidth = ui->spinBox_pageBaseParaWidth->value();
			ledModule.moduleHeight = ui->spinBox_pageBaseParaHeight->value();
			ui->tableWidget_pageDotCoord->setColumnCount(ledModule.moduleWidth);
		}
		ledModule.baseColor = ui->comboBox_pageBaseParaBaseColor->currentIndex();
		ledModule.decodeMode = ui->comboBox_pageBaseParaDecodeMode->currentIndex();
		ledModule.expandRGB = ui->checkBox_pageBaseParaExpandInterface->isChecked();
		ledModule.emptyDot = ui->spinBox_pageBaseParaEmptyDot->value();
	}
	else if (preid == ScanDotWizardPageDataPolar)
	{
		if (ui->comboBox_pageDataPolarSelect->currentIndex() == 0)
			ledModule.dataPolarity = true;
		else
			ledModule.dataPolarity = false;
	}
	else if (preid == ScanDotWizardPageOePolar)
	{
		if (ui->comboBox_pageOePolarSelect->currentIndex() == 0)
			ledModule.oePolarity = true;
		else
			ledModule.oePolarity = false;
	}
	else if (preid == ScanDotWizardPageModuleLines)
	{
		if (ui->spinBox_pageInterfaceLinesSet->value() != ledModule.moduleLines) {
			ledModule.moduleLines = ui->spinBox_pageInterfaceLinesSet->value();
			clearDotCoordData();
			ui->tableWidget_pageDotCoord->clearContents();
			ui->tableWidget_pageDotCoord->setRowCount(ledModule.moduleLines);
		} else {
			ledModule.moduleLines = ui->spinBox_pageInterfaceLinesSet->value();
			ui->tableWidget_pageDotCoord->setRowCount(ledModule.moduleLines);
		}
	}
	else if (preid == ScanDotWizardPageWarpTimes)
	{
		if (ui->spinBox_pageWarpTimesSet->value() != ledModule.warpTimes) {
			ledModule.warpTimes = ui->spinBox_pageWarpTimesSet->value();
			ledModule.scanLines = ledModule.moduleLines / ledModule.warpTimes;
			clearDotCoordData();
		} else {
			ledModule.warpTimes = ui->spinBox_pageWarpTimesSet->value();
			ledModule.scanLines = ledModule.moduleLines / ledModule.warpTimes;
		}
	}
	else if (preid == ScanDotWizardPageDotCoord)
	{
		isSave = false;
		if (ledModule.dotIndex == (ledModule.moduleWidth + ledModule.emptyDot) * ledModule.moduleLines) {
			ledModule.complete = true;
			showPageCompleteHint(tr("LED module scandot successed, please save and rename module"), Qt::green);
		} else {
			ledModule.complete = false;
			showPageCompleteHint(tr("LED module scandot not completed"), Qt::red);
		}
	}

	if (id == ScanDotWizardPageStart)
	{
		memset(&scanDotSend, 0, sizeof(scanDotSend));
	}
	else if (id == ScanDotWizardPageBasePara)
	{
		scanDotSend.display = 0xA5;
		scanDotSend.type = 0x02;
		scanDotSend.baseColor = ui->comboBox_pageBaseParaBaseColor->currentIndex() + 1;
		if (ui->comboBox_pageBaseParaDecodeMode->currentIndex() == 0)
			scanDotSend.bitMask &= ~CTRLINFO_DECODEMODE;
		else
			scanDotSend.bitMask |= CTRLINFO_DECODEMODE;
		sendScanDotSend(&scanDotSend);
	}
	else if (id == ScanDotWizardPageDataPolar)
	{
		if (ui->comboBox_pageDataPolarSelect->currentIndex() == 0)
			scanDotSend.bitMask &= ~CTRLINFO_DATAPOLARITY;
		else
			scanDotSend.bitMask |= CTRLINFO_DATAPOLARITY;

		if (ui->radioButton_pageDataPolarTestAllBright->isChecked())
			scanDotSend.type = 0x02;
		else
			scanDotSend.type = 0x03;

		sendScanDotSend(&scanDotSend);
	}
	else if (id == ScanDotWizardPageOePolar)
	{
		if (ui->comboBox_pageOePolarSelect->currentIndex() == 0)
			scanDotSend.bitMask &= ~CTRLINFO_OEPOLARITY;
		else
			scanDotSend.bitMask |= CTRLINFO_OEPOLARITY;

		if (ui->radioButton_pageOePolarBright->isChecked())
			scanDotSend.type = 0x04;
		else
			scanDotSend.type = 0x05;

		sendScanDotSend(&scanDotSend);
	}
	else if (id == ScanDotWizardPageModuleLines)
	{
		scanDotSend.type = 0x00;
		sendScanDotSend(&scanDotSend);
	}
	else if (id == ScanDotWizardPageWarpTimes)
	{
		scanDotSend.type = 0x01;
		sendScanDotSend(&scanDotSend);
	}
	else if (id == ScanDotWizardPageDotCoord)
	{
		int scanLen = (ledModule.moduleWidth + ledModule.emptyDot) * ledModule.warpTimes;
		scanDotSend.type = 0x07;
		scanDotSend.dotIndex = 256 - scanLen + ledModule.dotIndex%scanLen;
		scanDotSend.currentLine = ledModule.dotIndex/scanLen;
		sendScanDotSend(&scanDotSend);
	}
}

void ModuleWizard::on_pushButton_pageDotCoordReset_clicked()
{
	int scanLen = (ledModule.moduleWidth + ledModule.emptyDot) * ledModule.warpTimes;
	clearDotCoordData();
	for (int y = 0; y < ui->spinBox_pageInterfaceLinesSet->value(); y++)
		for (int x = 0; x < ui->spinBox_pageBaseParaWidth->value(); x++)
			ui->tableWidget_pageDotCoord->setItem(y, x, new QTableWidgetItem(""));

	ledModule.dotIndex = 0;
	ledModule.emptyDotIndex = 0;
	scanDotSend.type = 0x07;
	scanDotSend.dotIndex = 256 - scanLen + ledModule.dotIndex%scanLen;
	scanDotSend.currentLine = 0;
	sendScanDotSend(&scanDotSend);
}

void ModuleWizard::on_pushButton_pageDotCoordCancel_clicked()
{
	int scanLen = (ledModule.moduleWidth + ledModule.emptyDot) * ledModule.warpTimes;
	int x, y;

	if (ledModule.dotIndex > 0 && ledModule.dotIndex <= scanLen) {
		if (ledModule.dotMap[ledModule.dotIndex-1].empty == true) {
			if (ledModule.emptyDotIndex > 0) {
				ledModule.dotMap[ledModule.dotIndex-1].empty = false;
				ledModule.dotIndex--;
				ledModule.emptyDotIndex--;
				showEmptyDotConut(ledModule.emptyDotIndex);
			}
		} else {
			x = ledModule.dotMap[ledModule.dotIndex-1].x;
			y = ledModule.dotMap[ledModule.dotIndex-1].y;
			ledModule.dotMap[ledModule.dotIndex-1].x = -1;
			ledModule.dotMap[ledModule.dotIndex-1].y = -1;
			dotyx[y][x] = -1;
			ui->tableWidget_pageDotCoord->setItem(y, x, new QTableWidgetItem(""));
			ledModule.dotIndex--;
		}

		scanDotSend.type = 0x07;
		scanDotSend.dotIndex = 256 - scanLen + ledModule.dotIndex%scanLen;
		scanDotSend.currentLine = 0;
		sendScanDotSend(&scanDotSend);
	} else if (ledModule.dotIndex > scanLen) {
		for (int i = 0; i < scanLen; i++) {
			if (ledModule.dotMap[ledModule.dotIndex-1].empty == true) {
				ledModule.dotMap[ledModule.dotIndex-1].empty = false;
				ledModule.dotIndex--;
			} else {
				x = ledModule.dotMap[ledModule.dotIndex-1].x;
				y = ledModule.dotMap[ledModule.dotIndex-1].y;
				ledModule.dotMap[ledModule.dotIndex-1].x = -1;
				ledModule.dotMap[ledModule.dotIndex-1].y = -1;
				dotyx[y][x] = -1;
				ui->tableWidget_pageDotCoord->setItem(y, x, new QTableWidgetItem(""));
				ledModule.dotIndex--;
			}
		}

		if (ledModule.dotIndex == scanLen) {
			scanDotSend.type = 0x07;
			scanDotSend.dotIndex = 256 - scanLen + ledModule.dotIndex%scanLen;
			scanDotSend.currentLine = 1;
			sendScanDotSend(&scanDotSend);
		} else {
			scanDotSend.type = 0x07;
			scanDotSend.dotIndex = 256 - scanLen + ledModule.dotIndex%scanLen;
			scanDotSend.currentLine--;
			sendScanDotSend(&scanDotSend);
		}
	}
}

void ModuleWizard::on_pushButton_pageDotCoordZoom_clicked()
{
	if (isMaximized()) {
		showNormal();
		ui->pushButton_pageDotCoordZoom->setText(tr("Max Window"));
	} else {
		showMaximized();
		ui->pushButton_pageDotCoordZoom->setText(tr("Restore"));
	}
}

void ModuleWizard::on_checkBox_pageBaseParaEnableEmptyDot_toggled(bool checked)
{
	if (checked == true)
		ui->spinBox_pageBaseParaEmptyDot->setReadOnly(false);
	else {
		ui->spinBox_pageBaseParaEmptyDot->setReadOnly(true);
		ui->spinBox_pageBaseParaEmptyDot->setValue(0);
	}
}

void ModuleWizard::on_tableWidget_pageDotCoord_cellPressed(int row, int column)
{
	dotCoordCellSelected(row, column);
}

void ModuleWizard::on_tableWidget_pageDotCoord_cellEntered(int row, int column)
{
	if (leftButtonPress) dotCoordCellSelected(row, column);
}

void ModuleWizard::on_pushButton_pageDotCoordEmpty_clicked()
{
	int scanLen = (ledModule.moduleWidth + ledModule.emptyDot) * ledModule.warpTimes;

	if (ledModule.dotIndex >= 0 && ledModule.dotIndex <= scanLen
	  && ledModule.emptyDotIndex < ledModule.emptyDot*ledModule.warpTimes) {
		ledModule.dotMap[ledModule.dotIndex].empty = true;
		ledModule.dotIndex++;
		ledModule.emptyDotIndex++;
		showEmptyDotConut(ledModule.emptyDotIndex);

		scanDotSend.type = 0x07;
		scanDotSend.dotIndex = 256 - scanLen + ledModule.dotIndex%scanLen;
		scanDotSend.currentLine = 0;
		sendScanDotSend(&scanDotSend);
	}
}

void ModuleWizard::on_ScanDotWizard_finished(int result)
{
	scanDotSend.display = 0x00;
	scanDotSend.type = 0x00;
	sendScanDotSend(&scanDotSend);

	if (result) {
		if (ledModule.complete == true && isSave == false)
			saveScanDataFile();
	}
}

void ModuleWizard::on_pushButton_pageCompleteSava_clicked()
{
	if (ledModule.complete == false) {
		QMessageBox::information(this, tr("Warning"), tr("Scan Dot not completed"), QMessageBox::Close);
	} else if (ledModule.complete == true && isSave == false) {
		saveScanDataFile();
		isSave = true;
	}
}

void ModuleWizard::on_comboBox_pageBaseParaDecodeMode_currentIndexChanged(int index)
{
	if (index == 0)
		scanDotSend.bitMask &= ~CTRLINFO_DECODEMODE;
	else
		scanDotSend.bitMask |= CTRLINFO_DECODEMODE;
	sendScanDotSend(&scanDotSend);
}

void ModuleWizard::on_comboBox_pageBaseParaBaseColor_currentIndexChanged(int index)
{
	scanDotSend.baseColor = index + 1;
	sendScanDotSend(&scanDotSend);
}

void ModuleWizard::on_comboBox_pageDataPolarSelect_currentIndexChanged(int index)
{
	if (index == 0)
		scanDotSend.bitMask &= ~CTRLINFO_DATAPOLARITY;
	else
		scanDotSend.bitMask |= CTRLINFO_DATAPOLARITY;

	if (ui->radioButton_pageDataPolarTestAllBright->isChecked())
		scanDotSend.type = 0x02;
	else
		scanDotSend.type = 0x03;

	sendScanDotSend(&scanDotSend);
}

void ModuleWizard::on_radioButton_pageDataPolarTestAllBright_toggled(bool checked)
{
	if (ui->comboBox_pageDataPolarSelect->currentIndex() == 0)
		scanDotSend.bitMask &= ~CTRLINFO_DATAPOLARITY;
	else
		scanDotSend.bitMask |= CTRLINFO_DATAPOLARITY;

	if (checked)
		scanDotSend.type = 0x02;
	else
		scanDotSend.type = 0x03;

	sendScanDotSend(&scanDotSend);
}

void ModuleWizard::on_comboBox_pageOePolarSelect_currentIndexChanged(int index)
{
	if (index == 0)
		scanDotSend.bitMask &= ~CTRLINFO_OEPOLARITY;
	else
		scanDotSend.bitMask |= CTRLINFO_OEPOLARITY;

	if (ui->radioButton_pageOePolarBright->isChecked())
		scanDotSend.type = 0x04;
	else
		scanDotSend.type = 0x05;

	sendScanDotSend(&scanDotSend);
}

void ModuleWizard::on_radioButton_pageOePolarBright_toggled(bool checked)
{
	if (ui->comboBox_pageOePolarSelect->currentIndex() == 0)
		scanDotSend.bitMask &= ~CTRLINFO_OEPOLARITY;
	else
		scanDotSend.bitMask |= CTRLINFO_OEPOLARITY;

	if (checked)
		scanDotSend.type = 0x04;
	else
		scanDotSend.type = 0x05;

	sendScanDotSend(&scanDotSend);
}

bool ModuleWizard::eventFilter(QObject *obj, QEvent *event)
{
	if (obj != ui->tableWidget_pageDotCoord->viewport())// && obj != ui->tableWidget_pageDotCoord)
		return true;

	QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

	if (event->type() == QEvent::MouseButtonPress) {
		if (mouseEvent->buttons() == Qt::LeftButton) {
			leftButtonPress = true;
			return false;
		} else
			return true;
	} else if (event->type() == QEvent::MouseButtonRelease) {
		leftButtonPress = false;
		return false;
	} else if (event->type() == QEvent::MouseButtonDblClick) {
		return true;
	}

	return false;
}

void ModuleWizard::on_tableWidget_pageDotCoord_itemSelectionChanged()
{
	dotCoordCellSelected(ui->tableWidget_pageDotCoord->currentRow(),
						 ui->tableWidget_pageDotCoord->currentColumn());
}
