#include "ProgramManager.h"

#include "Program.h"

DEF_PLUGIN_CREATOR(ProgramManager)

Commands ProgramManager::commands()
{
	Commands cmds;

	Command cmdNew;
	cmdNew.menuBarIndex = MENU_ADD;
	cmdNew.menuIndex = MENU_ADD_PROGRAM;
	cmdNew.toolBarIndex = TOOL_PROGRAM;
	cmdNew.action = new QAction(tr("Program"), nullptr);
	cmdNew.action->setIcon(QIcon(QPixmap("image/icon/program_64.png")));
	connect(cmdNew.action, &QAction::triggered,
			this, static_cast<void(ProgramManager::*)()>(&ProgramManager::create));
	cmds.push_back(cmdNew);

	Command cmdSeparator;
	cmdSeparator.menuBarIndex = MENU_ADD;;
	cmdSeparator.menuIndex = MENU_ADD_SEPERATOR1;
	cmdSeparator.action = new QAction(nullptr);
	cmdSeparator.action->setSeparator(true);
	cmds.push_back(cmdSeparator);

	return cmds;
}

void ProgramManager::init(const std::list<IPlugin*>& plugins)
{
	for (auto plugin: plugins) {
		if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
		if (!backgroundFactory) backgroundFactory = dynamic_cast<IBackgroundFactory *>(plugin);
		if (!borderFactory) borderFactory = dynamic_cast<IBorderFactory *>(plugin);
		INodeFactory* factory = dynamic_cast<INodeFactory *>(plugin);
		if (factory && factory->type() > type())
			nodeFactories[factory->type()] = factory;
		if (mainWnd && borderFactory && backgroundFactory) break;
	}
}

INode::Type ProgramManager::type()
{
	return INode::Type::PROGRAM;
}

void ProgramManager::create()
{
	INodeBox* box = mainWnd->currentNodeBox(type());
	Program* node = new Program(box, nullptr,
								backgroundFactory ? backgroundFactory->create(nullptr) : nullptr,
								borderFactory ? borderFactory->create(nullptr) : nullptr);
	mainWnd->addNode(node);
	mainWnd->setCurrentNode(node);
}

INode* ProgramManager::create(INodeBox* box, Json& jnode)
{
	Program* node = new Program(box, &jnode,
								backgroundFactory ? backgroundFactory->create(&jnode) : nullptr,
								borderFactory ? borderFactory->create(&jnode) : nullptr);
	mainWnd->addNode(node);
	Json jnodes = jnode["~regions"];
	for (Json::iterator jit = jnodes.begin(); jit != jnodes.end(); ++jit) {
		nodeFactories[(*jit)["type"]]->create(node, *jit);
	}
	return node;
}
