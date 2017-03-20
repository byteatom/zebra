#ifndef ABOUT_H
#define ABOUT_H

#include <QActionGroup>

#include "IPlugin.h"
#include "IInit.h"
#include "ICommand.h"
#include "ISetting.h"
#include "IMainWnd.h"

class Lang : public QObject, public IPlugin, public IInit, public ICommand
{
	Q_OBJECT

	virtual ~Lang() = default;

	virtual Priority priority() override;
	virtual void init(const std::list<IPlugin*>& plugins) override;
	virtual Commands commands() override;

	ISetting* setting = nullptr;
	IMainWnd* mainWnd = nullptr;
	QActionGroup group{nullptr};
};

#endif // ABOUT_H
