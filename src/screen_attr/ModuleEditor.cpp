#include "ModuleEditor.h"
#include "ui_ModuleEditor.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QInputDialog>
#include <QMessageBox>

#include "ModuleWizard.h"

extern const QHash<QString, enum IScreen::BaseColor> baseColorJsonHash;
extern const QHash<QString, enum IScreen::WarpMode> warpModeJsonHash;
extern const QHash<QString, enum IScreen::DecodeMode> decodeModeJsonHash;
extern const QHash<int, int> scanLinesViewHash;
extern const QHash<int, int> moduleLinesViewHash;
extern const QHash<int, int> warpBitsViewHash;
extern const QString moduleType;
extern const QString moduleHeader;
extern QString usrModulePath;

ModuleEditor::ModuleEditor(const ISetting &setting_,
						   IScreen::Module &ledModule_,
						   IProtocol *protocol_,
						   bool newModule_,
						   QWidget *parent) :
	QDialog(parent),
	setting(setting_),
	ledModule(ledModule_),
	protocol(protocol_),
	newModule(newModule_),
	ui(new Ui::ModuleEditor)
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	hideCnt = 0;

	ui->stackedWidget->setCurrentIndex(0);

	connect(ui->pushButtonAttrSet, &QPushButton::clicked, this, &ModuleEditor::dispAttrSet);
	connect(ui->pushButtonScanDot, &QPushButton::clicked, this, &ModuleEditor::dispScanDot);
	connect(ui->pushButtonAttrSetCancel, &QPushButton::clicked, this, &QDialog::close);
	connect(ui->pushButtonAttrSetOK, &QPushButton::clicked, this, &ModuleEditor::saveAttrSet);
	connect(ui->pushButtonAttrSetUnlock, &QPushButton::clicked, this, &ModuleEditor::unlockAttrSet);
}

ModuleEditor::~ModuleEditor()
{
	delete ui;
}

void ModuleEditor::dispAttrSet(void)
{
	if (!newModule) {
		setEnabledAttrSet(ledModule.change);
		ui->lineEditModuleName->setText(ledModule.model);
		ui->comboBoxScanLines->setCurrentIndex(scanLinesViewHash.key(ledModule.scanLines));
		ui->comboBoxModuleLines->setCurrentIndex(moduleLinesViewHash.key(ledModule.moduleLines));
		ui->comboBoxWarpMode->setCurrentIndex(static_cast<int>(ledModule.warpMode));
		ui->comboBoxWarpBits->setCurrentIndex(warpBitsViewHash.key(ledModule.warpBits));
		ui->comboBoxOePolarity->setCurrentIndex(ledModule.oePolarity ? 1 : 0);
		ui->comboBoxStrPolarity->setCurrentIndex(ledModule.strPolarity ? 1 : 0);
		ui->comboBoxDataPolarity->setCurrentIndex(ledModule.dataPolarity ? 1 : 0);
		ui->comboBoxDecodeMode->setCurrentIndex(static_cast<int>(ledModule.decodeMode));
		ui->comboBoxBaseColor->setCurrentIndex(static_cast<int>(ledModule.baseColor));
		ui->comboBoxSplit->setCurrentIndex(ledModule.split ? 1 : 0);
		ui->comboBoxMirror->setCurrentIndex(ledModule.mirror ? 1 : 0);
		ui->comboBoxWarpTurn->setCurrentIndex(ledModule.warpTurn ? 1 : 0);
	}

	ui->stackedWidget->setCurrentIndex(1);
}

void ModuleEditor::dispScanDot(void)
{
	if (newModule == true || (newModule == false && ledModule.change == true)) {
		ModuleWizard scanDotWizard(setting, ledModule, protocol, newModule, this->parentWidget());
		scanDotWizard.exec();
		close();
	} else {
		QMessageBox::information(this, tr("Warning"), tr("Can not edit default LED module"), QMessageBox::Ok);
	}
}

void ModuleEditor::saveAttrSet(void)
{
	QString tmpModel;

	if (ui->lineEditModuleName->text().isEmpty()) {
		bool clickOk;
		tmpModel = QInputDialog::getText(this, tr("Get Name"), tr("New LED module name:"), QLineEdit::Normal, tr("new"), &clickOk);
		if (!clickOk || tmpModel.isEmpty()) return;
	} else
		tmpModel = ui->lineEditModuleName->text();

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
	if (newModule) {
		ledModule.change = true;
		ledModule.dotIndex = 0;
	}
	ledModule.scanLines = scanLinesViewHash.value(ui->comboBoxScanLines->currentIndex());
	ledModule.moduleLines = moduleLinesViewHash.value(ui->comboBoxModuleLines->currentIndex());
	ledModule.warpTimes = ledModule.moduleLines / ledModule.scanLines;
	ledModule.warpMode = static_cast<IScreen::WarpMode>(ui->comboBoxWarpMode->currentIndex());
	ledModule.warpBits = warpBitsViewHash.value(ui->comboBoxWarpBits->currentIndex());
	ledModule.oePolarity = ui->comboBoxOePolarity->currentIndex() == 0 ? false : true;
	ledModule.strPolarity = ui->comboBoxStrPolarity->currentIndex() == 0 ? false : true;
	ledModule.dataPolarity = ui->comboBoxDataPolarity->currentIndex() == 0 ? false : true;
	ledModule.decodeMode = static_cast<IScreen::DecodeMode>(ui->comboBoxDecodeMode->currentIndex());
	ledModule.baseColor = static_cast<IScreen::BaseColor>(ui->comboBoxBaseColor->currentIndex());
	ledModule.split = ui->comboBoxSplit->currentIndex() == 0 ? false : true;
	ledModule.mirror = ui->comboBoxMirror->currentIndex() == 0 ? false : true;
	ledModule.warpTurn = ui->comboBoxWarpTurn->currentIndex() == 0 ? false : true;

	file.open(QIODevice::WriteOnly);
	QJsonObject jsonLedModule;
	QJsonObject json;

	if(ledModule.scanLines == 0 || ledModule.scanLines > 32)
		ledModule.scanLines = 4;
	if(ledModule.moduleLines == 0 || ledModule.moduleLines > 32)
		ledModule.moduleLines = 16;
	if(ledModule.warpBits == 0 || ledModule.warpBits > 64)
		ledModule.warpBits = 8;

	if (ledModule.dotIndex > 0)
		ledModule.scanDot = true;
	else
		ledModule.scanDot = false;

	json.insert("model", ledModule.model);
	json.insert("change", ledModule.change);
	json.insert("baseColor", baseColorJsonHash.key((IScreen::BaseColor)ledModule.baseColor.get()));
	json.insert("moduleLines", ledModule.moduleLines);
	json.insert("scanLines", ledModule.scanLines);
	json.insert("warpTimes", ledModule.warpTimes);
	json.insert("warpBits", ledModule.warpBits);
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
	if (ledModule.dotIndex > 0) {
		QJsonArray arrayDotMap;
		for (int i = 0; i < ledModule.dotIndex; i++) {
			QJsonObject obj;
			obj.insert("x", ledModule.dotMap[i].x);
			obj.insert("y", ledModule.dotMap[i].y);
			obj.insert("empty", ledModule.dotMap[i].empty);
			arrayDotMap.insert(i, obj);
		}
		json.insert("dotMap", arrayDotMap);

		QJsonArray arrayLineMap;
		for (int i = 0; i < ledModule.scanLines; i++) {
			arrayLineMap.insert(i, ledModule.lineMap[i]);
		}
		json.insert("lineMap", arrayLineMap);
	}

	jsonLedModule.insert("ledModule", json);
	QJsonDocument document;
	document.setObject(jsonLedModule);
	QString jsonString(document.toJson(QJsonDocument::Indented));
	file.seek(0);
	file.write(jsonString.toUtf8().data());
	file.close();

	ModuleEditor::close();
}

void ModuleEditor::unlockAttrSet(void)
{
	if (++hideCnt%10 == 0) {
		setEnabledAttrSet(ledModule.change = !ledModule.change);
		if (ledModule.change == false)
			ui->pushButtonAttrSetOK->setEnabled(true);
	}
}

void ModuleEditor::setEnabledAttrSet(bool en)
{
	QObjectList list = ui->stackedWidget->widget(1)->children();
	for (auto obj: list)
		if (obj->isWidgetType()) static_cast<QWidget*>(obj)->setEnabled(en);
	ui->pushButtonAttrSetCancel->setEnabled(true);
	ui->pushButtonAttrSetUnlock->setEnabled(true);
}
