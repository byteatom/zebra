#ifndef FLASHTEXTATTRCFGER_H
#define FLASHTEXTATTRCFGER_H

#include <QGroupBox>

#include "FlashTextAttr.h"

namespace Ui {
class FlashTextAttrCfger;
}

class FlashTextAttrCfger : public QGroupBox
{
	Q_OBJECT
public:
	explicit FlashTextAttrCfger(QWidget *parent, FlashTextAttr *attr);
	~FlashTextAttrCfger();
	void textChanged();
	int updataFontFile();

private:
	FlashTextAttr* attr = nullptr;
	Ui::FlashTextAttrCfger* ui;
};

#endif // FLASHTEXTATTRCFGER_H
