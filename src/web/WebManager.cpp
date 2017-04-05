#include "WebManager.h"

#include <QWebEngineProfile>
#include <QWebEngineSettings>

#include "Web.h"
#include "IMainWnd.h"
#include "IFadeFactory.h"
#include "IBorderFactory.h"
#include "IGlowFactory.h"

DEF_PLUGIN_CREATOR(WebManager)

ISetting* setting = nullptr;
IMainWnd* mainWnd = nullptr;
extern IBorderFactory* borderFactory = nullptr;
extern IGlowFactory* glowFactory = nullptr;

Commands WebManager::commands()
{
	Commands cmds;

	Command cmdNew;
	cmdNew.menuBarIndex = MENU_ADD;
	cmdNew.menuIndex = MENU_ADD_WEB;
	cmdNew.toolBarIndex = TOOL_WEB;
	cmdNew.action = new QAction(NodeAttr::typeName(type()), nullptr);
	cmdNew.action->setIcon(QIcon(QPixmap("image/icon/web_64.png")));
	connect(cmdNew.action, &QAction::triggered,
			this, static_cast<void(WebManager::*)()>(&WebManager::create));
	cmds.push_back(cmdNew);

	return cmds;
}

void WebManager::init(const std::list<IPlugin*>& plugins)
{
	for (auto plugin: plugins) {
		if (!setting) setting = dynamic_cast<ISetting *>(plugin);
		if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
		if (setting && mainWnd) break;
	}

	QWebEngineProfile* profile = QWebEngineProfile::defaultProfile();
	profile->setCachePath("data/web");
	profile->setPersistentStoragePath("data/web/Storage");
	QWebEngineSettings* set = profile->settings();
	set->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
	//set->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
	//set->setAttribute(QWebEngineSettings::AllowRunningInsecureContent, true);
}

INode::Type WebManager::type()
{
	return INode::Type::WEB;
}

void WebManager::create()
{
	INodeBox* box = mainWnd->currentNodeBox(type());
	Web* node = new Web(box, nullptr);
	mainWnd->addNode(node);
	mainWnd->setCurrentNode(node);
}

INode* WebManager::create(INodeBox* box, Json& jnode)
{
	Web* node = new Web(box, &jnode);
	mainWnd->addNode(node);
	return node;
}
