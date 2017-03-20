#include "About.h"
#include "AboutDlg.h"

DEF_PLUGIN_CREATOR(About)

void About::init(const std::list<IPlugin*>& plugins)
{
	for (auto plugin: plugins) {
		if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
		if (mainWnd) break;
	}
}

Commands About::commands()
{
	Commands cmds;

	Command cmdAbout;
	cmdAbout.menuBarIndex = MENU_HELP;
	cmdAbout.menuIndex = MENU_HELP_ABOUT;
	cmdAbout.action = new QAction(QIcon{"image/icon/about_48.png"}, tr("About"), nullptr);
	connect(cmdAbout.action, &QAction::triggered, this,
			[this](){ AboutDlg dlg(mainWnd->widgetAsMainWnd()); dlg.exec(); });
	cmds.push_back(cmdAbout);

	return cmds;
}
