#ifndef RICHEDITOR_H
#define RICHEDITOR_H

#include <QTextEdit>
#include <QPixmap>
#include <QPainter>
#include <QTextBlock>
#include <QTextCursor>

class RichEditor : public QTextEdit
{
public:
	explicit RichEditor(QWidget *parent = nullptr) : QTextEdit{parent}
	{
		setObjectName("RichEditor");

		setFrameShape(QFrame::StyledPanel);
		setFrameShadow(QFrame::Plain);

		document()->setDocumentMargin(0);

		/*QPalette pal(viewport()->palette());
		pal.setColor(viewport()->backgroundRole(), Qt::black);
		viewport()->setPalette(pal);*/
		setStyleSheet("QTextEdit {background-color: black;}");

		setDefaultTextColor();
		connect(this, &RichEditor::textChanged, [this](){
			if (document()->isEmpty()) {
				setDefaultTextColor();
				setCurrentCharFormat(headCharFormat);
			}
		});
		connect(this, &RichEditor::currentCharFormatChanged, [this](const QTextCharFormat& format){
			if (!document()->isEmpty()) {
				headCharFormat = format;
			}
		});

		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setUndoRedoEnabled(true);
		setCursorWidth(2);
	}

	virtual ~RichEditor() = default;

	void setBackgroundColor(const QColor& color)
	{
		setStyleSheet(QString("QTextEdit {background-color: rgba(%1, %2, %3, %4);}")
					  .arg(color.red())
					  .arg(color.green())
					  .arg(color.blue())
					  .arg(color.alphaF()));
		/*setStyleSheet(QString("QTextEdit {background-color: rgb(%1, %2, %3);}")
							  .arg(color.red())
							  .arg(color.green())
							  .arg(color.blue()));*/
	}

	void setDefaultTextColor()
	{
		//setStyleSheet("color: white");	// default cursor color
		//document()->setDefaultStyleSheet("color: white");
		QTextEdit::setTextColor(Qt::red);		  // default text color for drawContents
		QPalette pal(palette());
		pal.setColor(QPalette::Text, Qt::red);
		setPalette(pal);
	}

	static QPixmap snapshot(QTextDocument* doc)
	{
		//doc->adjustSize();
		QPixmap snapshot(doc->size().toSize());
		snapshot.fill(Qt::transparent);
		QPainter painter(&snapshot);
		painter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing, false);
		doc->drawContents(&painter);
		return snapshot;
	}

	QPixmap snapshot()
	{
		return snapshot(document());
	}

	void setFontFamily(const QFont &font)
	{
		QTextEdit::setFontFamily(font.family());
	}

	static void setDocLetterSpace(QTextDocument* doc, qreal space)
	{
		for (QTextBlock block = doc->begin();
			 block != doc->end();
			 block = block.next())
		{
			QTextCursor cursor{block};
			cursor.select(QTextCursor::BlockUnderCursor);

			QTextCharFormat format = block.charFormat();
			format.setFontLetterSpacingType(QFont::AbsoluteSpacing);
			format.setFontLetterSpacing(space);

			cursor.mergeCharFormat(format);
		}
	}

	void setLetterSpace(qreal space)
	{
		setDocLetterSpace(document(), space);
	}

	static void setDocLineSpace(QTextDocument* doc, qreal space)
	{
		for (QTextBlock block = doc->begin();
			 block != doc->end();
			 block = block.next())
		{
			QTextCursor cursor{block};

			QTextBlockFormat format = block.blockFormat();
			format.setLineHeight(space, QTextBlockFormat::LineDistanceHeight);

			cursor.setBlockFormat(format);
		}
	}

	void setLineSpace(qreal space)
	{
		setDocLineSpace(document(), space);
	}

	static void setDocParagraphSpace(QTextDocument* doc, qreal space)
	{
		for (QTextBlock block = doc->begin();
			 block != doc->end();
			 block = block.next())
		{
			QTextCursor cursor{block};

			QTextBlockFormat format = block.blockFormat();
			format.setBottomMargin(space + 4);

			cursor.setBlockFormat(format);
		}
	}

	void setParagraphSpace(qreal space)
	{
		setDocParagraphSpace(document(), space);
	}

	void setFontBold(bool checked)
	{
		setFontWeight(checked ? QFont::Bold : QFont::Normal);
	}

protected:
	/*
	 * QTextEdit's mouse event directly called from viewport,
	 * not even pass through self's event filter,
	 * so override is the only method.
	*/
	/*virtual void mouseDoubleClickEvent(QMouseEvent *event) override
	{
		setReadOnly(!isReadOnly());
		setMouseTracking(isReadOnly());	// for edge cursor
		if (isReadOnly()) {
			viewport()->unsetCursor();	// use parent's cursor
		} else {
			viewport()->setCursor(Qt::IBeamCursor);	// QTextCfger defualt cursor
		}
		event->accept();
	}

	virtual void mousePressEvent(QMouseEvent *event) override
	{
		QTextEdit::mousePressEvent(event);
		event->setModifiers(Qt::MetaModifier);	// for MouseSelector
		event->ignore();
	}

	virtual void mouseMoveEvent(QMouseEvent *event) override
	{
		if (isReadOnly())
			event->ignore();	// pass throuth to parent region
		else {
			QTextEdit::mouseMoveEvent(event);
			// event->accept();
		}
	}*/
private:
	QTextCharFormat headCharFormat;
};

#endif // RICHEDITOR_H
