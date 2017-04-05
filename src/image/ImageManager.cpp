#include "ImageManager.h"

#include "Image.h"
#include "ISetting.h"
#include "IMainWnd.h"
#include "IFadeFactory.h"
#include "IBorderFactory.h"
#include "IGlowFactory.h"

DEF_PLUGIN_CREATOR(ImageManager)

ISetting* setting = nullptr;
IMainWnd* mainWnd = nullptr;
IFadeFactory* fadeFactory = nullptr;
IBorderFactory* borderFactory = nullptr;
IGlowFactory* glowFactory = nullptr;

Commands ImageManager::commands()
{
	Commands cmds;

	Command cmdNew;
	cmdNew.menuBarIndex = MENU_ADD;
	cmdNew.menuIndex = MENU_ADD_IMAGE;
	cmdNew.toolBarIndex = TOOL_IMAGE;
	cmdNew.action = new QAction(NodeAttr::typeName(type()), nullptr);
	cmdNew.action->setIcon(QIcon(QPixmap("image/icon/image_64.png")));
	connect(cmdNew.action, &QAction::triggered,
			this, static_cast<void(ImageManager::*)()>(&ImageManager::create));
	cmds.push_back(cmdNew);

	return cmds;
}

void ImageManager::init(const std::list<IPlugin*>& plugins)
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

INode::Type ImageManager::type()
{
	return INode::Type::IMAGE;
}

void ImageManager::create()
{
	INodeBox* box = mainWnd->currentNodeBox(type());
	Image* node = new Image(box, nullptr);
	mainWnd->addNode(node);
	mainWnd->setCurrentNode(node);
}

INode* ImageManager::create(INodeBox* box, Json& jnode)
{
	Image* node = new Image(box, &jnode);
	mainWnd->addNode(node);
	return node;
}
