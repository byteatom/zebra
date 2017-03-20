#include "Project.h"

#include <boost/filesystem/fstream.hpp>
namespace bfs = boost::filesystem;

#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>

DEF_PLUGIN_CREATOR(Project)

#define cfgLastProject "LastProject"

Project::Project()
{
	autoSave.setInterval(600000);
	connect(&autoSave, &QTimer::timeout,
			this, static_cast<void (Project::*)()>(&Project::save));
}

IInit::Priority Project::priority()
{
	return Priority::PROJECT;
}

void Project::init(const std::list<IPlugin*>& plugins)
{
	for (auto plugin: plugins) {
		if (!setting) setting = dynamic_cast<ISetting *>(plugin);
		if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
		INodeFactory* nodeFactory = dynamic_cast<INodeFactory *>(plugin);
		if (nodeFactory) {
			if (nodeFactory->type() == INode::Type::SCREEN)
				screenFactory = nodeFactory;
			else if (nodeFactory->type() == INode::Type::PROGRAM)
				programFactory = nodeFactory;
		}
		if (setting && mainWnd && screenFactory && programFactory) break;
	}
	mainWidget = mainWnd->widgetAsMainWnd();
	mainTitle = mainWidget->windowTitle();

	QStr lastProjectPath;
	QVariant cfgLast = setting->get(cfgLastProject);
	if (!cfgLast.isNull()) {
		lastProjectPath = cfgLast.toString();
	}
	if (lastProjectPath.isEmpty() || !open(lastProjectPath))
		newp();
}

Commands Project::commands()
{
	Commands cmds;

	Command cmdNew;
	cmdNew.menuBarIndex = MENU_FILE;
	cmdNew.menuIndex = MENU_FILE_NEW;
	cmdNew.action = new QAction(tr("New"), nullptr);
	connect(cmdNew.action, &QAction::triggered, this, &Project::newp);
	cmds.push_back(cmdNew);

	Command cmdOpen;
	cmdOpen.menuBarIndex = MENU_FILE;
	cmdOpen.menuIndex = MENU_FILE_OPEN;
	cmdOpen.action = new QAction(tr("Open"), nullptr);
	connect(cmdOpen.action, &QAction::triggered, this, static_cast<void (Project::*)()>(&Project::open));
	cmds.push_back(cmdOpen);

	Command cmdFileSprt1;
	cmdFileSprt1.menuBarIndex = MENU_FILE;
	cmdFileSprt1.menuIndex = MENU_FILE_SEPERATOR1;
	cmdFileSprt1.action = new QAction(nullptr);
	cmdFileSprt1.action->setSeparator(true);
	cmds.push_back(cmdFileSprt1);

	Command cmdSave;
	cmdSave.menuBarIndex = MENU_FILE;
	cmdSave.menuIndex = MENU_FILE_SAVE;
	cmdSave.action = new QAction(tr("Save"), nullptr);
	cmdSave.action->setShortcut(QKeySequence::Save);
	connect(cmdSave.action, &QAction::triggered, this, static_cast<void (Project::*)()>(&Project::save));
	cmds.push_back(cmdSave);

	Command cmdSaveAs;
	cmdSaveAs.menuBarIndex = MENU_FILE;
	cmdSaveAs.menuIndex = MENU_FILE_SAVEAS;
	cmdSaveAs.action = new QAction(tr("Save As..."), nullptr);
	cmdSaveAs.action->setShortcut(QKeySequence::SaveAs);
	connect(cmdSaveAs.action, &QAction::triggered, this, &Project::saveAs);
	cmds.push_back(cmdSaveAs);

	Command cmdFileSprt2;
	cmdFileSprt2.menuBarIndex = MENU_FILE;
	cmdFileSprt2.menuIndex = MENU_FILE_SEPERATOR2;
	cmdFileSprt2.action = new QAction(nullptr);
	cmdFileSprt2.action->setSeparator(true);
	cmds.push_back(cmdFileSprt2);

	return cmds;
}

void Project::activateView(QWidget* child)
{
	mainWnd->activateView(child);
}

void Project::activateCfger(QWidget* child)
{
	mainWnd->activateCfger(child);
}

QString Project::filePath()
{
	return currentPath;
}

void Project::setFilePath(const QString& path)
{
	currentPath = path;
	if (currentPath.isEmpty()) {
		mainWidget->setWindowTitle(mainTitle);
		autoSave.stop();
	} else {
		QFileInfo info{currentPath};
		mainWidget->setWindowTitle(info.fileName()
					   .append(" [")
					   .append(info.canonicalPath())
					   .append("] ")
					   .append(mainTitle));
		autoSave.start();
	}
}

QString Project::dir()
{
	QString path{"data/project/"};
	QDir().mkpath(path);
	return path;
}

bool Project::open(const QString& filePath)
{
	bfs::ifstream ifs{bfs::path{static_cast<std::string>(filePath.toStdString())},
					  std::ios::in | std::ios::binary};
	if (!ifs.is_open()) {
			QMessageBox::critical(mainWidget, tr("Error"),
				tr("Failed to open file \n%1\nfor reading.").arg(filePath));
			return false;
	}

	Json jProject;
	ifs >> jProject;
	if (!jProject.is_object()) return true;

	Json jnode = jProject["~screens"];
	INode* activeNode = nullptr;
	for (Json::iterator jit = jnode.begin(); jit != jnode.end(); ++jit) {
		INode* node = screenFactory->create(this, *jit);
		if (!activeNode) activeNode = node;
	}
	if (activeNode)
		mainWnd->setCurrentNode(activeNode);

	setting->set(cfgLastProject, filePath);
	setFilePath(filePath);

	return true;
}

bool Project::save(const QString& filePath)
{
	bfs::ofstream ofs{bfs::path{static_cast<std::string>(filePath.toStdString())},
					  std::ios::out | std::ios::binary | std::ios::trunc};
	if (!ofs.is_open()) {
			QMessageBox::critical(mainWidget, tr("Error"),
				tr("Failed to save file \n%1\nfor writting.").arg(filePath));
			return false;
	}

	Json jnode;
	for (INode* child: NodeBox::children) {
		Json jchild;
		child->exportProjJson(jchild);
		jnode.push_back(jchild);
	}
	Json jProject = {
		{"modified:", QDateTime::currentDateTime().toString(Qt::DefaultLocaleLongDate)},
		{"~screens", jnode}
	};

	ofs << std::setw(4) << jProject << std::endl;

	setting->set(cfgLastProject, filePath);
	setFilePath(filePath);
	return true;
}

bool Project::close()
{
	if (NodeBox::children.size() == 0) return true;

	QMessageBox::StandardButton ret	= QMessageBox::question(mainWidget, tr("Confirm"),
									tr("Save changes?"),
									QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

	if (QMessageBox::Save == ret) {
		save();
	}
	bool close = (ret != QMessageBox::Cancel);
	if (close) {
		clear();
		setFilePath("");
	}
	return close;
}

void Project::newp()
{
	if (!close()) return;

	screenFactory->create();
	programFactory->create();
}

void Project::open()
{
	if (!close()) return;

	QStr filePath = QFileDialog::getOpenFileName(mainWidget,
										tr("Open"),
										dir(),
										tr("Project (*.kxp)"));
	if (filePath.isEmpty()) return;
	open(filePath);
}

void Project::save()
{
	if (currentPath.isEmpty())
		saveAs();
	else
		save(currentPath);
}

void Project::saveAs()
{
	QStr filePath = QFileDialog::getSaveFileName(mainWidget,
									tr("Save"),
									dir() + "/project.kxp",
									tr("Project (*.kxp)"));
	if (filePath.isEmpty())	return;
	save(filePath);
}

void Project::pack()
{
}
