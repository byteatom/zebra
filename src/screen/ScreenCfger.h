#ifndef SCREENCFGER_H
#define SCREENCFGER_H

#include "ScreenAttr.h"
#include "Cfger.h"

namespace Ui {
class ScreenCfger;
}

struct DispInfo {
    QString cardModel;
    int screenWidth;
    int screenHeight;
    int baseColor;
    QString communication;
    QString port;
};
class ScreenCfger : public Cfger
{
	Q_OBJECT
public:
	explicit ScreenCfger(ScreenAttr *attr);
	virtual ~ScreenCfger();
    void updateDispInfo(DispInfo &info);

private:
	Ui::ScreenCfger *ui = nullptr;
	ScreenAttr *attr = nullptr;
};

#endif // SCREENCFGER_H
