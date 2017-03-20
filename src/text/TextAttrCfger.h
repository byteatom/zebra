#ifndef TEXTATTRCFGER_H
#define TEXTATTRCFGER_H

#include <QGroupBox>
#include <QButtonGroup>
#include <QTextCharFormat>
#include <QTextEdit>

#include "TextAttr.h"

namespace Ui {
class TextAttrCfger;
}

class TextAttrCfger : public QGroupBox
{
	Q_OBJECT
public:
	explicit TextAttrCfger(QWidget *parent, TextAttr *attr);
	~TextAttrCfger();

	void currentCharFormatChanged(const QTextCharFormat &format);
	void setBgColor();
	void setTextBackgroundColor();
	void setTextColor();
	void setLetterSpace(qreal value);
	void strechHor(int value);
	void strechVer(int value);
	void offset(int value);
	void cfgChanged();
private:
	Ui::TextAttrCfger* ui;
	QButtonGroup alignBtns;
	TextAttr* attr = nullptr;
};

#endif // TEXTATTRCFGER_H
