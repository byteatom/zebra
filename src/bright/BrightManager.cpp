#include "BrightManager.h"
#include "BrightDlg.h"

DEF_PLUGIN_CREATOR(BrightManager)

Commands BrightManager::commands()
{
	Commands cmds;

	Command cmdBright;
	cmdBright.menuBarIndex = MENU_TEST;
	cmdBright.menuIndex = MENU_TEST_BRIGHTNESS;
	cmdBright.toolBarIndex = TOOL_BRIGHT;
	cmdBright.action = new QAction(tr("Screen Brightness"), nullptr);
	cmdBright.action->setShortcut(Qt::CTRL + Qt::Key_B);
	cmdBright.action->setIcon(QIcon(QPixmap("image/icon/bright_48.png")));
	connect(cmdBright.action, &QAction::triggered,
			[this](){ BrightDlg dlg(mainWnd, mainWnd->widgetAsMainWnd()); dlg.exec(); });
	cmds.push_back(cmdBright);

	return cmds;
}

void BrightManager::init(const std::list<IPlugin*> &plugins)
{
	for (auto plugin : plugins) {
		if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
		if (mainWnd) break;
	}
}
