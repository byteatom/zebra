#ifndef COMMCFGER_H
#define COMMCFGER_H

#include <QDialog>

#include "IProtocol.h"

namespace Ui {
class CommCfger;
}

class CommCfger : public QDialog
{
	Q_OBJECT

public:
	explicit CommCfger(QWidget *parent, IProtocol* protocol);
	~CommCfger();

private:
	Ui::CommCfger *ui;
	IProtocol* protocol;
	IProtocol::TypeNames typeNames;
	QWidget* cfger = nullptr;

	void showCfger();
	void typeChanged(int index);
	void test();
	virtual void done(int r) override;
};

#endif // COMMCFGER_H
