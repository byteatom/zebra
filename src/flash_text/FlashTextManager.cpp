#include "FlashTextManager.h"

#include "FlashText.h"

#include "ISetting.h"
#include "IMainWnd.h"
#include "IBorderFactory.h"
#include "IGlowFactory.h"

DEF_PLUGIN_CREATOR(FlashTextManager)

ISetting* setting = nullptr;
IMainWnd* mainWnd = nullptr;
IBorderFactory* borderFactory = nullptr;
IGlowFactory* glowFactory = nullptr;

Commands FlashTextManager::commands()
{
	Commands cmds;

	Command cmdNew;
	cmdNew.menuBarIndex = MENU_ADD;
	cmdNew.menuIndex = MENU_ADD_FLASH_TEXT;
	cmdNew.toolBarIndex = TOOL_FLASH_TEXT;
	cmdNew.action = new QAction(NodeAttr::typeName(type()), nullptr);
	cmdNew.action->setIcon(QIcon(QPixmap("image/icon/flash_text_64.png")));
	connect(cmdNew.action, &QAction::triggered,
			this, static_cast<void(FlashTextManager::*)()>(&FlashTextManager::create));
	cmds.push_back(cmdNew);

	return cmds;
}

void FlashTextManager::init(const std::list<IPlugin*> &plugins)
{
	for (auto plugin: plugins) {
		if (!setting) setting = dynamic_cast<ISetting *>(plugin);
		if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
		if (!borderFactory) borderFactory = dynamic_cast<IBorderFactory *>(plugin);
		if (!glowFactory) glowFactory = dynamic_cast<IGlowFactory *>(plugin);
		if (setting && mainWnd && borderFactory && glowFactory) break;
	}
}

INode::Type FlashTextManager::type()
{
	return INode::FLASH_TEXT;
}

void FlashTextManager::create()
{
	INodeBox* box = mainWnd->currentNodeBox(type());
	FlashText* node = new FlashText(box, nullptr);
	mainWnd->addNode(node);
	mainWnd->setCurrentNode(node);
}

INode* FlashTextManager::create(INodeBox* box, Json& jnode)
{
	FlashText* node = new FlashText(box, &jnode);
	mainWnd->addNode(node);
	return node;
}
