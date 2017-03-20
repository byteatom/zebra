#ifndef ABOUT_H
#define ABOUT_H

#include "IPlugin.h"
#include "IInit.h"
#include "ICommand.h"
#include "IMainWnd.h"

class About : public QObject, public IPlugin, public IInit, public ICommand
{
	Q_OBJECT

    virtual ~About() = default;

	virtual void init(const std::list<IPlugin*>& plugins) override;
	virtual Commands commands() override;

	IMainWnd* mainWnd = nullptr;
};

#endif // ABOUT_H
