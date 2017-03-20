#ifndef MAINWN_H
#define MAINWN_H

#include "ui_MainWnd.h"

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QTimer>
#include <QContextMenuEvent>

#include "IPlugin.h"
#include "IMainWnd.h"
#include "IInit.h"
#include "ISetting.h"
#include "IAnimate.h"
#include "IRegion.h"
#include "IProject.h"
#include "INodeBox.h"
#include "INodeFactory.h"

namespace Ui {
class MainWnd;
}

class MainWnd : public QMainWindow, public IPlugin, public IInit, public IMainWnd
{
	Q_OBJECT
public:
	MainWnd();

private:
	virtual ~MainWnd();

	Ui::MainWnd *ui;
	ISetting* setting = nullptr;
	IProject* project = nullptr;
	INodeBox* topbox = nullptr;
	INodeFactory* screenFactory = nullptr;
	INodeFactory* programFactory = nullptr;

	virtual IInit::Priority priority() override;
	virtual void init(const std::list<IPlugin*>& plugins) override;
	virtual QWidget* widgetAsMainWnd() override;
	virtual INode* currentNode(INode::Type type) override;
	virtual INodeBox* currentNodeBox(INode::Type type) override;
	virtual void addNode(INode* node) override;
	virtual void setCurrentNode(INode* node) override;

	virtual void activateView(QWidget* child) override;
	virtual void activateCfger(QWidget* child) override;

	void itemCurrentChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
	void itemSelectionChanged();
	void nameChanged(QWidget* editor, QAbstractItemDelegate::EndEditHint hint);

	QMenu *treePopMenu;
	void popTreeMenu(const QPoint &pos);

	void expand();
	void collapse();
	void up();
	void down();
	void remove();

	IAnimate* animate = nullptr;
	QTimer animateTimer;
	QWidget* animateWidget = nullptr;
	void animateSwitch(bool checked);
	void animateDraw();

	int zoomIndex = 0;
	void zoomIn();
	void zoomOut();
	void zoomFit();

	void adjustMax();

	boost::signals2::connection pageSig;
	void enablePage(bool enable);
	IRegion* currentRegion();
	void pageFirst();
	void pagePrevious();
	void pageNext();
	void pageLast();

	void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWN_H
