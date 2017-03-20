#ifndef FADECFGER_H
#define FADECFGER_H

#include <QGroupBox>

#include "FadeStyle.h"

class Fade;

namespace Ui {
class FadeCfger;
}

class FadeCfger : public QGroupBox
{
	Q_OBJECT

public:
	explicit FadeCfger(Fade* fade);
	~FadeCfger();

private:
	Ui::FadeCfger *ui;
	Fade* fade;
};

#endif // FADECFGER_H
