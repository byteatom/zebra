#include "Comm.h"

#include "IScreen.h"

#include "CommCfger.h"

DEF_PLUGIN_CREATOR(Comm)

void Comm::init(const std::list<IPlugin*>& plugins)
{
	for (auto plugin: plugins) {
		if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
		if (mainWnd) break;
	}
}

Commands Comm::commands()
{
	Commands cmds;

	Command cmdComm;
	cmdComm.menuBarIndex = MENU_SET;
	cmdComm.menuIndex = MENU_SET_COMM_PARA;
	cmdComm.action = new QAction(tr("Communication Parameters"), nullptr);
	cmdComm.action->setShortcut(Qt::CTRL + Qt::Key_C);
	cmdComm.action->setIcon(QIcon(QPixmap("image/icon/comm_64.png")));
	connect(cmdComm.action, &QAction::triggered,
			this, &Comm::cfg);
	cmds.push_back(cmdComm);

	return cmds;
}

void Comm::cfg()
{
	INode* node = mainWnd->currentNode(INode::Type::SCREEN);
	IScreen* screen = dynamic_cast<IScreen*>(node);
	if (!screen) return;
	CommCfger cfger{mainWnd->widgetAsMainWnd(), screen->protocol()};
	cfger.exec();
}
