#include "Update.h"

DEF_PLUGIN_CREATOR(Update)

#include <QCoreApplication>

#include "winsparkle.h"

#define cfgUpdateAuto "updateAuto"
#define cfgUpdateUrl "updateUrl"

Update::~Update()
{
	win_sparkle_cleanup();
}

void Update::init(const std::list<IPlugin*>& plugins)
{
	for (auto plugin: plugins) {
		if (!setting) setting = dynamic_cast<ISetting *>(plugin);
		if (setting) break;
	}

	std::string lang = setting->language().toStdString();
	win_sparkle_set_lang(lang.c_str());

	std::wstring company = QCoreApplication::organizationName().toStdWString();
	std::wstring product = QCoreApplication::applicationName().toStdWString();
	std::wstring version = QCoreApplication::applicationVersion().toStdWString();
	win_sparkle_set_app_details(company.c_str(), product.c_str(), version.c_str());

	std::string url = setting->preset().value(cfgUpdateUrl)
					  .toString()
					  .append(QCoreApplication::applicationName())
					  .append(".xml")
					  .toStdString();
	win_sparkle_set_appcast_url(url.c_str());

	QVariant vAuto = setting->preset().value(cfgUpdateAuto);
	int iAuto = vAuto.toInt();
	win_sparkle_set_automatic_check_for_updates(vAuto.isValid() ? iAuto : 1);

	win_sparkle_init();
}

Commands Update::commands()
{
	Commands cmds;

	Command cmdAuto;
	cmdAuto.menuBarIndex = MENU_HELP;
	cmdAuto.menuIndex = MENU_HELP_UPDATE_AUTO;
	cmdAuto.action = new QAction(tr("Auto Update"), nullptr);
	cmdAuto.action->setCheckable(true);
	if (setting->preset().value(cfgUpdateAuto).toInt() == 1)
		cmdAuto.action->setChecked(true);
	connect(cmdAuto.action, &QAction::toggled, this,
			[this](bool checked){
		win_sparkle_set_automatic_check_for_updates(checked);
		setting->preset().setValue(cfgUpdateAuto, (int)checked);
	});
	cmds.push_back(cmdAuto);

	Command cmdCheck;
	cmdCheck.menuBarIndex = MENU_HELP;
	cmdCheck.menuIndex = MENU_HELP_UPDATE_CHECK;
	cmdCheck.action = new QAction(tr("Check for Updates..."), nullptr);
	connect(cmdCheck.action, &QAction::triggered, this,
			[this](){
		win_sparkle_check_update_with_ui();
	});
	cmds.push_back(cmdCheck);

	return cmds;
}

