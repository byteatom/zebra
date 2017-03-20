#ifndef ISETTING_H
#define ISETTING_H

#include <QVariant>
#include <QSettings>

class ISetting
{
public:
	virtual QVariant get(const QString& key) const = 0;
	virtual void set(const QString& key, const QVariant& value) = 0;
	virtual QString tempDir() const = 0;
	virtual QString language() const = 0;
	virtual void setLanguage(const QString& lang) = 0;
	virtual QSettings& preset() = 0;

protected:
	virtual ~ISetting() = default;
};

#endif // ISETTING_H
