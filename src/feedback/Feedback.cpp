#include "Feedback.h"
#include "FeedbackDlg.h"

DEF_PLUGIN_CREATOR(Feedback)

ISetting* Feedback::setting = nullptr;

void Feedback::init(const std::list<IPlugin*>& plugins)
{
	for (auto plugin: plugins) {
		if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
		if (!setting) setting = dynamic_cast<ISetting *>(plugin);
		if (mainWnd && setting) break;
	}
}

Commands Feedback::commands()
{
	Commands cmds;

	Command cmdFeedback;
	cmdFeedback.menuBarIndex = MENU_HELP;
	cmdFeedback.menuIndex = MENU_HELP_FEEDBACK;
	cmdFeedback.toolBarIndex = TOOL_FEEDBACK;
	cmdFeedback.action = new QAction(QIcon{"image/icon/feedback_64.png"}, tr("Feedback"), nullptr);
	connect(cmdFeedback.action, &QAction::triggered, this, &Feedback::show);
	cmds.push_back(cmdFeedback);

	return cmds;
}

void Feedback::show()
{
	FeedbackDlg dlg{mainWnd->widgetAsMainWnd()};
	dlg.exec();
}
