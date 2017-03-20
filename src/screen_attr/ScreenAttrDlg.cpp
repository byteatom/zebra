#include "ScreenAttrDlg.h"
#include "ui_ScreenAttrDlg.h"

#include <QDir>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QMessageBox>
#include <QString>
#include <QHash>

#include "ModuleEditor.h"

extern const QHash<QString, enum IScreen::BaseColor> baseColorJsonHash {
	{"one", IScreen::BaseColor::ONE},
	{"two", IScreen::BaseColor::TWO},
	{"three", IScreen::BaseColor::THREE}};
extern const QHash<QString, enum IScreen::WarpMode> warpModeJsonHash {
	{"up", IScreen::WarpMode::UP},
	{"down", IScreen::WarpMode::DOWN},
	{"straight", IScreen::WarpMode::STRAIGHT},
	{"scanDot", IScreen::WarpMode::SCANDOT}};
extern const QHash<QString, enum IScreen::DecodeMode> decodeModeJsonHash {
	{"high138", IScreen::DecodeMode::HIGH_138},
	{"low138", IScreen::DecodeMode::LOW_138},
	{"highIO", IScreen::DecodeMode::HIGH_IO},
	{"lowIO", IScreen::DecodeMode::LOW_IO}};
extern const QHash<int, int> scanLinesViewHash {
	{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 8}, {7, 10},
	{8, 12}, {9, 16}, {10, 32}};
extern const QHash<int, int> moduleLinesViewHash {
	{0, 1}, {1, 2}, {2, 4}, {3, 8}, {4, 10}, {5, 12}, {6, 16}, {7, 32}};
extern const QHash<int, int> warpBitsViewHash {
	{0, 4}, {1, 8}, {2, 16}, {3, 32}};

extern const QString moduleType = "json";
extern const QString moduleHeader = "(U)";
QString usrModulePath;
QString ownModulePath;

const struct {
	const char *model;
	int width;
	int height;
	int area;
	bool overlay;
	bool colorful;
} cardInfo[] {
	{"AUTO", 9999, 1024,9999*1024, true,  true},
#if 1
	{"XC2",  320,  32,  1024*128,  false, false},
	{"XU2",  512,  32,  1024*128,  false, false},
	{"X2",   1024, 32,  1024*128,  false, false},
	{"XU3",  1024, 48,  1024*128,  false, false},
	{"X3S",  1024, 48,  1024*128,  false, false},
	{"XU4",  1024, 64,  1024*128,  false, false},
	{"X4S",  1024, 64,  1024*128,  false, false},
	{"X4E",  1024, 64,  1024*128,  false, false},
	{"XU8",  1024, 128, 1024*128,  false, false},
	{"X0S",  1024, 256, 1024*128,  false, false},
	{"XQ1",  1024, 32,  1024*128,  false, false},
	{"XQ2",  1024, 64,  1024*128,  false, false},
	{"X4",   9999, 64,  9999*1024, true,  false},
	{"X6",   9999, 96,  9999*1024, true,  false},
	{"X8",   9999, 128, 9999*1024, true,  false},
	{"X8E",  9999, 128, 9999*1024, true,  false},
	{"X16",  9999, 256, 9999*1024, true,  false},
	{"X16E", 9999, 256, 9999*1024, true,  false},
	{"X32E", 9999, 512, 9999*1024, true,  false},
	{"X32",  9999, 512, 9999*1024, true,  false},
	{"XN4",  768, 128,  768*128,   false, true},
#endif
};

ScreenAttrDlg::ScreenAttrDlg(IMainWnd *mainWnd_, ISetting *setting, QWidget *parent) :
	QDialog(parent),
	mainWnd(mainWnd_),
	ui(new Ui::ScreenAttrDlg)
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setWindowIcon(QIcon(QPixmap("image/icon/screen_attr_64.png")));

	ui->addScan->setIcon(QPixmap("image/icon/add_128.png"));
	ui->delScan->setIcon(QPixmap("image/icon/del_128.png"));
	ui->editScan->setIcon(QPixmap("image/icon/edit_128.png"));

	ui->comboBoxColor->hide();
	ui->labelColor->hide();
	ui->read->hide();

#if 0
	ui->labelModel->hide();
	ui->comboBoxCardModel->hide();
#endif

	usrModulePath = "data/module";
	QDir().mkpath(usrModulePath);
	ownModulePath = "module";

	ledModule.model = "";
	ledModule.change = true;
	ledModule.baseColor = IScreen::BaseColor::ONE;
	ledModule.moduleLines = 16;
	ledModule.scanLines = 4;
	ledModule.warpTimes = 4;
	ledModule.warpBits = 8;
	ledModule.warpMode = IScreen::WarpMode::DOWN;
	ledModule.decodeMode = IScreen::DecodeMode::HIGH_138;
	ledModule.dataPolarity = true;
	ledModule.strPolarity = true;
	ledModule.oePolarity = true;
	ledModule.mirror = false;
	ledModule.split = false;
	ledModule.warpTurn = false;
	ledModule.scanDot = false;
	ledModule.moduleWidth = 32;
	ledModule.moduleHeight = 16;
	ledModule.emptyDot = 0;
	ledModule.dotIndex = 0;
	ledModule.emptyDotIndex = 0;
	ledModule.color1 = 0;
	ledModule.color2 = 0;
	ledModule.color3 = 0;
	ledModule.color4 = 0;
	ledModule.expandRGB = false;
	ledModule.complete = false;

	if (!mainWnd) return;

	INode *node = mainWnd->currentNode(INode::Type::SCREEN);
	if (!screen) screen = dynamic_cast<IScreen*>(node);

	if (screen) {
		setScreen = screen->screenPara();
		protocol = screen->protocol();
		if (protocol) protocol->open();
	}

	updataLedModule(usrModulePath, ownModulePath);
	if (setScreen.ledModel.isEmpty())
		setScreen.ledModel = ui->comboBoxLedModule->currentText();
	ui->comboBoxLedModule->setCurrentText(setScreen.ledModel);
	openLedModule(ui->comboBoxLedModule->currentData().toString());

	selectRadios = new QButtonGroup(this);
	selectRadios->addButton(ui->radioButtonClassical);
	selectRadios->addButton(ui->radioButtonColorful);

#if 1
	if (setScreen.cardSeries == IScreen::CLASSIAL) {
		ui->radioButtonClassical->setChecked(true);
	} else if (setScreen.cardSeries == IScreen::COLORFUL) {
		ui->radioButtonColorful->setChecked(true);
		setScreen.grayScale = IScreen::GrayScale::GRAY_4;
	}
#else
	ui->radioButtonClassical->setEnabled(false);
	ui->radioButtonColorful->setChecked(true);
	setScreen.grayScale = IScreen::GrayScale::GRAY_4;
#endif

	ui->comboBoxLedModule->view()->setMinimumWidth(this->width()*7/10);

	if (setScreen.autoMode == false) {
		ui->checkBoxAutoMode->setCheckState(Qt::Unchecked);
		ui->spinBoxFreq->setEnabled(true);
		ui->spinBoxBlank->setEnabled(true);
	} else {
		ui->checkBoxAutoMode->setCheckState(Qt::Checked);
		ui->spinBoxFreq->setEnabled(false);
		ui->spinBoxBlank->setEnabled(false);
		setScreen.frequency = 2;
		setScreen.lineBlank = 20;
	}

	ui->spinBoxWidth->setValue(setScreen.size->width());
	ui->spinBoxHeight->setValue(setScreen.size->height());
	ui->comboBoxOePolarity->setCurrentIndex(setScreen.oePolarity ? 1 : 0);
	ui->comboBoxDataPolarity->setCurrentIndex(setScreen.dataPolarity ? 1 : 0);
	ui->comboBoxStrPolarity->setCurrentIndex(setScreen.strPolarity ? 1 : 0);
	ui->comboBoxColor->setCurrentIndex(static_cast<int>(setScreen.baseColor));
	ui->spinBoxFreq->setValue(setScreen.frequency);
	ui->spinBoxBlank->setValue(setScreen.lineBlank);
	ui->comboBoxGray->setCurrentIndex(static_cast<int>(setScreen.grayScale));
	ui->spinBoxThreshold->setValue(setScreen.threshold);
	ui->spinBoxlLineVerify->setValue(setScreen.lineVerify);
	ui->spinBoxGama->setValue(setScreen.gama);
	//ui->groupBoxOther->setVisible(setScreen.seniorCfgHide);
	ui->groupBoxOther->setVisible(false);
	ui->pushButtonSeniorCfgHide->hide();

	QStringList cardModelList;
	for (auto card : cardInfo)
		cardModelList << tr(card.model);
	ui->comboBoxCardModel->addItems(cardModelList);
	ui->comboBoxCardModel->setCurrentText(setScreen.cardModel);

	connect(ui->checkBoxAutoMode, &QCheckBox::stateChanged,
			this, &ScreenAttrDlg::checkAutoMode);

	connect(selectRadios,
			static_cast<void (QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),
			this, &ScreenAttrDlg::selectMode);

	connect(ui->editScan, &QPushButton::clicked, this,
		[setting, this]()->void {
			IScreen::Module tmpLedModule = ledModule;
			ModuleEditor cfg(*setting, tmpLedModule, protocol, false, this); cfg.exec();
			ledModule = tmpLedModule;
			updataLedModule(usrModulePath, ownModulePath);
			ui->comboBoxLedModule->setCurrentText(ledModule.model);
			openLedModule(ui->comboBoxLedModule->currentData().toString());
		});

	connect(ui->addScan, &QPushButton::clicked, this,
		[setting, this]()->void {
			IScreen::Module tmpLedModule = ledModule;
			ModuleEditor cfg(*setting, tmpLedModule, protocol, true, this); cfg.exec();
			ledModule = tmpLedModule;
			updataLedModule(usrModulePath, ownModulePath);
			ui->comboBoxLedModule->setCurrentText(ledModule.model);
			openLedModule(ui->comboBoxLedModule->currentData().toString());
		});

	connect(ui->delScan, &QPushButton::clicked, this,
		[setting, this]()->void {
			if (ledModule.change == false) {
				QMessageBox::information(this, tr("Delete"), tr("Can not delete default LED module"), QMessageBox::Ok);
				return;
			}
			QString tmpLedModule = ui->comboBoxLedModule->currentText();
			int ret = QMessageBox::information(this, tr("Delete"), tmpLedModule + " " + tr("will be deleted"),
											   QMessageBox::Ok, QMessageBox::Cancel);
			if (ret == QMessageBox::Ok) {
				QFile file(usrModulePath + "/" + tmpLedModule + "." + moduleType);
				if (file.exists()) {
					if (file.remove()) {
						updataLedModule(usrModulePath, ownModulePath);
						openLedModule(ui->comboBoxLedModule->currentData().toString());
					} else
						QMessageBox::information(this, tr("Delete"), tr("None permission to delete LED module"), QMessageBox::Ok);
				} else
					QMessageBox::information(this, tr("Delete"), tr("LED module not exist"), QMessageBox::Ok);
			}
		});

	connect(ui->comboBoxLedModule,
		static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged), this,
		[setting, this](int index)->void {
			openLedModule(ui->comboBoxLedModule->itemData(index).toString());
			setScreen.oePolarity = ledModule.oePolarity;
			setScreen.dataPolarity = ledModule.dataPolarity;
			setScreen.strPolarity = ledModule.strPolarity;
			ui->comboBoxOePolarity->setCurrentIndex(setScreen.oePolarity ? 1 : 0);
			ui->comboBoxDataPolarity->setCurrentIndex(setScreen.dataPolarity ? 1 : 0);
			ui->comboBoxStrPolarity->setCurrentIndex(setScreen.strPolarity ? 1 : 0);
		});

	connect(ui->pushButtonSeniorCfgHide, &QPushButton::clicked, this,
		[this]() {
			ui->groupBoxOther->setVisible(setScreen.seniorCfgHide = !setScreen.seniorCfgHide);
		});

	connect(ui->cancel, &QPushButton::clicked, this, &ScreenAttrDlg::accept);

	connect(ui->save, &QPushButton::clicked, this, &ScreenAttrDlg::save);

	connect(ui->spinBoxWidth,
			static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this,
			[this](int value){
				for (auto card : cardInfo) {
					if (card.model == ui->comboBoxCardModel->currentText()) {
						if (value > card.width) ui->spinBoxWidth->setValue(card.width);
					}
				}
			});

	connect(ui->spinBoxHeight,
			static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this,
			[this](int value){
				for (auto card : cardInfo) {
					if (card.model == ui->comboBoxCardModel->currentText()) {
						if (value > card.height) ui->spinBoxHeight->setValue(card.height);
					}
				}
			});
}

ScreenAttrDlg::~ScreenAttrDlg()
{
	if (protocol) protocol->close();
	delete ui;
}

void ScreenAttrDlg::save(void)
{
	setScreen.cardModel = ui->comboBoxCardModel->currentText();
	setScreen.ledModel = ui->comboBoxLedModule->currentText();
	setScreen.size = QSize{ui->spinBoxWidth->value(), ui->spinBoxHeight->value()};
	setScreen.oePolarity = ui->comboBoxOePolarity->currentIndex() == 0 ? false : true;
	setScreen.dataPolarity = ui->comboBoxDataPolarity->currentIndex() == 0 ? false : true;
	setScreen.strPolarity = ui->comboBoxStrPolarity->currentIndex() == 0 ? false : true;
	setScreen.baseColor = static_cast<IScreen::BaseColor>(ui->comboBoxColor->currentIndex());
	setScreen.frequency = ui->spinBoxFreq->value();
	setScreen.lineBlank = ui->spinBoxBlank->value();
	setScreen.threshold = ui->spinBoxThreshold->value();
	setScreen.lineVerify = ui->spinBoxlLineVerify->value();
	setScreen.gama = ui->spinBoxGama->value();
	setScreen.grayScale = static_cast<IScreen::GrayScale>(ui->comboBoxGray->currentIndex());
	//setScreen.seniorCfgHide = setScreen.seniorCfgHide;

	if (screen) {
		IScreen::ScreenPara &scrSetScreen = screen->screenPara();
		scrSetScreen = setScreen;

		IScreen::Module &scrLedModule = screen->module();
		scrLedModule = ledModule;

		INode *node = dynamic_cast<INode*>(screen);
		if (node) {
			node->activate();
			mainWnd->setCurrentNode(node);
		}
	}

	ScreenAttrDlg::close();
}

int ScreenAttrDlg::updataLedModule(const QString &usrPath, const QString &ownPath)
{
	QFileInfoList usrList, ownList;
	QDir usrDir(usrPath), ownDir(ownPath);

	if (!usrDir.exists())
		return -1;

	ui->comboBoxLedModule->blockSignals(true);
	for (int i = ui->comboBoxLedModule->count(); i >= 0 ; --i)
		ui->comboBoxLedModule->removeItem(i);
	ui->comboBoxLedModule->blockSignals(false);

	int fileCnt;
	int cnt = 0;
	QFileInfo file;
	QString suffix;

	if (!ownPath.isEmpty() && ownDir.exists()) {
		ownDir.setFilter(QDir::Files | QDir::NoSymLinks);
		ownList = ownDir.entryInfoList();
		if ((fileCnt = ownList.count()) > 0) {
			ui->comboBoxLedModule->blockSignals(true);
			for (int i = 0; i < fileCnt; ++i) {
				file = ownList.at(i);
				suffix = file.suffix();
				if (QString::compare(suffix, QString("jdef"), Qt::CaseInsensitive) == 0) {
					ui->comboBoxLedModule->insertItem(cnt, file.completeBaseName(), QVariant(file.filePath()));
					ui->comboBoxLedModule->setItemIcon(cnt, QIcon("image/icon/module_default_48.png"));
					cnt++;
				}
			}
			for (int i = 0; i < fileCnt; ++i) {
				file = ownList.at(i);
				suffix = file.suffix();
				if (QString::compare(suffix, QString("json"), Qt::CaseInsensitive) == 0) {
					ui->comboBoxLedModule->insertItem(cnt, file.completeBaseName(), QVariant(file.filePath()));
					ui->comboBoxLedModule->setItemIcon(cnt, QIcon("image/icon/module_own_48.png"));
					cnt++;
				}
			}
			ui->comboBoxLedModule->blockSignals(false);
		}
	}

	usrDir.setFilter(QDir::Files | QDir::NoSymLinks);
	usrList = usrDir.entryInfoList();
	if ((fileCnt = usrList.count()) <= 0)
		return -2;

	ui->comboBoxLedModule->blockSignals(true);
	for (int i = 0; i < fileCnt; ++i) {
		file = usrList.at(i);
		suffix = file.suffix();
		if (QString::compare(suffix, QString("json"), Qt::CaseInsensitive) == 0) {
			ui->comboBoxLedModule->insertItem(cnt, file.completeBaseName(), QVariant(file.filePath()));
			ui->comboBoxLedModule->setItemIcon(cnt, QIcon("image/icon/module_user_48.png"));
			cnt++;
		}
	}
	ui->comboBoxLedModule->blockSignals(false);

	return 0;
}

int ScreenAttrDlg::openLedModule(const QString &path)
{
	QFile file(path);
	bool success = file.open(QIODevice::ReadOnly);
	if (!success)
		return -1;

	QByteArray byteArray(file.size() + 1, 0);
	file.seek(0);
	byteArray = file.readAll();

	QJsonParseError jsonError;
	QJsonDocument parseDoucment = QJsonDocument::fromJson(byteArray, &jsonError);

	if ((jsonError.error == QJsonParseError::NoError) && parseDoucment.isObject()) {
		QJsonObject object = parseDoucment.object().take("ledModule").toObject();
		QFileInfo fileInfo(file);
		//qDebug() << fileInfo.completeBaseName() << object.take("model").toString();
		ledModule.model = fileInfo.completeBaseName();//object.take("model").toString();
		ledModule.change = object.take("change").toBool();
		ledModule.baseColor = baseColorJsonHash.value(object.take("baseColor").toString());
		ledModule.moduleLines = object.take("moduleLines").toInt();
		ledModule.scanLines = object.take("scanLines").toInt();
		ledModule.warpTimes = object.take("warpTimes").toInt();
		ledModule.warpBits = object.take("warpBits").toInt();
		ledModule.warpMode = warpModeJsonHash.value(object.take("warpMode").toString());
		ledModule.decodeMode = decodeModeJsonHash.value(object.take("decodeMode").toString());
		ledModule.dataPolarity = object.take("dataPolarity").toBool();
		ledModule.strPolarity = object.take("strPolarity").toBool();
		ledModule.oePolarity = object.take("oePolarity").toBool();
		ledModule.mirror = object.take("mirror").toBool();
		ledModule.split = object.take("split").toBool();
		ledModule.warpTurn = object.take("warpTurn").toBool();
		ledModule.scanDot = object.take("scanDot").toBool();
		ledModule.moduleWidth = object.take("moduleWidth").toInt();
		ledModule.moduleHeight = object.take("moduleHeight").toInt();
		ledModule.emptyDot = object.take("emptyDot").toInt();
		ledModule.dotIndex = object.take("dotIndex").toInt();
		ledModule.emptyDotIndex = object.take("emptyDotIndex").toInt();
		ledModule.color1 = object.take("color1").toInt();
		ledModule.color2 = object.take("color2").toInt();
		ledModule.color3 = object.take("color3").toInt();
		ledModule.color4 = object.take("color4").toInt();
		ledModule.expandRGB = object.take("expandRGB").toBool();
		ledModule.complete = object.take("complete").toBool();

		QJsonArray arrayLineMap = object.take("lineMap").toArray();
		for (int i = 0; i < ledModule.scanLines; ++i) {
			ledModule.lineMap[i] = arrayLineMap.at(i).toInt();
		}

		QJsonArray arrayDotMap = object.take("dotMap").toArray();
		int loopLen = ledModule.dotIndex/ledModule.scanLines;
		for (int j = 0; j < ledModule.scanLines; ++j) {
			for (int i = 0; i < loopLen; ++i) {
				ledModule.dotMap[j*loopLen + i].empty = arrayDotMap.at(i).toObject().take("empty").toBool();
				ledModule.dotMap[j*loopLen + i].x = arrayDotMap.at(i).toObject().take("x").toInt();
				ledModule.dotMap[j*loopLen + i].y = arrayDotMap.at(i).toObject().take("y").toInt() + ledModule.lineMap[j];
			}
		}
	}

	file.close();

	return 0;
}

void ScreenAttrDlg::selectMode(QAbstractButton *button)
{
	if (button == ui->radioButtonClassical) {
		ui->radioButtonClassical->setChecked(true);
		setScreen.cardSeries = IScreen::CardSeries::CLASSIAL;
		setScreen.grayScale = IScreen::GrayScale::NONE;
		ui->comboBoxGray->setCurrentIndex(static_cast<int>(setScreen.grayScale));
	} else if (button == ui->radioButtonColorful) {
		ui->radioButtonColorful->setChecked(true);
		setScreen.cardSeries = IScreen::CardSeries::COLORFUL;
		setScreen.grayScale = IScreen::GrayScale::GRAY_4;
		ui->comboBoxGray->setCurrentIndex(static_cast<int>(setScreen.grayScale));
	}
}

void ScreenAttrDlg::checkAutoMode(int state)
{
	if (state) {
		setScreen.autoMode = true;
		ui->checkBoxAutoMode->setCheckState(Qt::Checked);
		ui->spinBoxFreq->setEnabled(false);
		ui->spinBoxBlank->setEnabled(false);
		setScreen.frequency = 2;
		setScreen.lineBlank = 20;
		ui->spinBoxFreq->setValue(setScreen.frequency);
		ui->spinBoxBlank->setValue(setScreen.lineBlank);
	} else {
		setScreen.autoMode = false;
		ui->checkBoxAutoMode->setCheckState(Qt::Unchecked);
		ui->spinBoxFreq->setEnabled(true);
		ui->spinBoxBlank->setEnabled(true);
	}
}
