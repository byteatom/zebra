#include "MainWnd.h"

#include <vector>
#include <list>
#include <utility>
#include <algorithm>

#include <QVariant>
#include <QMessageBox>
#include <QCoreApplication>
//#include <QStyleFactory>

#include "ICommand.h"
#include "NodeItem.h"

DEF_PLUGIN_CREATOR(MainWnd)

#define cfgLastWindowGeometry "LastWindowGeometry"
#define cfgLastWindowState "LastWindowState"

MainWnd::MainWnd() :
	QMainWindow(nullptr),
	ui(new Ui::MainWnd),
	treePopMenu{new QMenu{this}}
{
}

MainWnd::~MainWnd()
{
	delete ui;
}

IInit::Priority MainWnd::priority()
{
	return Priority::MAINWND;
}

void MainWnd::init(const std::list<IPlugin*>& plugins)
{
	for (auto plugin: plugins) {
		if (!setting) setting = dynamic_cast<ISetting *>(plugin);
		if (!project) project = dynamic_cast<IProject *>(plugin);
		INodeFactory* nodeFactory = dynamic_cast<INodeFactory *>(plugin);
		if (nodeFactory) {
			if (nodeFactory->type() == INode::Type::SCREEN)
				screenFactory = nodeFactory;
			else if (nodeFactory->type() == INode::Type::PROGRAM)
				programFactory = nodeFactory;
		}
		if (setting && project && screenFactory && programFactory) break;
	}
	topbox =  dynamic_cast<INodeBox *>(project);

	QString themeName = setting->preset().value("theme").toString();
	if (themeName.isEmpty())
		themeName = "classic";
	QFile themeFile(QString("theme/%1.qss").arg(themeName));
	themeFile.open(QFile::ReadOnly);
	if (themeFile.isOpen())
		qApp->setStyleSheet(themeFile.readAll());

	ui->setupUi(this);
	setContextMenuPolicy(/*Qt::NoContextMenu*/Qt::PreventContextMenu);
	QString product = QCoreApplication::applicationName();
	setWindowIcon(QIcon{QString("image/icon/%1.ico").arg(product)});
	setWindowTitle(QString("%1 V%2").arg(product).arg(QCoreApplication::applicationVersion()[0]));

	QWidget *viewStub = ui->viewScroller->takeWidget();
	delete viewStub;
	QWidget *cfgerStub = ui->cfgerScroller->takeWidget();
	delete cfgerStub;

	treePopMenu->addAction(ui->expand);
	treePopMenu->addAction(ui->collapse);
	treePopMenu->addSeparator();
	treePopMenu->addAction(ui->up);
	treePopMenu->addAction(ui->down);
	treePopMenu->addSeparator();
	treePopMenu->addAction(ui->remove);

	ui->splitterVer->setStretchFactor(0, 1);
	ui->splitterVer->setStretchFactor(1, 1);
	ui->splitterHor->setStretchFactor(0, 1);
	ui->splitterHor->setStretchFactor(1, 4);

	connect(ui->tree, &QTreeWidget::currentItemChanged,
			this, &MainWnd::itemCurrentChanged);
	connect(ui->tree, &QTreeWidget::itemSelectionChanged,
			this, &MainWnd::itemSelectionChanged);
	connect(ui->tree->itemDelegate(), &QAbstractItemDelegate::closeEditor,
			this, &MainWnd::nameChanged);
	connect(ui->tree, &QTreeWidget::customContextMenuRequested,
			this, &MainWnd::popTreeMenu);
	connect(ui->expand, &QAction::triggered,
			this, &MainWnd::expand);
	connect(ui->collapse, &QAction::triggered,
			this, &MainWnd::collapse);
	connect(ui->up, &QAction::triggered,
			this, &MainWnd::up);
	connect(ui->down, &QAction::triggered,
			this, &MainWnd::down);

	ui->remove->setEnabled(false);
	connect(ui->remove, &QAction::triggered,
			this, &MainWnd::remove);

	ui->animate->setEnabled(false);
	connect(ui->animate, &QAction::toggled,
			this, &MainWnd::animateSwitch);
	animateTimer.setTimerType(Qt::PreciseTimer);
	animateTimer.setInterval(15);
	connect(&animateTimer, &QTimer::timeout,
			this, &MainWnd::animateDraw);

	std::vector<std::list<Command>> menus(MENU_MAX);
	std::list<Command> tools;

	//get cmd from others
	for (auto plugin: plugins) {
		ICommand *icmd = dynamic_cast<ICommand *>(plugin);
		if (icmd) {
			Commands cmds = icmd->commands();
			for (Command& cmd: cmds) {
				if (cmd.menuBarIndex >= 0 && cmd.menuIndex >= 0) {
					menus[cmd.menuBarIndex].push_back(cmd);
				}
				if (cmd.toolBarIndex >= 0) {
					tools.push_back(cmd);
				}
			}
		}
	}

	Command cmdQuit;
	cmdQuit.menuBarIndex = MENU_FILE;
	cmdQuit.menuIndex = MENU_FILE_QUIT;
	cmdQuit.action = new QAction(tr("Quit"), nullptr);
	connect(cmdQuit.action, &QAction::triggered, this, [this](){QMainWindow::close();});
	menus[MENU_FILE].push_back(cmdQuit);

	Command cmdRemove;
	cmdRemove.toolBarIndex = TOOL_REMOVE;
	cmdRemove.action = ui->remove;
	tools.push_back(cmdRemove);

	Command cmdAnimate;
	cmdAnimate.toolBarIndex = TOOL_PREVIEW;
	cmdAnimate.action = ui->animate;
	tools.push_back(cmdAnimate);

	static const int TOOL_SPRTS[] = {
		TOOL_SEPERATOR1,
		TOOL_SEPERATOR2,
		TOOL_SEPERATOR3,
		TOOL_SEPERATOR4,
		TOOL_SEPERATOR5,
		TOOL_SEPERATOR6,
		TOOL_SEPERATOR7
	};
	for (auto index: TOOL_SPRTS) {
		Command cmdToolSprt;
		cmdToolSprt.toolBarIndex = index;
		cmdToolSprt.action = new QAction(nullptr);
		cmdToolSprt.action->setSeparator(true);
		tools.push_back(cmdToolSprt);
	}

	static const QString MENU_BAR[MENU_MAX] = {
		tr("&File"),
		tr("&Add"),
		tr("&Set"),
		tr("&Test"),
		tr("&Cloud"),
		tr("&Language"),
		tr("&Help")
	};

	//add to menu
	for (Commands& cmds: menus) {
		if (cmds.size() == 0) continue;
		cmds.sort([](const Command& first, const Command& second){
			return first.menuIndex < second.menuIndex;
		});
		QMenu *menu = new QMenu(MENU_BAR[cmds.front().menuBarIndex], ui->menuBar);
		ui->menuBar->addMenu(menu);
		for (Command& cmd: cmds) {
			menu->addAction(cmd.action);
			if (!cmd.action->parent()) cmd.action->setParent(this);
		}
	}

	//add to toolbar
	tools.sort([](const Command& first, const Command& second){
		return first.toolBarIndex < second.toolBarIndex;
	});
	ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	for (Command& cmd: tools) {
		ui->toolBar->addAction(cmd.action);
		if (!cmd.action->parent()) cmd.action->setParent(this);
	}

	connect(ui->zoomIn, &QToolButton::clicked,
			this, &MainWnd::zoomIn);
	connect(ui->zoomOut, &QToolButton::clicked,
			this, &MainWnd::zoomOut);
	connect(ui->zoomFit, &QToolButton::clicked,
			this, &MainWnd::zoomFit);
	connect(ui->pageFirst, &QToolButton::clicked,
			this, &MainWnd::pageFirst);
	connect(ui->pagePrevious, &QToolButton::clicked,
			this, &MainWnd::pagePrevious);
	connect(ui->pageNext, &QToolButton::clicked,
			this, &MainWnd::pageNext);
	connect(ui->pageLast, &QToolButton::clicked,
			this, &MainWnd::pageLast);
	connect(ui->adjustMax, &QToolButton::clicked,
			this, &MainWnd::adjustMax);

	if (setting) {
		QVariant geo = setting->get(cfgLastWindowGeometry);
		if (!geo.isNull())  restoreGeometry(geo.toByteArray());
		QVariant state = setting->get(cfgLastWindowState);
		if (!state.isNull()) restoreState(state.toByteArray(), 0);
	}

	QMainWindow::show();

	qApp->processEvents();
}

QWidget* MainWnd::widgetAsMainWnd()
{
	return this;
}

INode* MainWnd::currentNode(INode::Type type)
{
	NodeItem* currrentItem = static_cast<NodeItem*>(ui->tree->currentItem());
	while (currrentItem) {
		if (currrentItem->node->type() == type) break;
		currrentItem = static_cast<NodeItem*>(currrentItem->parent());
	}
	return currrentItem ? currrentItem->node : nullptr;
}

INodeBox* MainWnd::currentNodeBox(INode::Type type)
{
	NodeItem* currentItem = static_cast<NodeItem*>(ui->tree->currentItem());

	if (!currentItem) {
		if (type == INode::Type::SCREEN)
			return topbox;
		if (type > INode::Type::SCREEN)
			screenFactory->create();
		if (type > INode::Type::PROGRAM)
			programFactory->create();
		return dynamic_cast<INodeBox *>(static_cast<NodeItem*>(ui->tree->currentItem())->node);
	}

	int currentType = currentItem->node->type();
	NodeItem* boxItem = currentItem;
	if (type > INode::Type::PROGRAM) {
		if (currentType > INode::Type::PROGRAM )
			return dynamic_cast<INodeBox *>(static_cast<NodeItem*>(currentItem->parent())->node);
		else if(currentType == INode::Type::PROGRAM) {
			return dynamic_cast<INodeBox *>(currentItem->node);
		}
		else if(currentType == INode::Type::SCREEN) {
			int childCount = currentItem->childCount();
			if (0 >= childCount) {
				programFactory->create();
				return dynamic_cast<INodeBox *>(static_cast<NodeItem*>(ui->tree->currentItem())->node);
			} else {
				return dynamic_cast<INodeBox *>(static_cast<NodeItem*>(currentItem->child(childCount - 1))->node);
			}
		}
	}
	else if (type == INode::Type::PROGRAM) {
		while (currentItem->parent()) {
			currentItem = static_cast<NodeItem*>(currentItem->parent());
		}
		return dynamic_cast<INodeBox *>(currentItem->node);
	} else if (type == INode::Type::SCREEN) {
		return topbox;
	}

	return nullptr;
}

void MainWnd::addNode(INode* node)
{
	INodeBox* nodeParent = node->box();
	INode* parentNode = dynamic_cast<INode *>(nodeParent);
	NodeItem* item = nullptr;
	if (parentNode) {
		item = new NodeItem(static_cast<NodeData*>(parentNode->data())->item,
							node->name(), node);
	} else {
		item = new NodeItem(ui->tree, node->name(), node);
	}
	node->setData(new NodeData{item});
}

void MainWnd::setCurrentNode(INode* node)
{
	ui->tree->setCurrentItem(static_cast<NodeData*>(node->data())->item);
}

void MainWnd::activateView(QWidget* child)
{
	if (ui->viewScroller->widget() == child) return;

	ui->viewScroller->takeWidget();
	ui->viewScroller->setWidget(child);
}

void MainWnd::activateCfger(QWidget* child)
{
	if (ui->cfgerScroller->widget() == child) return;

	ui->cfgerScroller->takeWidget();
	ui->cfgerScroller->setWidget(child);
}

void MainWnd::itemCurrentChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	if (previous) {
		INode* nodePre = static_cast<NodeItem*>(previous)->node;
		if (nodePre) nodePre->deactive();
	}

	INode* node = nullptr;
	IAnimate* animate = nullptr;
	IRegion* region = nullptr;
	if (pageSig.connected()) pageSig.disconnect();
	enablePage(false);
	if (current) {
		NodeItem* item = static_cast<NodeItem*>(current);
		node = item->node;
		node->activate();
		region = dynamic_cast<IRegion*>(node);
		if (region) {
			pageSig = region->pageObserve([this](int count, int current) {
				ui->pageCount->setText(QString::number(count));
				ui->pageCurrent->setText(QString::number(count > 0 ? current + 1 : 0));
				enablePage(count > 1);
			});
		}
		animate = dynamic_cast<IAnimate*>(node);
	}

	ui->animate->setEnabled(animate);
	ui->adjustMax->setEnabled(region);
	ui->zoomFit->setEnabled(node);
	ui->zoomIn->setEnabled(node);
	ui->zoomOut->setEnabled(node);
}

void MainWnd::itemSelectionChanged()
{
	ui->remove->setEnabled(ui->tree->selectedItems().count() > 0);
}

void MainWnd::nameChanged(QWidget* editor, QAbstractItemDelegate::EndEditHint hint)
{
	NodeItem* item = static_cast<NodeItem*>(ui->tree->currentItem());
	if (!item->node->box()->checkName(item->node, item->text(0))) {
		QMessageBox::warning(this, tr("Error"), tr("The name entered has existed. Please try another one."));
		item->setText(0, item->node->name());
		ui->tree->editItem(item);
	} else
		item->node->setName(item->text(0));
}

void MainWnd::popTreeMenu(const QPoint &pos)
{
	QTreeWidgetItem *currentItem = ui->tree->itemAt(pos);

	ui->expand->setEnabled(false);
	ui->collapse->setEnabled(false);
	for (int i = 0; i < ui->tree->topLevelItemCount(); ++i) {
		QTreeWidgetItem *topItem = ui->tree->topLevelItem(i);
		if (topItem->childCount() > 0) {
			ui->expand->setEnabled(true);
			ui->collapse->setEnabled(true);
			break;
		}
	}

	ui->up->setEnabled(false);
	ui->down->setEnabled(false);
	if (currentItem) {
		QTreeWidgetItem *parentItem = currentItem->parent();
		if (!parentItem)
			parentItem = ui->tree->invisibleRootItem();
		int itemIndex = parentItem->indexOfChild(
			const_cast<QTreeWidgetItem *>(currentItem));
		ui->up->setEnabled(itemIndex > 0);
		ui->down->setEnabled(itemIndex < parentItem->childCount() - 1);
	}

	treePopMenu->exec(ui->tree->viewport()->mapToGlobal(pos));
}

void MainWnd::expand()
{
	ui->tree->expandAll();
}

void MainWnd::collapse()
{
	ui->tree->collapseAll();
}

void MainWnd::up()
{
	NodeItem* item = static_cast<NodeItem*>(ui->tree->currentItem());
	NodeItem* parent = static_cast<NodeItem*>(item->parent());
	INodeBox* box = nullptr;
	if (parent) {
		int index = parent->indexOfChild(item);
		parent->takeChild(index);
		parent->insertChild(index - 1, item);
		box = dynamic_cast<INodeBox*>(parent->node);
	} else {
		int index = ui->tree->indexOfTopLevelItem(item);
		ui->tree->takeTopLevelItem(index);
		ui->tree->insertTopLevelItem(index - 1, item);
		box = topbox;
	}
	box->raise(item->node);
	ui->tree->setCurrentItem(item);
}

void MainWnd::down()
{
	NodeItem* item = static_cast<NodeItem*>(ui->tree->currentItem());
	NodeItem* parent = static_cast<NodeItem*>(item->parent());
	INodeBox* box = nullptr;
	if (parent) {
		int index = parent->indexOfChild(item);
		parent->takeChild(index);
		parent->insertChild(index + 1, item);
		box = dynamic_cast<INodeBox*>(parent->node);
	} else {
		int index = ui->tree->indexOfTopLevelItem(item);
		ui->tree->takeTopLevelItem(index);
		ui->tree->insertTopLevelItem(index + 1, item);
		box = topbox;
	}
	box->lower(item->node);
	ui->tree->setCurrentItem(item);
}

void MainWnd::remove()
{
	ui->tree->blockSignals(true);

	QList<QTreeWidgetItem*> items = ui->tree->selectedItems();
	if (items.count() <= 0) return;
	std::sort(items.begin(), items.end(), [](QTreeWidgetItem* first, QTreeWidgetItem* second) {
		int depth1 = 0;
		QTreeWidgetItem* item1 = first;
		while (item1 = item1->parent()) ++depth1;
		int depth2 = 0;
		QTreeWidgetItem* item2 = second;
		while (item2 = item2->parent()) ++depth2;
		return depth1 > depth2;
	});

	QTreeWidgetItem *activeItem = nullptr;

	for (auto item: items) {
		INode* node = static_cast<NodeItem*>(item)->node;
		if (node->type() != INode::Type::SCREEN ||
			QMessageBox::Yes == QMessageBox::question(this,
									tr("Confirm"),
									tr("Delete screen \"%1\"?").arg(node->name()))) {
			QTreeWidgetItem* lastItem = items.back();
			if (item == lastItem) {
				QTreeWidgetItem *lastParent = lastItem->parent();
				if (!lastParent)
					lastParent = ui->tree->invisibleRootItem();
				int index = lastParent->indexOfChild(lastItem);
				activeItem = lastParent->child(index + 1);
				if (!activeItem) activeItem = ui->tree->itemAbove(lastItem);
			}

			node->userDelete();
			delete node;
		}
	}

	if (activeItem) {
		ui->tree->setCurrentItem(activeItem);
	}
	itemCurrentChanged(activeItem, nullptr);

	ui->tree->blockSignals(false);
}

void MainWnd::animateSwitch(bool checked)
{
	if (checked) {
		NodeItem* item = static_cast<NodeItem*>(ui->tree->currentItem());
		if (!item) return;
		INode* node = item->node;
		if (item->node->type() != INode::Type::PROGRAM)
			node = dynamic_cast<INode*>(node->box());
		if (!node) return;
		animate = dynamic_cast<IAnimate*>(node);
		if (!animate) return;
		animateWidget = node->view();
		animate->play();
		animateTimer.start();
	} else {
		animateTimer.stop();
		if (animate) {
			animate->stop();
			animate->draw();
		}
		animate = nullptr;
		if (animateWidget) animateWidget->update();
		animateWidget = nullptr;
	}
}

void MainWnd::animateDraw()
{
	if (animate) animate->draw();
	if (animateWidget) animateWidget->repaint();
}

static const int zoomFactors[] = {1, 2, 3, 4, 6, 8};

void MainWnd::zoomIn()
{
	if (zoomIndex >= std::extent<decltype(zoomFactors)>::value - 1) return;
	INode* node = currentNode(INode::Type::SCREEN);
	if (!node) return;
	++zoomIndex;
	node->zoom(zoomFactors[zoomIndex]);
}

void MainWnd::zoomOut()
{
	if (zoomIndex <= 0) return;
	INode* node = currentNode(INode::Type::SCREEN);
	if (!node) return;
	--zoomIndex;
	node->zoom(zoomFactors[zoomIndex]);
}

void MainWnd::zoomFit()
{
	if (zoomIndex == 0) return;
	INode* node = currentNode(INode::Type::SCREEN);
	if (!node) return;
	zoomIndex = 0;
	node->zoom(zoomFactors[zoomIndex]);
}

void MainWnd::enablePage(bool enable)
{
	ui->pageFirst->setEnabled(enable);
	ui->pagePrevious->setEnabled(enable);
	ui->pageCurrent->setEnabled(enable);
	ui->pageCount->setEnabled(enable);
	ui->pageNext->setEnabled(enable);
	ui->pageLast->setEnabled(enable);
}

IRegion* MainWnd::currentRegion()
{
	NodeItem* item = static_cast<NodeItem*>(ui->tree->currentItem());
	return item ? dynamic_cast<IRegion*>(item->node) : nullptr;
}

void MainWnd::pageFirst()
{
	IRegion* region = currentRegion();
	if (!region) return;
	region->pageFirst();
}

void MainWnd::pagePrevious()
{
	IRegion* region = currentRegion();
	if (region) region->pagePrevious();
}

void MainWnd::pageNext()
{
	IRegion* region = currentRegion();
	if (region) region->pageNext();
}

void MainWnd::pageLast()
{
	IRegion* region = currentRegion();
	if (region) region->pageLast();
}

void MainWnd::adjustMax()
{
	IRegion* region = currentRegion();
	if (!region) return;
	INode* node = static_cast<NodeItem*>(ui->tree->currentItem())->node;
	region->geometry()->setRect(node->box()->contentRect()->rect());
}

void MainWnd::closeEvent(QCloseEvent *event)
{
	ui->tree->blockSignals(true);
	ui->animate->setChecked(false);
	if (project->close()) {
		if (setting) {
			setting->set(cfgLastWindowGeometry, saveGeometry());
			setting->set(cfgLastWindowState, saveState());
		}
		QMainWindow::closeEvent(event);
	} else {
		event->ignore();
	}
}
