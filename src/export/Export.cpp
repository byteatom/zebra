#include "Export.h"
#include "ExportDlg.h"

#include <QProcess>

#include "IScreen.h"

DEF_PLUGIN_CREATOR(Export)

void Export::init(const std::list<IPlugin*>& plugins)
{
	for (auto plugin: plugins) {
		if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
		if (!setting) setting = dynamic_cast<ISetting *>(plugin);
		if (mainWnd && setting) break;
	}
}

Commands Export::commands()
{
	Commands cmds;

	Command cmdExport;
	cmdExport.menuBarIndex = MENU_TEST;
	cmdExport.menuIndex = MENU_TEST_EXPORT;
	cmdExport.toolBarIndex = TOOL_EXPORT;
	cmdExport.action = new QAction(QIcon{"image/icon/export_64.png"}, tr("Export"), nullptr);
	cmdExport.action->setShortcut(Qt::CTRL + Qt::Key_E);
	connect(cmdExport.action, &QAction::triggered,
			this, &Export::show);
	cmds.push_back(cmdExport);

	Command cmdWifiPara;
	cmdWifiPara.menuBarIndex = MENU_SET;
	cmdWifiPara.menuIndex = MENU_SET_WIFI_PARA;
	cmdWifiPara.action = new QAction(QIcon{"image/icon/wifi_para_512.png"}, tr("Wi-Fi Parameters"), nullptr);
	connect(cmdWifiPara.action, &QAction::triggered, [this]() {
		QProcess::execute("bin/wifi_para.exe", QStringList{setting->language()});
	});
	cmds.push_back(cmdWifiPara);

	return cmds;
}

void Export::show()
{
	INode* node = mainWnd->currentNode(INode::Type::SCREEN);
	if (!node) return;
	IScreen* screen = dynamic_cast<IScreen*>(node);
	if (!screen) return;
	ExportDlg dlg{mainWnd->widgetAsMainWnd(), screen};
	dlg.exec();
}
