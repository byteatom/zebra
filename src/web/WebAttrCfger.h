#ifndef WEBATTRCFGER_H
#define WEBATTRCFGER_H

#include <QGroupBox>

#include "WebAttr.h"

namespace Ui {
class WebAttrCfger;
}

class WebAttrCfger : public QGroupBox
{
	Q_OBJECT
public:
	explicit WebAttrCfger(QWidget *parent, WebAttr *attr);
	virtual ~WebAttrCfger();

private:
	Ui::WebAttrCfger* ui;
	WebAttr* attr = nullptr;

	void edit();
};

#endif // WEBATTRCFGER_H
