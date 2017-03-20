#include "OnOffManager.h"
#include "OnOffDlg.h"

DEF_PLUGIN_CREATOR(OnOffManager)

Commands OnOffManager::commands()
{
	Commands cmds;

	Command cmdOnOff;
	cmdOnOff.menuBarIndex = MENU_TEST;
	cmdOnOff.menuIndex = MENU_TEST_ONOFF;
	cmdOnOff.toolBarIndex = TOOL_ONOFF;
	cmdOnOff.action = new QAction(tr("Open/Close Screen"), nullptr);
	cmdOnOff.action->setShortcut(Qt::CTRL + Qt::Key_P);
	cmdOnOff.action->setIcon(QIcon(QPixmap("image/icon/onoff_48.png")));
	connect(cmdOnOff.action, &QAction::triggered,
			[this](){ OnOffDlg dlg(mainWnd, mainWnd->widgetAsMainWnd()); dlg.exec(); });
	cmds.push_back(cmdOnOff);

	return cmds;
}

void OnOffManager::init(const std::list<IPlugin*> &plugins)
{
	for (auto plugin: plugins) {
		if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
		if (mainWnd) break;
	}
}
