#ifndef QSTRINGU8_H
#define QSTRINGU8_H

#include <string>
#include <QString>

/**
 * QString wrapper for conversion from/to UTF-8 std::string
 */
struct QStr : public QString
{
	QStr() = default;

	QStr(const QStr& other) :
		QString{other}
	{
	}

	QStr(const QString& other) :
		QString{other}
	{}

	QStr(const std::string& other)
	{
		QString tmp = QString::fromStdString(other);
		swap(tmp);
	}

	operator std::string() const
	{
		return QString::toStdString();
	}

	QStr(const char* other) :
		QString{other}
	{}

	QStr(const char* other, int size) :
		QString{QByteArray{other, size}}
	{}
};

#endif // QSTRINGU8_H
