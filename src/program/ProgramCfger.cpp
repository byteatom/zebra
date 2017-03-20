#include "ProgramCfger.h"
#include "ui_ProgramCfger.h"

ProgramCfger::ProgramCfger(ProgramAttr *attr_):
	Cfger(attr_),
	ui(new Ui::ProgramCfger),
	attr{attr_}
{
	ui->setupUi(this);

	typeRadios.addButton(ui->sequential, ProgramAttr::Mode::SEQUENTIAL);
	typeRadios.addButton(ui->specified, ProgramAttr::Mode::SPECIFIED);
	typeRadios.addButton(ui->fixed, ProgramAttr::Mode::FIXED);
	typeRadios.addButton(ui->external, ProgramAttr::Mode::EXTERNAL);

	ui->start->setDateTime(attr->start);
	ui->stop->setDateTime(attr->stop);
	setEveryday();
	ui->monday->setChecked(attr->monday);
	ui->tuesday->setChecked(attr->tuesday);
	ui->wednesday->setChecked(attr->wednesday);
	ui->thursday->setChecked(attr->thursday);
	ui->friday->setChecked(attr->friday);
	ui->saturday->setChecked(attr->saturday);
	ui->sunDay->setChecked(attr->sunDay);
	setFixed();
	setExternal();

	connect(&typeRadios, static_cast<void(QButtonGroup::*)(QAbstractButton *, bool)>(&QButtonGroup::buttonToggled),
		[this](QAbstractButton *button, bool checked){
			ui->start->setEnabled(button == ui->specified && checked);
			ui->stop->setEnabled(button == ui->specified && checked);
			ui->every->setEnabled(button == ui->specified && checked);
			ui->monday->setEnabled(button == ui->specified && checked);
			ui->tuesday->setEnabled(button == ui->specified && checked);
			ui->wednesday->setEnabled(button == ui->specified && checked);
			ui->thursday->setEnabled(button == ui->specified && checked);
			ui->friday->setEnabled(button == ui->specified && checked);
			ui->saturday->setEnabled(button == ui->specified && checked);
			ui->sunDay->setEnabled(button == ui->specified && checked);
			ui->fixedDays->setEnabled(button == ui->fixed && checked);
			ui->fixedDaysTag->setEnabled(button == ui->fixed && checked);
			ui->fixedHours->setEnabled(button == ui->fixed && checked);
			ui->fixedHoursTag->setEnabled(button == ui->fixed && checked);
			ui->fixedMinutes->setEnabled(button == ui->fixed && checked);
			ui->fixedMinutesTag->setEnabled(button == ui->fixed && checked);
			ui->fixedSeconds->setEnabled(button == ui->fixed && checked);
			ui->fixedSecondsTag->setEnabled(button == ui->fixed && checked);
			ui->externalDays->setEnabled(button == ui->external && checked);
			ui->externalDaysTag->setEnabled(button == ui->external && checked);
			ui->externalHours->setEnabled(button == ui->external && checked);
			ui->externalHoursTag->setEnabled(button == ui->external && checked);
			ui->externalMinutes->setEnabled(button == ui->external && checked);
			ui->externalMinutesTag->setEnabled(button == ui->external && checked);
			ui->externalSeconds->setEnabled(button == ui->external && checked);
			ui->externalSecondsTag->setEnabled(button == ui->external && checked);

			attr->mode = typeRadios.checkedId();
	});
	typeRadios.button(attr->mode)->toggle();

	connect(ui->start, &QDateTimeEdit::dateTimeChanged,
			[this](const QDateTime &datetime) {
		attr->start = datetime;
	});
	connect(ui->stop, &QDateTimeEdit::dateTimeChanged,
			[this](const QDateTime &datetime) {
		attr->stop = datetime;
	});
	connect(ui->fixedDays, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			[this](int value) {
		getFixed();
	});
	connect(ui->fixedHours, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			[this](int value) {
		getFixed();
	});
	connect(ui->fixedMinutes, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			[this](int value) {
		getFixed();
	});
	connect(ui->fixedSeconds, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			[this](int value) {
		getFixed();
	});
	connect(ui->externalDays, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			[this](int value) {
		getExternal();
	});
	connect(ui->externalHours, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			[this](int value) {
		getExternal();
	});
	connect(ui->externalMinutes, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			[this](int value) {
		getExternal();
	});
	connect(ui->externalSeconds, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			[this](int value) {
		getExternal();
	});
	connect(ui->monday, &QCheckBox::toggled, [this](bool checked) {
		attr->monday = checked;
		setEveryday();
	});
	connect(ui->tuesday, &QCheckBox::toggled, [this](bool checked) {
		attr->tuesday = checked;
		setEveryday();
	});
	connect(ui->wednesday, &QCheckBox::toggled, [this](bool checked) {
		attr->wednesday = checked;
		setEveryday();
	});
	connect(ui->thursday, &QCheckBox::toggled, [this](bool checked) {
		attr->thursday = checked;
		setEveryday();
	});
	connect(ui->friday, &QCheckBox::toggled, [this](bool checked) {
		attr->friday = checked;
		setEveryday();
	});
	connect(ui->saturday, &QCheckBox::toggled, [this](bool checked) {
		attr->saturday = checked;
		setEveryday();
	});
	connect(ui->sunDay, &QCheckBox::toggled, [this](bool checked) {
		attr->sunDay = checked;
		setEveryday();
	});
	connect(ui->every, &QCheckBox::toggled, [this](bool checked) {
		if (!checked) return;
		ui->monday->setChecked(true);
		ui->tuesday->setChecked(true);
		ui->wednesday->setChecked(true);
		ui->thursday->setChecked(true);
		ui->friday->setChecked(true);
		ui->saturday->setChecked(true);
		ui->sunDay->setChecked(true);
	});

	if (attr->background) {
		QWidget* backgroundCfger = attr->background->newCfger();
		append(backgroundCfger);
	}

	if (attr->border) {
		QWidget* borderCfger = attr->border->newCfger();
		append(borderCfger);
	}
}

ProgramCfger::~ProgramCfger() {
	delete ui;
}

void ProgramCfger::getFixed()
{
	attr->fixedValue = 86400 * ui->fixedDays->value()
					   + 3600 * ui->fixedHours->value()
					   + 60 * ui->fixedMinutes->value()
					   + ui->fixedSeconds->value();
}

void ProgramCfger::setFixed()
{
	ui->fixedDays->setValue(attr->fixedValue / 86400);
	ui->fixedHours->setValue(attr->fixedValue % 86400 / 3600);
	ui->fixedMinutes->setValue(attr->fixedValue % 86400 % 3600 / 60);
	ui->fixedSeconds->setValue(attr->fixedValue % 86400 % 3600 % 60);
}

void ProgramCfger::getExternal()
{
	attr->externalValue = 86400 * ui->externalDays->value()
					   + 3600 * ui->externalHours->value()
					   + 60 * ui->externalMinutes->value()
					   + ui->externalSeconds->value();
}

void ProgramCfger::setExternal()
{
	ui->externalDays->setValue(attr->externalValue / 86400);
	ui->externalHours->setValue(attr->externalValue % 86400 / 3600);
	ui->externalMinutes->setValue(attr->externalValue % 86400 %3600 / 60);
	ui->externalSeconds->setValue(attr->externalValue % 86400 %3600 % 60);
}


void ProgramCfger::setEveryday()
{
	ui->every->setChecked(attr->monday
			&& attr->tuesday
			&& attr->wednesday
			&& attr->thursday
			&& attr->friday
			&& attr->saturday
			&& attr->sunDay);
}
