#include "Cluster.h"
#include "ClusterDlg.h"

DEF_PLUGIN_CREATOR(Cluster)

void Cluster::init(const std::list<IPlugin*>& plugins)
{
	for (auto plugin: plugins) {
		if (!setting) setting = dynamic_cast<ISetting *>(plugin);
		if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
		if (!project) project = dynamic_cast<IProject *>(plugin);
		if (setting && mainWnd && project) break;
	}
}

Commands Cluster::commands()
{
	Commands cmds;

	Command cmdCluster;
	cmdCluster.menuBarIndex = MENU_CLOUD;
	cmdCluster.menuIndex = MENU_CLOUD_CLUSTER;
	cmdCluster.toolBarIndex = TOOL_CLUSTER;
	cmdCluster.action = new QAction(QIcon{"image/icon/cluster_512.png"}, tr("Cluster"), nullptr);
	connect(cmdCluster.action, &QAction::triggered, this, [this](){
		INodeBox* box = dynamic_cast<INodeBox *>(project);
		ClusterDlg dlg(mainWnd->widgetAsMainWnd(), box, setting);
		dlg.exec();
	});
	cmds.push_back(cmdCluster);

	return cmds;
}
