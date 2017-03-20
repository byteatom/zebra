#include "SsidValidator.h"

SsidValidator::SsidValidator(QObject* parent) : QValidator{parent}
{

}

QValidator::State SsidValidator::validate(QString &input, int &pos) const
{
	QByteArray ssid = input.toUtf8();
	if (ssid.size() > 32) return Invalid;
	return Acceptable;
}
