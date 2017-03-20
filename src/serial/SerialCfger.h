#ifndef SERIALCFGER_H
#define SERIALCFGER_H

#include <QFrame>

#include "Serial.h"

namespace Ui {
class SerialCfger;
}

class SerialCfger : public QFrame
{
	Q_OBJECT

public:
	explicit SerialCfger(Serial* serial);
	~SerialCfger();

private:
	Ui::SerialCfger *ui;
	Serial* serial;
};

#endif // SERIALCFGER_H
