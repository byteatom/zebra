#include "CalendarAttrCfger.h"
#include "ui_CalendarAttrCfger.h"

CalendarAttrCfger::CalendarAttrCfger(QWidget *parent, CalendarAttr *attr_) :
	QGroupBox(parent),
	ui(new Ui::CalendarAttrCfger),
	attr{attr_}
{
	ui->setupUi(this);

	Attr<int>& colorMode = attr->box->colorMode();

	for (int i = 0; i < CalendarAttr::dateStyleCount(); ++i) {
		ui->dateFormats->addItem(CalendarAttr::dateStyles[i]->ui);
	}
	ui->dateColor->setMode(colorMode);

	for (int i = 0; i < CalendarAttr::weekStyleCount(); ++i) {
		ui->weekFormats->addItem(CalendarAttr::weekStyles[i][0]);
	}
	ui->weekColor->setMode(colorMode);

	for (int i = 0; i < CalendarAttr::timeStyleCount(); ++i) {
		ui->timeFormats->addItem(CalendarAttr::timeStyles[i]->ui);
	}
	ui->timeColor->setMode(colorMode);

	ui->textColor->setMode(colorMode);

	connect(ui->multiLine, &QCheckBox::toggled, [this](bool checked){
		attr->multiLine = checked;
		attr->changed();
	});
	connect(ui->dateFirst, &QCheckBox::toggled, [this](bool checked){
		attr->dateFirst = checked;
		attr->changed();
	});
	connect(ui->space, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			[this](int value) {
		attr->space = value;
		attr->changed();
	});
	connect(ui->date, &QCheckBox::toggled, [this](bool checked){
		attr->date = checked;
		ui->dateFormats->setEnabled(checked);
		ui->dateColor->setEnabled(checked);
		attr->changed();
	});
	connect(ui->dateFormats, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
			[this](int index) {
		attr->dateStyle = index;
		attr->changed();
	});
	connect(ui->dateColor, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
			[this](int index) {
		attr->dateColor = ui->dateColor->getCurrentArgb();
		attr->changed();
	});
	connect(ui->week, &QCheckBox::toggled, [this](bool checked){
		attr->week = checked;
		ui->weekFormats->setEnabled(checked);
		ui->weekColor->setEnabled(checked);
		attr->changed();
	});
	connect(ui->weekFormats, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
			[this](int index) {
		attr->weekStyle = index;
		attr->changed();
	});
	connect(ui->weekColor, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
			[this](int index) {
		attr->weekColor = ui->weekColor->getCurrentArgb();
		attr->changed();
	});
	connect(ui->time, &QCheckBox::toggled, [this](bool checked){
		attr->time = checked;
		ui->timeFormats->setEnabled(checked);
		ui->timeColor->setEnabled(checked);
		attr->changed();
	});
	connect(ui->timeFormats, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
			[this](int index) {
		attr->timeStyle = index;
		attr->changed();
	});
	connect(ui->timeColor, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
			[this](int index) {
		attr->timeColor = ui->timeColor->getCurrentArgb();
		attr->changed();
	});
	connect(ui->offset, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
			[this](int index) {
		attr->offset = index;
		attr->changed();
	});
	connect(ui->offsetValue, &QTimeEdit::timeChanged, [this](const QTime &time) {
		attr->offsetHour = time.hour();
		attr->offsetMinute = time.minute();
		attr->changed();
	});
	connect(ui->extraText, &QCheckBox::toggled, [this](bool checked){
		attr->extraText = checked;
		ui->textColorTag->setEnabled(checked);
		ui->textColor->setEnabled(checked);
		ui->text->setEnabled(checked);
		if (checked) ui->text->setFocus();
		attr->changed();
	});
	connect(ui->text, &QLineEdit::textChanged, [this](const QString& text) {
		attr->text = text;
		attr->changed();
	});
	connect(ui->textColor, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
			[this](int index) {
		attr->textColor = ui->textColor->getCurrentArgb();
		attr->changed();
	});
	connect(ui->family, &QFontComboBox::currentFontChanged, [this](const QFont& font) {
		attr->family = font.family();
		attr->changed();
	});
	connect(ui->point, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			[this](int value) {
		attr->point = value;
		attr->changed();
	});
	connect(ui->bold, &QPushButton::toggled, [this](bool checked){
		attr->bold = checked;
		attr->changed();
	});
	connect(ui->italic, &QPushButton::toggled, [this](bool checked){
		attr->italic = checked;
		attr->changed();
	});
	connect(ui->underLine, &QPushButton::toggled, [this](bool checked){
		attr->underline = checked;
		attr->changed();
	});

	ui->multiLine->setChecked(attr->multiLine);
	ui->dateFirst->setChecked(attr->dateFirst);
	ui->space->setValue(attr->space);
	ui->date->setChecked(attr->date);
	ui->dateFormats->setCurrentIndex(attr->dateStyle);
	ui->dateColor->setCurrentArgb(attr->dateColor);
	ui->week->setChecked(attr->week);
	ui->weekFormats->setCurrentIndex(attr->weekStyle);
	ui->weekColor->setCurrentArgb(attr->weekColor);
	ui->time->setChecked(attr->time);
	ui->timeFormats->setCurrentIndex(attr->timeStyle);
	ui->timeColor->setCurrentArgb(attr->timeColor);
	ui->offset->setCurrentIndex(attr->offset);
	ui->offsetValue->setTime(QTime{attr->offsetHour, attr->offsetMinute});
	ui->family->setCurrentFont(QFont(attr->family));
	ui->point->setValue(attr->point);
	ui->bold->setChecked(attr->bold);
	ui->italic->setChecked(attr->italic);
	ui->underLine->setChecked(attr->underline);
	ui->extraText->setChecked(attr->extraText);
	ui->text->setText(attr->text);
	ui->textColor->setCurrentArgb(attr->textColor);
}

CalendarAttrCfger::~CalendarAttrCfger()
{
	delete ui;
}

void CalendarAttrCfger::textChanged()
{
}
