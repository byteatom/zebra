#include "Lang.h"

DEF_PLUGIN_CREATOR(Lang)

#include <QLocale>
#include <QDirIterator>
#include <QTranslator>
#include <QCoreApplication>
#include <QMessageBox>

IInit::Priority Lang::priority()
{
	return Priority::LANGUAGE;
}

void Lang::init(const std::list<IPlugin*>& plugins)
{
	for (auto plugin: plugins) {
		if (!setting) setting = dynamic_cast<ISetting *>(plugin);
		if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
		if (setting && mainWnd) break;
	}

	QString lang = setting->language();
	QTranslator* trans = new QTranslator;
	if (trans->load(QString("language/kshow_%1.qm").arg(lang)))
		QCoreApplication::installTranslator(trans);
	else
		delete trans;
}

Commands Lang::commands()
{
	Commands cmds;
	int menuIndex = 0;
	QString lang = setting->language();
	QDirIterator dirIt("language", {{"kshow_*.qm"}}, QDir::Files);
	while (dirIt.hasNext()) {
		dirIt.next();
		QString name = dirIt.fileName();
		name = name.mid(6, name.size() - 9);

		Command cmd;
		cmd.menuBarIndex = MENU_LANGUAGE;
		cmd.menuIndex = menuIndex;
		menuIndex++;
		QAction* action = new QAction(QLocale(name).nativeLanguageName(), nullptr);
		action->setCheckable(true);
		action->setActionGroup(&group);
		if (name == lang) action->setChecked(true);
		connect(action, &QAction::toggled, [this, name](bool checked){
			if (!checked) return;
			setting->setLanguage(name);
			QWidget* mainWidget = mainWnd->widgetAsMainWnd();
			if (QMessageBox::Yes != QMessageBox::question(mainWidget,
					tr("Restart"),
					tr("Would you like to restart application to make the language take effect immediately?")))
				return;
			mainWidget->close();
			if (mainWidget->isHidden())
				qApp->setProperty("exitCode", 0x12345678);
		});
		cmd.action = action;
		cmds.push_back(cmd);
	}

	return cmds;
}
