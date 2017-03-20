#include "VariableAttrCfger.h"
#include "ui_VariableAttrCfger.h"

VariableAttrCfger::VariableAttrCfger(QWidget *parent, VariableAttr *attr_) :
	QGroupBox(parent),
	ui(new Ui::VariableAttrCfger),
	attr{attr_}
{
	ui->setupUi(this);

	ui->color->setMode(attr->box->colorMode());

	connect(ui->x, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			[this](int value) {
		attr->x = value;
		attr->changed();
	});
	connect(ui->y, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			[this](int value) {
		attr->y = value;
		attr->changed();
	});
	connect(ui->text, &QPlainTextEdit::textChanged, [this]() {
		attr->text = ui->text->toPlainText();
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
	connect(ui->color, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
			[this](int index) {
		attr->color = ui->color->getCurrentArgb();
		attr->changed();
	});
	connect(ui->test, &QPushButton::clicked, [this]() {

	});

	ui->id->setValue(attr->id);
	ui->x->setValue(attr->x);
	ui->y->setValue(attr->y);
	ui->text->setPlainText(attr->text);
	ui->family->setCurrentFont(QFont(attr->family));
	ui->point->setValue(attr->point);
	ui->color->setCurrentArgb(attr->color);
}

VariableAttrCfger::~VariableAttrCfger()
{
	delete ui;
}

void VariableAttrCfger::textChanged()
{
}
