#include "TextEditor.h"
#include "ui_TextEditor.h"

#include "TextItem.h"
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QColorDialog>

TextEditor::TextEditor(QWidget *parent, QSize size, TextItem* item) :
	QDialog(parent),
	ui(new Ui::TextEditor),
	size{size},
	item{item}
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	alignBtns.addButton(ui->left);
	alignBtns.addButton(ui->center);
	alignBtns.addButton(ui->right);
	alignBtns.addButton(ui->justify);

	connect(ui->text, &RichEditor::textChanged,
			this, &TextEditor::textChanged);
	connect(ui->text, &RichEditor::currentCharFormatChanged,
			this, &TextEditor::currentCharFormatChanged);
	connect(ui->text, &RichEditor::cursorPositionChanged,
			this, &TextEditor::cursorPositionChanged);
	connect(ui->family,	&QFontComboBox::currentFontChanged,
			ui->text, &RichEditor::setFontFamily);
	connect(ui->point, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			ui->text, &RichEditor::setFontPointSize);
	connect(ui->bold, &QPushButton::clicked,
			ui->text, &RichEditor::setFontBold);
	connect(ui->underLine, &QPushButton::clicked,
			ui->text, &RichEditor::setFontUnderline);
	connect(ui->italic, &QPushButton::clicked,
			ui->text, &RichEditor::setFontItalic);
	connect(ui->textColor, &QPushButton::clicked,
			this, &TextEditor::setTextColor);
	connect(ui->textBgColor, &QPushButton::clicked,
			this, &TextEditor::setTextBackgroundColor);
	connect(ui->left, &QPushButton::toggled, [this](bool checked) {
		if (checked) ui->text->setAlignment(Qt::AlignLeft);
	});
	connect(ui->center, &QPushButton::toggled, [this](bool checked) {
		if (checked) ui->text->setAlignment(Qt::AlignCenter);
	});
	connect(ui->right, &QPushButton::toggled, [this](bool checked) {
		if (checked) ui->text->setAlignment(Qt::AlignRight);
	});
	connect(ui->justify, &QPushButton::toggled, [this](bool checked) {
		if (checked) ui->text->setAlignment(Qt::AlignJustify);
	});
	connect(ui->letterSpace, static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
			ui->text, &RichEditor::setLetterSpace);
	connect(ui->lineSpace, static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
			ui->text, &RichEditor::setLineSpace);
	connect(ui->paragraphSpace, static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
			ui->text, &RichEditor::setParagraphSpace);

	ui->text->resize(size);
	if (!item->html.isEmpty()) {
		ui->text->setHtml(item->html);
		ui->letterSpace->setValue(item->letterSpace);
		ui->lineSpace->setValue(item->lineSpace);
		ui->paragraphSpace->setValue(item->paragraphSpace);
		QTimer::singleShot(0, this, &TextEditor::textChanged);
	}
	ui->text->setFocus();

	connect(ui->ok, &QPushButton::clicked,
			this, &TextEditor::accept);
	connect(ui->cancel, &QPushButton::clicked,
			this, &TextEditor::reject);

}

TextEditor::~TextEditor()
{
	delete ui;
}

void TextEditor::currentCharFormatChanged(const QTextCharFormat &format) {
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

void TextEditor::cursorPositionChanged() {
	Qt::Alignment align = ui->text->alignment();
	if (align & Qt::AlignLeft)
		ui->left->setChecked(true);
	else if (align & Qt::AlignHCenter)
		ui->center->setChecked(true);
	else if (align & Qt::AlignRight)
		ui->right->setChecked(true);
	else if (align & Qt::AlignJustify)
		ui->justify->setChecked(true);
}

void TextEditor::setTextBackgroundColor()
{
	QColor color = QColorDialog::getColor(Qt::black, this,
										  tr("Select Text Background Color"),
										  QColorDialog::ShowAlphaChannel);
	if (!color.isValid()) return;
	ui->text->setTextBackgroundColor(color);
}

void TextEditor::setTextColor()
{
	QColor color = QColorDialog::getColor(Qt::white, this,
										  tr("Select Text Color"),
										  QColorDialog::ShowAlphaChannel);
	if (!color.isValid()) return;
	ui->text->setTextColor(color);
}

void TextEditor::textChanged() {
	QSize docSize = ui->text->document()->size().toSize();

	if (docSize.height() > size.height())
		ui->text->resize(size.width(), docSize.height());
	else
		ui->text->resize(size);
}

void TextEditor::accept()
{
	item->html = ui->text->toHtml();
	item->letterSpace = ui->letterSpace->value();
	item->lineSpace = ui->lineSpace->value();
	item->paragraphSpace = ui->paragraphSpace->value();

	QDialog::accept();
}

