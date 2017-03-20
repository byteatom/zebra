#include "ScreenAttrManager.h"
#include "ScreenAttrDlg.h"

DEF_PLUGIN_CREATOR(ScreenAttrManager)

Commands ScreenAttrManager::commands()
{
	Commands cmds;

	Command cmdScreenAttr;
	cmdScreenAttr.menuBarIndex = MENU_SET;
	cmdScreenAttr.menuIndex = MENU_SET_SCREEN_ATTR;
	cmdScreenAttr.action = new QAction(tr("Screen Parameters"), nullptr);
	cmdScreenAttr.action->setShortcut(Qt::CTRL + Qt::Key_A);
	cmdScreenAttr.action->setIcon(QIcon(QPixmap("image/icon/screen_attr_64.png")));
	connect(cmdScreenAttr.action, &QAction::triggered,
			[this](){ScreenAttrDlg dlg(mainWnd, setting, mainWnd->widgetAsMainWnd()); dlg.exec();});
	cmds.push_back(cmdScreenAttr);

	return cmds;
}

void ScreenAttrManager::init(const std::list<IPlugin*> &plugins)
{
	for (auto plugin: plugins) {
		if (!setting) setting = dynamic_cast<ISetting *>(plugin);
		if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
		if (setting && mainWnd) break;
	}
}
