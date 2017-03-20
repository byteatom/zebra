#include "Send.h"
#include "SendDlg.h"

#include "IScreen.h"

DEF_PLUGIN_CREATOR(Send)

void Send::init(const std::list<IPlugin*>& plugins)
{
	for (auto plugin: plugins) {
		if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
		if (!setting) setting = dynamic_cast<ISetting *>(plugin);
		if (mainWnd && setting) break;
	}
}

Commands Send::commands()
{
	Commands cmds;

	Command cmdSend;
	cmdSend.menuBarIndex = MENU_TEST;
	cmdSend.menuIndex = MENU_TEST_SEND;
	cmdSend.toolBarIndex = TOOL_SEND;
	cmdSend.action = new QAction(QIcon{"image/icon/send_64.png"}, tr("Send"), nullptr);
	cmdSend.action->setShortcut(Qt::CTRL + Qt::Key_G);
	connect(cmdSend.action, &QAction::triggered, this, &Send::show);
	cmds.push_back(cmdSend);

	return cmds;
}

void Send::show()
{
	INode* node = mainWnd->currentNode(INode::Type::SCREEN);
	if (!node) return;
	IScreen* screen = dynamic_cast<IScreen*>(node);
	if (!screen) return;
	SendDlg dlg{mainWnd->widgetAsMainWnd(), screen, setting};
	dlg.exec();
}
