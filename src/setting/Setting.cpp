#include "Setting.h"

#include <QDir>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QLocale>

DEF_PLUGIN_CREATOR(Setting)

Setting::Setting() :
	user{QString("data/%1.ini").arg(QCoreApplication::applicationName()), QSettings::IniFormat},
	preset_{"bin/kshow.ini", QSettings::IniFormat}
{
	tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation)
			+ "/" + QCoreApplication::organizationName()
			+ "/" + QCoreApplication::applicationName()
			+ "/" ;
}

Setting::~Setting()
{
	QDir dir{tempPath};
	if (dir.exists(tempPath))
		dir.removeRecursively();
}

QVariant Setting::get(const QString &key) const
{
	return user.value(key);
}

void Setting::set(const QString &key, const QVariant &value)
{
	user.setValue(key, value);
}

QString Setting::tempDir() const
{
	QDir tempDir{tempPath};
	if (!tempDir.exists())
		tempDir.mkpath(tempPath);
	return tempPath;
}

QString Setting::language() const
{
	QString lang = get("language").toString();
	if (lang.isEmpty()) {
		lang = QLocale::system().name();
	}
	return lang;
}

void Setting::setLanguage(const QString& lang)
{
	set("language", lang);
}

QSettings& Setting::preset()
{
	return preset_;
}
