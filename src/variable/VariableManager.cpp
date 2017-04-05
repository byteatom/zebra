#include "VariableManager.h"

#include "Variable.h"
#include "IMainWnd.h"
#include "IBorderFactory.h"
#include "IGlowFactory.h"

DEF_PLUGIN_CREATOR(VariableManager)

IMainWnd* mainWnd = nullptr;
IBorderFactory* borderFactory = nullptr;
IGlowFactory* glowFactory = nullptr;

Commands VariableManager::commands()
{
	Commands cmds;

	Command cmdNew;
	cmdNew.menuBarIndex = MENU_ADD;
	cmdNew.menuIndex = MENU_ADD_VARIABLE;
	cmdNew.action = new QAction(NodeAttr::typeName(type()), nullptr);
	cmdNew.action->setIcon(QIcon(QPixmap("image/icon/variable_64.png")));
	connect(cmdNew.action, &QAction::triggered,
			this, static_cast<void(VariableManager::*)()>(&VariableManager::create));
	cmds.push_back(cmdNew);

	return cmds;
}

void VariableManager::init(const std::list<IPlugin*>& plugins)
{
	for (auto plugin: plugins) {
		if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
		if (!borderFactory) borderFactory = dynamic_cast<IBorderFactory *>(plugin);
		if (!glowFactory) glowFactory = dynamic_cast<IGlowFactory *>(plugin);
		if (mainWnd && borderFactory && glowFactory) break;
	}
}

INode::Type VariableManager::type()
{
	return INode::Type::VARIABLE;
}

void VariableManager::create()
{
	INodeBox* box = mainWnd->currentNodeBox(type());
	Variable* node = new Variable(box, nullptr);
	mainWnd->addNode(node);
	mainWnd->setCurrentNode(node);
}

INode* VariableManager::create(INodeBox* box, Json& jnode)
{
	Variable* node = new Variable(box, &jnode);
	mainWnd->addNode(node);
	return node;
}
