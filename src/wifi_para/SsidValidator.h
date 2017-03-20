#ifndef SSIDVALIDATOR_H
#define SSIDVALIDATOR_H

#include <QValidator>

class SsidValidator : public QValidator
{
	Q_OBJECT
public:
	explicit SsidValidator(QObject* parent = nullptr);
	virtual State validate(QString &input, int &pos) const override;
};

#endif // SSIDVALIDATOR_H
