#ifndef COMM_H
#define COMM_H

#include <vector>

#include <QObject>

#include "IPlugin.h"
#include "IInit.h"
#include "ICommand.h"
#include "IMainWnd.h"

class  Comm : public QObject, public IPlugin, public IInit, public ICommand
{
	Q_OBJECT

	virtual ~Comm() = default;

	virtual void init(const std::list<IPlugin*>& plugins) override;
	virtual Commands commands() override;

	void cfg();

	IMainWnd* mainWnd = nullptr;
};

#endif // SCREENCOMM_H
