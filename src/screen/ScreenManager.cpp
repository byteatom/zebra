#include "ScreenManager.h"

#include <boost/filesystem/fstream.hpp>
namespace bfs = boost::filesystem;

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

#include "Screen.h"
#include "ICoreJson.h"

DEF_PLUGIN_CREATOR(ScreenManager)

int zoomFactors[] = {1, 4, 6, 8, 10, 12};

void ScreenManager::init(const std::list<IPlugin*>& plugins)
{
	for (auto plugin: plugins) {
		if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
		if (!core) core = dynamic_cast<ICore *>(plugin);
		if (!nodeFactory) {
			INodeFactory* factory = dynamic_cast<INodeFactory *>(plugin);
			if (factory && factory->type() == type() + 1)
				nodeFactory = factory;
		}
		if (!protocolFactory) protocolFactory = dynamic_cast<IProtocolFactory *>(plugin);
		if (mainWnd && core && nodeFactory && protocolFactory) break;
	}
}

Commands ScreenManager::commands()
{
	Commands cmds;

	Command cmdNew;
	cmdNew.menuBarIndex = MENU_ADD;
	cmdNew.menuIndex = MENU_ADD_SCREEN;
	cmdNew.action = new QAction(NodeAttr::typeName(type()), nullptr);
	cmdNew.action->setIcon(QIcon(QPixmap("image/icon/screen_16.png")));
	connect(cmdNew.action, &QAction::triggered,
			this, static_cast<void(ScreenManager::*)()>(&ScreenManager::create));
	cmds.push_back(cmdNew);

	return cmds;
}

INode::Type ScreenManager::type()
{
	return INode::Type::SCREEN;
}

void ScreenManager::create()
{
	INodeBox* box = mainWnd->currentNodeBox(type());
	Screen* node = new Screen(box, nullptr, core,
				protocolFactory ? protocolFactory->create(nullptr) : nullptr);
	mainWnd->addNode(node);
	mainWnd->setCurrentNode(node);
}

INode* ScreenManager::create(INodeBox* box, Json& jnode)
{
	Screen* node = new Screen(box, &jnode, core,
				protocolFactory ? protocolFactory->create(&jnode) : nullptr);
	mainWnd->addNode(node);
	Json jnodes = jnode["~programs"];
	for (Json::iterator jit = jnodes.begin(); jit != jnodes.end(); ++jit) {
		nodeFactory->create(node, *jit);
	}
	return node;
}


