#ifndef ABOUT_H
#define ABOUT_H

#include "IPlugin.h"
#include "IInit.h"
#include "ISetting.h"
#include "ICommand.h"

class Update : public QObject, public IPlugin, public IInit, public ICommand
{
	Q_OBJECT

	virtual ~Update();

	virtual void init(const std::list<IPlugin*>& plugins) override;
	virtual Commands commands() override;

	ISetting* setting = nullptr;
};

#endif // ABOUT_H
