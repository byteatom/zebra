#include "FlashTextAttrCfger.h"
#include "ui_FlashTextAttrCfger.h"

#include <QDir>

#include "FlashColor.h"

FlashTextAttrCfger::FlashTextAttrCfger(QWidget *parent, FlashTextAttr *attr) :
	QGroupBox(parent),
	ui(new Ui::FlashTextAttrCfger),
	attr{attr}
{
	ui->setupUi(this);

	QStringList styleList;
	styleList << FlashTextAttrCfger::tr("HeratBeat") << FlashTextAttrCfger::tr("Waltz")
			  << FlashTextAttrCfger::tr("Excursion") << FlashTextAttrCfger::tr("RadioWave")
			  << FlashTextAttrCfger::tr("FlashBulb") << FlashTextAttrCfger::tr("Spring")
			  << FlashTextAttrCfger::tr("RollOver") << FlashTextAttrCfger::tr("Domino")
			  << FlashTextAttrCfger::tr("ExcursionReverse") << FlashTextAttrCfger::tr("Swirling")
			  << FlashTextAttrCfger::tr("NeonLight") << FlashTextAttrCfger::tr("RandomColor")
			  << FlashTextAttrCfger::tr("RainbowUp") << FlashTextAttrCfger::tr("RainbowDown")
			  << FlashTextAttrCfger::tr("RainbowLeft") << FlashTextAttrCfger::tr("RainbowRight");
	ui->comboBoxStyle->addItems(styleList);
	ui->bold->setChecked(attr->bold);
	ui->italic->setChecked(attr->italic);
	ui->underLine->setChecked(attr->underline);
	updataFontFile();
	if (this->attr->fontName.isEmpty())
		this->attr->fontName = ui->comboBoxFontType->currentText();
	else
		ui->comboBoxFontType->setCurrentText(this->attr->fontName);

	connect(ui->textEdit, &LineEditor::textChanged, this, &FlashTextAttrCfger::textChanged);
	connect(ui->fgColor, &QPushButton::clicked, ui->textEdit,
			[this](bool checked)->void {
				ui->textEdit->selectAll();
				int color = FlashColor::getFlashColor();
				if (color >= 0) {
					this->attr->fontColor = color;
					this->attr->changed();
				}
			});
	connect(ui->bold, &QPushButton::clicked, ui->textEdit,
			[this](bool checked)->void {
				ui->textEdit->selectAll();
				ui->textEdit->setFontBold(checked);
				this->attr->bold = checked;
				this->attr->changed();
			});
	connect(ui->underLine, &QPushButton::clicked, ui->textEdit,
			[this](bool checked)->void {
				ui->textEdit->selectAll();
				ui->textEdit->setFontUnderline(checked);
				this->attr->underline = checked;
				this->attr->changed();
			});
	connect(ui->italic, &QPushButton::clicked, ui->textEdit,
			[this](bool checked)->void {
				ui->textEdit->selectAll();
				ui->textEdit->setFontItalic(checked);
				this->attr->italic = checked;
				this->attr->changed();
			});
	connect(ui->point, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), ui->textEdit,
			[this](qreal s)->void {
				ui->textEdit->selectAll();
				ui->textEdit->setFontPointSize(s);
				this->attr->fontSize = s;
				this->attr->changed();
			});
	connect(ui->comboBoxStyle, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged), this,
			[this](int index)->void{
				this->attr->style = index;
				this->attr->changed();
			});
	connect(ui->spinBoxSpeed, static_cast<void (QSpinBox::*)(int value)>(&QSpinBox::valueChanged), this,
			[this](int value)->void{
				this->attr->speed = value;
			});
	connect(ui->comboBoxFontType, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this,
			[this](const QString &text){
				this->attr->fontName = text;
				this->attr->changed();
			});

	ui->comboBoxStyle->setCurrentIndex(attr->style);
	ui->spinBoxSpeed->setValue(attr->speed);
	ui->point->setValue(attr->fontSize);
	if (!attr->html.isEmpty()) {
		ui->textEdit->setHtml(attr->html);
		textChanged();
	}
}

FlashTextAttrCfger::~FlashTextAttrCfger()
{
	delete ui;
}

void FlashTextAttrCfger::textChanged()
{
	attr->text = ui->textEdit->toPlainText();
	attr->html = ui->textEdit->toHtml();
	attr->snapshot = ui->textEdit->snapshot();
	attr->changed();
}

int FlashTextAttrCfger::updataFontFile()
{
	QDir dir("font");
	QFileInfoList list;

	if (!dir.exists()) {
		return -1;
	}

	dir.setFilter(QDir::Files | QDir::NoSymLinks);
	list = dir.entryInfoList();

	ui->comboBoxFontType->blockSignals(true);
	for (int i = ui->comboBoxFontType->count(); i >= 0 ; --i)
		ui->comboBoxFontType->removeItem(i);
	ui->comboBoxFontType->blockSignals(false);

	int fileCnt = list.count();
	if (fileCnt <= 0)
		return -2;

	//QStringList strlist;
	QFileInfo file;
	QString suffix;
	ui->comboBoxFontType->blockSignals(true);
	for (int i = 0; i < fileCnt; ++i) {
		file = list.at(i);
		suffix = file.suffix();
		if (QString::compare(suffix, QString("ttc"), Qt::CaseInsensitive) == 0
		  || QString::compare(suffix, QString("ttf"), Qt::CaseInsensitive) == 0) {
			ui->comboBoxFontType->insertItem(i, file.completeBaseName());
		}
	}
	ui->comboBoxFontType->blockSignals(false);

	return 0;
}
