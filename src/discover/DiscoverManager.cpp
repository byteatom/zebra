#include "DiscoverManager.h"

#include <QProcess>
#include <QCoreApplication>

#include "DiscoverDlg.h"

DEF_PLUGIN_CREATOR(DiscoverManager)

Commands DiscoverManager::commands()
{
	Commands cmds;

	Command cmdNetPara;
	cmdNetPara.menuBarIndex = MENU_SET;
	cmdNetPara.menuIndex = MENU_SET_NET_PARA;
	cmdNetPara.action = new QAction(tr("Network Parameters"), nullptr);
	cmdNetPara.action->setShortcut(Qt::CTRL + Qt::Key_N);
	cmdNetPara.action->setIcon(QIcon(QPixmap("image/icon/net_para_48.png")));
	connect(cmdNetPara.action, &QAction::triggered,
			[this](){
				QProcess::startDetached("bin/net_para.exe");
			});
	cmds.push_back(cmdNetPara);

	Command cmdDiscover;
	cmdDiscover.menuBarIndex = MENU_SET;
	cmdDiscover.menuIndex = MENU_SET_DISCOVER;
	cmdDiscover.toolBarIndex = TOOL_DISCOVER;
	cmdDiscover.action = new QAction(tr("Discover"), nullptr);
	cmdDiscover.action->setShortcut(Qt::CTRL + Qt::Key_F);
	cmdDiscover.action->setIcon(QIcon(QPixmap("image/icon/discover_128.png")));
	connect(cmdDiscover.action, &QAction::triggered,
			[this](){ DiscoverDlg dlg(mainWnd, mainWnd->widgetAsMainWnd()); dlg.exec(); });
	cmds.push_back(cmdDiscover);

	return cmds;
}

void DiscoverManager::init(const std::list<IPlugin*> &plugins)
{
	for (auto plugin: plugins) {
		if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
		if (mainWnd) break;
	}
}
