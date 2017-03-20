#include "TimerManager.h"

#include "Timer.h"
#include "ISetting.h"
#include "IMainWnd.h"
#include "IBorderFactory.h"
#include "IGlowFactory.h"

DEF_PLUGIN_CREATOR(TimerManager)

ISetting* setting = nullptr;
IMainWnd* mainWnd = nullptr;
IBorderFactory* borderFactory = nullptr;
IGlowFactory* glowFactory = nullptr;

Commands TimerManager::commands()
{
	Commands cmds;

	Command cmdNew;
	cmdNew.menuBarIndex = MENU_ADD;
	cmdNew.menuIndex = MENU_ADD_TIMER;
	cmdNew.toolBarIndex = TOOL_TIMER;
	cmdNew.action = new QAction(tr("Timer"), nullptr);
	cmdNew.action->setIcon(QIcon(QPixmap("image/icon/timer_64.png")));
	connect(cmdNew.action, &QAction::triggered,
			this, static_cast<void(TimerManager::*)()>(&TimerManager::create));
	cmds.push_back(cmdNew);

	return cmds;
}

void TimerManager::init(const std::list<IPlugin*>& plugins)
{
	for (auto plugin: plugins) {
		if (!setting) setting = dynamic_cast<ISetting *>(plugin);
		if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
		if (!borderFactory) borderFactory = dynamic_cast<IBorderFactory *>(plugin);
		if (!glowFactory) glowFactory = dynamic_cast<IGlowFactory *>(plugin);
		if (setting && mainWnd && borderFactory && glowFactory) break;
	}
}

INode::Type TimerManager::type()
{
	return INode::Type::TIMER;
}

void TimerManager::create()
{
	INodeBox* box = mainWnd->currentNodeBox(type());
	Timer* node = new Timer(box, nullptr);
	mainWnd->addNode(node);
	mainWnd->setCurrentNode(node);
}

INode* TimerManager::create(INodeBox* box, Json& jnode)
{
	Timer* node = new Timer(box, &jnode);
	mainWnd->addNode(node);
	return node;
}
