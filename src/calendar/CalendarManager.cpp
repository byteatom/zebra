#include "CalendarManager.h"

#include "Calendar.h"
#include "ISetting.h"
#include "IMainWnd.h"
#include "IBorderFactory.h"
#include "IGlowFactory.h"

DEF_PLUGIN_CREATOR(CalendarManager)

ISetting* setting = nullptr;
IMainWnd* mainWnd = nullptr;
IBorderFactory* borderFactory = nullptr;
IGlowFactory* glowFactory = nullptr;

Commands CalendarManager::commands()
{
	Commands cmds;

	Command cmdNew;
	cmdNew.menuBarIndex = MENU_ADD;;
	cmdNew.menuIndex = MENU_ADD_CALENDAR;
	cmdNew.toolBarIndex = TOOL_CALENDAR;
	cmdNew.action = new QAction(tr("Calendar"), nullptr);
	cmdNew.action->setIcon(QIcon(QPixmap("image/icon/calendar_64.png")));
	connect(cmdNew.action, &QAction::triggered,
			this, static_cast<void(CalendarManager::*)()>(&CalendarManager::create));
	cmds.push_back(cmdNew);

	return cmds;
}

void CalendarManager::init(const std::list<IPlugin*>& plugins)
{
	for (auto plugin: plugins) {
		if (!setting) setting = dynamic_cast<ISetting *>(plugin);
		if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
		if (!borderFactory) borderFactory = dynamic_cast<IBorderFactory *>(plugin);
		if (!glowFactory) glowFactory = dynamic_cast<IGlowFactory *>(plugin);
		if (setting && mainWnd && borderFactory && glowFactory) break;
	}
}

INode::Type CalendarManager::type()
{
	return INode::Type::CALENDAR;
}

void CalendarManager::create()
{
	INodeBox* box = mainWnd->currentNodeBox(type());
	Calendar* node = new Calendar(box, nullptr);
	mainWnd->addNode(node);
	mainWnd->setCurrentNode(node);
}

INode* CalendarManager::create(INodeBox* box, Json& jnode)
{
	Calendar* node = new Calendar(box, &jnode);
	mainWnd->addNode(node);
	return node;
}
