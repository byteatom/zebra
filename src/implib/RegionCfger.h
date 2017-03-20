#ifndef REGIONCFGER_H
#define REGIONCFGER_H

#include <QTextEdit>
#include <QTextCharFormat>
#include <QButtonGroup>

#include "Cfger.h"
#include "RegionAttr.h"

namespace Ui {
class RegionCfger;
}

class RegionCfger : public Cfger
{
	Q_OBJECT
public:
	explicit RegionCfger(RegionAttr *attr);
	virtual ~RegionCfger();

protected:
	Ui::RegionCfger *ui;
	RegionAttr *attr = nullptr;

	void appendBorder();
	void appendGlow();
};

#endif // REGIONCFGER_H
