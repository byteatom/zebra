#include "PasswordValidator.h"

PasswordValidator::PasswordValidator(QObject* parent) : QValidator{parent}
{

}

QValidator::State PasswordValidator::validate(QString &input, int &pos) const
{
	QByteArray password = input.toUtf8();
	if (password.size() > 63) return Invalid;
	for (char byte: password) {
		if (byte < 32 || byte > 126) return Invalid;
	}
	return Acceptable;
}
