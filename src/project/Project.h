#ifndef PROJECT_H
#define PROJECT_H

#include <QTimer>
#include <QString>

#include "IPlugin.h"
#include "IInit.h"
#include "ICommand.h"
#include "NodeBox.h"
#include "IProject.h"
#include "ISetting.h"
#include "INodeFactory.h"
#include "IMainWnd.h"

class Project : public QObject, public IPlugin, public IInit, public ICommand,
		public NodeBox, public IProject
{
	Q_OBJECT
public:
	Project();

private:
	virtual ~Project() = default;

	virtual IInit::Priority priority() override;
	virtual void init(const std::list<IPlugin*>& plugins) override;

	virtual Commands commands() override;

	virtual void activateView(QWidget* child) override;
	virtual void activateCfger(QWidget* child) override;

	virtual QString filePath() override;
	void setFilePath(const QString& path);
	QString currentPath;

	QString dir();
	bool open(const QString &path);
	bool save(const QString &path);
	virtual bool close() override;
	void newp();
	void open();
	void save();
	void saveAs();
	void pack();

	ISetting* setting = nullptr;
	IMainWnd* mainWnd = nullptr;
	QWidget* mainWidget = nullptr;
	QString mainTitle;
	INodeFactory* screenFactory = nullptr;
	INodeFactory* programFactory = nullptr;
	QTimer autoSave;
};

#endif // PROJECT_H
