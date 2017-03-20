#ifndef VARIABLEATTRCFGER_H
#define VARIABLEATTRCFGER_H

#include <QGroupBox>

#include "VariableAttr.h"

namespace Ui {
class VariableAttrCfger;
}

class VariableAttrCfger : public QGroupBox
{
	Q_OBJECT
public:
	explicit VariableAttrCfger(QWidget *parent, VariableAttr *attr);
	~VariableAttrCfger();

	void textChanged();
private:
	Ui::VariableAttrCfger* ui;
	VariableAttr* attr = nullptr;
};

#endif // VARIABLEATTRCFGER_H
