#ifndef SINGLELINEEDITOR_H
#define SINGLELINEEDITOR_H

#include <QKeyEvent>
#include <QMimeData>

#include "RichEditor.h"

class LineEditor : public RichEditor
{
public:
	explicit LineEditor(QWidget *parent = nullptr):
		RichEditor{parent}
	{
		setLineWrapMode(NoWrap);
	}

	virtual ~LineEditor() = default;

	void setAlignment(Qt::Alignment align)
	{
		setLineWrapMode(QTextEdit::WidgetWidth);
		RichEditor::setAlignment(align);
		setLineWrapMode(QTextEdit::NoWrap);
	}

protected:
	virtual void keyPressEvent(QKeyEvent *event) override
	{
		if (event->key() == Qt::Key_Return) return;
		RichEditor::keyPressEvent(event);
	}

	virtual void insertFromMimeData(const QMimeData *source) override
	{
		if (!source->hasText()) return;

		QString text = source->text();
		text.remove('\r');
		text.remove('\n');
		insertPlainText(text);
	}
};

#endif // SINGLELINEEDITOR_H
