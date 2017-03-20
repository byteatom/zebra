#ifndef SETTING_H
#define SETTING_H

#include <memory>

#include <QSettings>

#include "IPlugin.h"
#include "ISetting.h"

class Setting : public IPlugin, public ISetting
{
public:
	explicit Setting();

private:
	virtual ~Setting();
	virtual QVariant get(const QString &key) const override;
	virtual void set(const QString &key, const QVariant &value) override;
	virtual QString tempDir() const override;
	virtual QString language() const override;
	virtual void setLanguage(const QString& lang) override;
	virtual QSettings& preset() override;

	QSettings user;
	QSettings preset_;
	QString tempPath;
};

#endif // SETTING_H
