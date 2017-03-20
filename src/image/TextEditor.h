#ifndef IMAGETEXTEDITOR_H
#define IMAGETEXTEDITOR_H

#include <QDialog>
#include <QButtonGroup>
#include <QTextCharFormat>
#include <QTextEdit>

namespace Ui {
class TextEditor;
}

class TextItem;

class TextEditor : public QDialog
{
	Q_OBJECT

public:
	explicit TextEditor(QWidget *parent, QSize size, TextItem* item);
	~TextEditor();

private:
	void currentCharFormatChanged(const QTextCharFormat &format);
	void cursorPositionChanged();
	void setTextBackgroundColor();
	void setTextColor();
	void textChanged();
	virtual void accept() override;

	Ui::TextEditor *ui;
	QButtonGroup alignBtns;
	QSize size;
	TextItem* item;
};

#endif // IMAGETEXTEDITOR_H
