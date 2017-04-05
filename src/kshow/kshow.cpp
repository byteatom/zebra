#include <list>
#include <memory>
#include <string>

#include <QApplication>
#include <QDirIterator>
#include <QLibrary>
#include <QSplashScreen>
#include <QTranslator>
#include <QDir>
#include <QFileInfo>

#include "IPlugin.h"
#include "IInit.h"
#include "UtilWin.h"
#include "QLog.h"
#ifdef NDEBUG
#include "CrashRpt.h"
#endif
#include "ISetting.h"
#include "Util.h"

#define LOG_MODULE "[kshow] "
#define kshowErr qErr<<LOG_MODULE
#define kshowInf qInfo<<LOG_MODULE

int main(int argc, char *argv[])
{
	//QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	//QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

	QApplication app(argc, argv);

	if (argc < 2) {
		kshowErr<<"start with error product";
		return EXIT_FAILURE;
	} else
		kshowInf<<"start with "<<argv[1];

	QString product{argv[1]};

	QSplashScreen splash(QPixmap(QString("image/picture/splash_%1.png").arg(product))
#ifdef NDEBUG
						 , Qt::WindowStaysOnTopHint
#endif
						 );
	splash.show();

	QDir dir;
	QFileInfoList infoList = dir.entryInfoList({{"*.company"}}, QDir::Files);
	if (!infoList.empty())
		app.setOrganizationName(infoList.first().baseName());
	app.setApplicationName(product);
	app.setApplicationVersion(UtilWin::fileVersion(product + ".exe"));

	QString pluginDir("bin/plugin");

	QLibrary settingLib{pluginDir + "/setting.dll"};
	PluginCreator settingCreator = (PluginCreator)settingLib.resolve("createKpg");
	if (!settingCreator) {
		kshowErr<<"failed to load settings";
		return EXIT_FAILURE;
	}
	IPlugin* settingPlugin = settingCreator();
	std::unique_ptr<IPlugin> settingsGuard(settingPlugin);
	ISetting* setting = dynamic_cast<ISetting*>(settingPlugin);
	if (!setting) {
		kshowErr<<"failed to create settings";
		return EXIT_FAILURE;
	}

	QString lang = setting->language();

#ifdef NDEBUG
	QFileInfo crLangFileInfo{QString("language/crashrpt_%1.ini").arg(lang)};
	std::wstring crLangFilePath = crLangFileInfo.absoluteFilePath().toStdWString();
	std::wstring crAppName = product.toStdWString();
	CR_INSTALL_INFOW crInfo;
	memset(&crInfo, 0, sizeof(crInfo));
	crInfo.cb = sizeof(crInfo);
	for (auto& priority: crInfo.uPriorities)
		priority = CR_NEGATIVE_PRIORITY;
	crInfo.dwFlags = CR_INST_ALL_POSSIBLE_HANDLERS
					 //| CR_INST_NO_GUI
					 | CR_INST_DONT_SEND_REPORT
					 | CR_INST_AUTO_THREAD_HANDLERS;
	crInfo.pszErrorReportSaveDir = L"log/dump";
	crInfo.pszLangFilePath = crLangFilePath.c_str();
	crInfo.pszAppName = crAppName.c_str();
	if (crInstallW(&crInfo) != 0) {
		wchar_t crMsg[512];
		crGetLastErrorMsgW(crMsg, 512);
		kshowErr<<"failed to install crash reporter:"<<crMsg;
	}
	Util::Guard crSg{[](){crUninstall();}};
#endif

	std::list<IPlugin*> plugins;
	Util::Guard pluginsSg{[&plugins](){
			plugins.reverse();
			for (IPlugin* plugin: plugins)
				delete plugin;
	}};

	//create
	QDirIterator itPlugin(pluginDir, {{"*.dll"}}, QDir::Files/*, QDirIterator::Subdirectories*/);
	while (itPlugin.hasNext()) {
		QString path = itPlugin.next();
		QLibrary lib{path};
		PluginCreator creator = (PluginCreator)lib.resolve("createKpg");
		QString name = QFileInfo(path).completeBaseName();
		splash.showMessage("Loading " + name,
						   Qt::AlignBottom | Qt::AlignHCenter,
						   creator ? Qt::blue : Qt::red);
		if (creator)
			plugins.push_back(creator());
		else
			kshowErr<<"failed to load "<<name<<":"<<lib.errorString();
	}

	splash.showMessage("Loading project...", Qt::AlignBottom | Qt::AlignHCenter, Qt::blue);

	//sort
	plugins.sort([](IPlugin *first, IPlugin *second){
		IInit* firstInit = dynamic_cast<IInit*>(first);
		IInit* secondInit = dynamic_cast<IInit*>(second);
		if (!firstInit)
			return false;
		else if (!secondInit)
			return true;
		else
			return firstInit->priority() < secondInit->priority();
	});

	//init
	for (IPlugin* plugin: plugins) {
		IInit* init = dynamic_cast<IInit*>(plugin);
		if (init) {
			init->init(plugins);
		} else
			break;
	}

	splash.close();

	int ret = app.exec();

	QVariant exitCode = app.property("exitCode");
	if (exitCode.isValid())
		ret = exitCode.toInt();

	kshowInf<<"exit with "<<ret;

	return ret;
}


