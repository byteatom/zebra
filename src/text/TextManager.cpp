#include "TextManager.h"

#include "Text.h"
#include "ISetting.h"
#include "IMainWnd.h"
#include "IFadeFactory.h"
#include "IBorderFactory.h"
#include "IGlowFactory.h"

DEF_PLUGIN_CREATOR(TextManager)

ISetting* setting = nullptr;
IMainWnd* mainWnd = nullptr;
IFadeFactory* fadeFactory = nullptr;
IBorderFactory* borderFactory = nullptr;
IGlowFactory* glowFactory = nullptr;

Commands TextManager::commands()
{
	Commands cmds;

	Command cmdNew;
	cmdNew.menuBarIndex = MENU_ADD;
	cmdNew.menuIndex = MENU_ADD_TEXT;
	cmdNew.toolBarIndex = TOOL_TEXT;
	cmdNew.action = new QAction(NodeAttr::typeName(type()), nullptr);
	cmdNew.action->setIcon(QIcon(QPixmap("image/icon/text_64.png")));
	connect(cmdNew.action, &QAction::triggered,
			this, static_cast<void(TextManager::*)()>(&TextManager::create));
	cmds.push_back(cmdNew);

	return cmds;
}

void TextManager::init(const std::list<IPlugin*>& plugins)
{
	for (auto plugin: plugins) {
		if (!setting) setting = dynamic_cast<ISetting *>(plugin);
		if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
		if (!fadeFactory) fadeFactory = dynamic_cast<IFadeFactory *>(plugin);
		if (!borderFactory) borderFactory = dynamic_cast<IBorderFactory *>(plugin);
		if (!glowFactory) glowFactory = dynamic_cast<IGlowFactory *>(plugin);
		if (setting && mainWnd && fadeFactory && borderFactory && glowFactory) break;
	}
}

INode::Type TextManager::type()
{
	return INode::Type::TEXT;
}

void TextManager::create()
{
	INodeBox* box = mainWnd->currentNodeBox(type());
	Text* node = new Text(box, nullptr);
	mainWnd->addNode(node);
	mainWnd->setCurrentNode(node);
}

INode* TextManager::create(INodeBox* box, Json& jnode)
{
	Text* node = new Text(box, &jnode);
	mainWnd->addNode(node);
	return node;
}
