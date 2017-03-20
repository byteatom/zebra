#ifndef SEND_H
#define SEND_H

#include "IPlugin.h"
#include "IInit.h"
#include "ICommand.h"
#include "IMainWnd.h"
#include "ISetting.h"

class Feedback : public QObject, public IPlugin, public IInit, public ICommand
{
	Q_OBJECT

	virtual ~Feedback() = default;

	virtual void init(const std::list<IPlugin*>& plugins) override;
	virtual Commands commands() override;

	void show();

	IMainWnd* mainWnd = nullptr;
public:
	static ISetting* setting;
};

#endif // SEND_H
