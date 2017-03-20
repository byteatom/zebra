#ifndef PROGRAMCFGER_H
#define PROGRAMCFGER_H

#include <QButtonGroup>

#include "ProgramAttr.h"
#include "Cfger.h"

namespace Ui {
class ProgramCfger;
}

class ProgramCfger : public Cfger
{
	Q_OBJECT
public:
	explicit ProgramCfger(ProgramAttr *attr);
	virtual ~ProgramCfger();

private:
	void getFixed();
	void setFixed();
	void getExternal();
	void setExternal();
	void setEveryday();

	Ui::ProgramCfger *ui = nullptr;
	QButtonGroup typeRadios;
	ProgramAttr *attr = nullptr;
};

#endif    // PROGRAMCFGER_H
