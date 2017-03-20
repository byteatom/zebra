#include "SyncTimeManager.h"
#include "SyncTimeDlg.h"

DEF_PLUGIN_CREATOR(SyncTimeManager)

Commands SyncTimeManager::commands()
{
	Commands cmds;

	Command cmdTime;
	cmdTime.menuBarIndex = MENU_TEST;
	cmdTime.menuIndex = MENU_TEST_TIME;
	cmdTime.toolBarIndex = TOOL_TIME;
	cmdTime.action = new QAction(tr("Synchronize Time"), nullptr);
	cmdTime.action->setShortcut(Qt::CTRL + Qt::Key_T);
	cmdTime.action->setIcon(QIcon(QPixmap("image/icon/sync_time_48.png")));
	connect(cmdTime.action, &QAction::triggered,
			[this](){ SyncTimeDlg dlg(mainWnd, mainWnd->widgetAsMainWnd()); dlg.exec(); });
	cmds.push_back(cmdTime);

	return cmds;
}

void SyncTimeManager::init(const std::list<IPlugin*> &plugins)
{
	for (auto plugin: plugins) {
		if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
		if (mainWnd) break;
	}
}
