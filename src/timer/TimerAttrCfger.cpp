#include "TimerAttrCfger.h"
#include "ui_TimerAttrCfger.h"

TimerAttrCfger::TimerAttrCfger(QWidget *parent, TimerAttr *attr_) :
	QGroupBox(parent),
	ui(new Ui::TimerAttrCfger),
	attr{attr_}
{
	ui->setupUi(this);

	mode.addButton(ui->countDown);
	mode.addButton(ui->elapse);

	parts[0] = ui->day;
	parts[1] = ui->hour;
	parts[2] = ui->minute;
	parts[3] = ui->second;

	Attr<int>& colorMode = attr->box->colorMode();
	ui->digitColor->setMode(colorMode);
	ui->unitColor->setMode(colorMode);
	ui->textColor->setMode(colorMode);

	connect(&mode, static_cast<void(QButtonGroup::*)(QAbstractButton *, bool)>(&QButtonGroup::buttonToggled),
			[this](QAbstractButton *button, bool checked){
		attr->elapse = (button == ui->elapse && checked);
		attr->changed();
	});
	connect(ui->target, &QDateTimeEdit::dateTimeChanged,
			[this](const QDateTime &datetime) {
		attr->target = datetime;
		attr->changed();
	});
	connect(ui->day, &QCheckBox::toggled, [this](bool checked){
		attr->day = checked;
		attr->changed();
	});
	connect(ui->hour, &QCheckBox::toggled, [this](bool checked){
		attr->hour = checked;
		attr->changed();
	});
	connect(ui->minute, &QCheckBox::toggled, [this](bool checked){
		attr->minute = checked;
		attr->changed();
	});
	connect(ui->second, &QCheckBox::toggled, [this](bool checked){
		attr->second = checked;
		attr->changed();
	});
	connect(ui->unit, &QCheckBox::toggled, [this](bool checked){
		attr->unit = checked;
		ui->unitColorTag->setEnabled(checked);
		ui->unitColor->setEnabled(checked);
		attr->changed();
	});
	connect(ui->unitColor, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
			[this](int index) {
		attr->unitColor = ui->unitColor->getCurrentArgb();
		attr->changed();
	});
	connect(ui->digitColor, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
			[this](int index) {
		attr->digitColor = ui->digitColor->getCurrentArgb();
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
	connect(ui->extraText, &QGroupBox::toggled, [this](bool checked){
		attr->extraText = checked;
		if (checked) ui->text->setFocus();
		attr->changed();
	});
	connect(ui->text, &QLineEdit::textChanged, [this](const QString& text) {
		attr->text = text;
		attr->changed();
	});
	connect(ui->extraLine, &QCheckBox::toggled, [this](bool checked){
		attr->extraLine = checked;
		attr->changed();
	});
	connect(ui->textColor, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
			[this](int index) {
		attr->textColor = ui->textColor->getCurrentArgb();
		attr->changed();
	});

	ui->countDown->setChecked(!attr->elapse);
	ui->elapse->setChecked(attr->elapse);
	ui->target->setDateTime(attr->target);
	ui->day->setChecked(attr->day);
	ui->hour->setChecked(attr->hour);
	ui->minute->setChecked(attr->minute);
	ui->second->setChecked(attr->second);
	ui->unit->setChecked(attr->unit);
	ui->unitColor->setCurrentArgb(attr->unitColor);
	ui->digitColor->setCurrentArgb(attr->digitColor);
	ui->family->setCurrentFont(QFont(attr->family));
	ui->point->setValue(attr->point);
	ui->bold->setChecked(attr->bold);
	ui->italic->setChecked(attr->italic);
	ui->underLine->setChecked(attr->underline);
	ui->extraText->setChecked(attr->extraText);
	ui->text->setText(attr->text);
	ui->extraLine->setChecked(attr->extraLine);
	ui->textColor->setCurrentArgb(attr->textColor);
}

TimerAttrCfger::~TimerAttrCfger()
{
	delete ui;
}

void TimerAttrCfger::makeContinuity(QCheckBox* current)
{

}
