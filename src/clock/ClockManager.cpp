#include "ClockManager.h"

#include "Clock.h"
#include "ISetting.h"
#include "IMainWnd.h"
#include "IBorderFactory.h"
#include "IGlowFactory.h"

DEF_PLUGIN_CREATOR(ClockManager)

ISetting* setting = nullptr;
IMainWnd* mainWnd = nullptr;
IBorderFactory* borderFactory = nullptr;
IGlowFactory* glowFactory = nullptr;

Commands ClockManager::commands()
{
	Commands cmds;

	Command cmdNew;
	cmdNew.menuBarIndex = MENU_ADD;;
	cmdNew.menuIndex = MENU_ADD_CLOCK;
	cmdNew.toolBarIndex = TOOL_CLOCK;
	cmdNew.action = new QAction(NodeAttr::typeName(type()), nullptr);
	cmdNew.action->setIcon(QIcon(QPixmap("image/icon/clock_64.png")));
	connect(cmdNew.action, &QAction::triggered,
			this, static_cast<void(ClockManager::*)()>(&ClockManager::create));
	cmds.push_back(cmdNew);

	return cmds;
}

void ClockManager::init(const std::list<IPlugin*> &plugins)
{
	for (auto plugin: plugins) {
		if (!setting) setting = dynamic_cast<ISetting *>(plugin);
		if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
		if (!borderFactory) borderFactory = dynamic_cast<IBorderFactory *>(plugin);
		if (!glowFactory) glowFactory = dynamic_cast<IGlowFactory *>(plugin);
		if (setting && mainWnd && borderFactory && glowFactory) break;
	}
}

INode::Type ClockManager::type()
{
	return INode::CLOCK;
}

void ClockManager::create()
{
	INodeBox* box = mainWnd->currentNodeBox(type());
	Clock* node = new Clock(box, nullptr);
	mainWnd->addNode(node);
	mainWnd->setCurrentNode(node);
}

INode* ClockManager::create(INodeBox* box, Json& jnode)
{
	Clock* node = new Clock(box, &jnode);
	mainWnd->addNode(node);
	return node;
}
