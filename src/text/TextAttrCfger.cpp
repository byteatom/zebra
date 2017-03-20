#include "TextAttrCfger.h"
#include "ui_TextAttrCfger.h"

#include <QColorDialog>

TextAttrCfger::TextAttrCfger(QWidget *parent, TextAttr *attr_) :
	QGroupBox(parent),
	ui(new Ui::TextAttrCfger),
	attr{attr_}
{
	ui->setupUi(this);

	alignBtns.addButton(ui->left, Qt::AlignLeft);
	alignBtns.addButton(ui->center, Qt::AlignCenter);
	alignBtns.addButton(ui->right, Qt::AlignRight);

	connect(ui->text, &LineEditor::textChanged,
			this, &TextAttrCfger::cfgChanged, Qt::QueuedConnection);
	connect(ui->text, &LineEditor::currentCharFormatChanged,
			this, &TextAttrCfger::currentCharFormatChanged);
	connect(ui->family,	&QFontComboBox::currentFontChanged,
			ui->text, &LineEditor::setFontFamily);
	connect(ui->left, &QPushButton::toggled,
			[this](bool checked) {
		if (!checked) return;
		attr->align = Qt::AlignLeft;
		ui->text->setAlignment(attr->align);
	});
	connect(ui->center, &QPushButton::toggled,
			[this](bool checked) {
		if (!checked) return;
		attr->align = Qt::AlignCenter;
		ui->text->setAlignment(attr->align);
	});
	connect(ui->right, &QPushButton::toggled,
			[this](bool checked) {
		if (!checked) return;
		attr->align = Qt::AlignRight;
		ui->text->setAlignment(attr->align);
	});
	connect(ui->bgColor, &QPushButton::clicked,
			this, &TextAttrCfger::setBgColor);
	connect(ui->textBgColor, &QPushButton::clicked,
			this, &TextAttrCfger::setTextBackgroundColor);
	connect(ui->textColor, &QPushButton::clicked,
			this, &TextAttrCfger::setTextColor);
	connect(ui->bold, &QPushButton::clicked,
			ui->text, &LineEditor::setFontBold);
	connect(ui->underLine, &QPushButton::clicked,
			ui->text, &LineEditor::setFontUnderline);
	connect(ui->italic, &QPushButton::clicked,
			ui->text, &LineEditor::setFontItalic);
	connect(ui->point, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			ui->text, &LineEditor::setFontPointSize);
	connect(ui->letterSpace, static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
			this, &TextAttrCfger::setLetterSpace);
	connect(ui->strechHor, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			this, &TextAttrCfger::strechHor);
	connect(ui->strechVer, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			this, &TextAttrCfger::strechVer);
	connect(ui->offset, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			this, &TextAttrCfger::offset);

	ui->text->setBackgroundColor(QColor{attr->bgColor});
	if (!attr->html.isEmpty()) {
		ui->text->setHtml(attr->html);
		alignBtns.button(attr->align)->setChecked(true);
		ui->letterSpace->setValue(attr->letterSpace);
		ui->strechHor->setValue(attr->strechHor);
		ui->strechVer->setValue(attr->strechVer);
		ui->offset->setValue(-attr->offset);
	}
	ui->text->setFocus();
}

TextAttrCfger::~TextAttrCfger()
{
	delete ui;
}

void TextAttrCfger::currentCharFormatChanged(const QTextCharFormat &format) {
	QFont font = format.font();
	ui->family->blockSignals(true);
	ui->family->setCurrentText(QFontInfo(font).family());
	ui->family->blockSignals(false);
	ui->point->blockSignals(true);
	ui->point->setValue(font.pointSize());
	ui->point->blockSignals(false);
	ui->bold->setChecked(font.bold());
	ui->italic->setChecked(font.italic());
	ui->underLine->setChecked(font.underline());
}

void TextAttrCfger::setBgColor()
{
	QColor color = QColorDialog::getColor(QColor(attr->bgColor), this,
										  tr("Select Background Color"),
										  QColorDialog::ShowAlphaChannel);
	if (!color.isValid()) return;

	attr->bgColor = color.rgba();
	ui->text->setBackgroundColor(QColor{attr->bgColor});
	cfgChanged();
}

void TextAttrCfger::setTextBackgroundColor()
{
	QColor color = QColorDialog::getColor(Qt::black, this,
										  tr("Select Text Background Color"),
										  QColorDialog::ShowAlphaChannel);
	if (!color.isValid()) return;
	ui->text->setTextBackgroundColor(color);
}

void TextAttrCfger::setTextColor()
{
	QColor color = QColorDialog::getColor(Qt::white, this,
										  tr("Select Text Color"),
										  QColorDialog::ShowAlphaChannel);
	if (!color.isValid()) return;
	ui->text->setTextColor(color);
}

void TextAttrCfger::setLetterSpace(qreal value)
{
	attr->letterSpace = ui->letterSpace->value();
	ui->text->setLetterSpace(value);
}

void TextAttrCfger::strechHor(int value)
{
	attr->strechHor = ui->strechHor->value();
	cfgChanged();
}

void TextAttrCfger::strechVer(int value)
{
	attr->strechVer = ui->strechVer->value();
	cfgChanged();
}

void TextAttrCfger::offset(int value)
{
	attr->offset = - ui->offset->value();
	cfgChanged();
}

void TextAttrCfger::cfgChanged()
{
	attr->html = ui->text->toHtml();
	QTextDocument doc;
	doc.setHtml(attr->html);
	doc.setDocumentMargin(0);
	RichEditor::setDocLetterSpace(&doc, attr->letterSpace);
	attr->snapshot = RichEditor::snapshot(&doc);
	if (attr->strechHor > 0 || attr->strechVer > 0)
		attr->snapshot = attr->snapshot.scaled(attr->snapshot.width() + attr->strechHor,
											   attr->snapshot.height() + attr->strechVer);
	attr->print();
	attr->changed();
}
