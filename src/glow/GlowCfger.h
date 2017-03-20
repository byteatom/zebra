#ifndef GLOWCFGER_H
#define GLOWCFGER_H

#include <QGroupBox>

class Glow;

namespace Ui {
class GlowCfger;
}

class GlowCfger : public QGroupBox
{
	Q_OBJECT

public:
	explicit GlowCfger(Glow* glow);
	~GlowCfger();

private:
	Ui::GlowCfger *ui;
	Glow* glow;
};

#endif // GLOWCFGER_H
