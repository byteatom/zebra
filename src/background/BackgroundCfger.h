#ifndef BORDERCFGER_H
#define BORDERCFGER_H

#include <QGroupBox>
#include <QButtonGroup>

#include "BackgroundAttr.h"

namespace Ui {
class BackgroundCfger;
}

class BackgroundCfger : public QGroupBox
{
	Q_OBJECT

public:
    explicit BackgroundCfger(BackgroundAttr *attr_);
    ~BackgroundCfger();

private:
    void styleChanged(int index);
    void backgourndChecked(bool check);
    void speedChanged(int speed);
    void selectMode(QAbstractButton *button);
    QButtonGroup selectRadios;
    BackgroundAttr *attr = nullptr;
    Ui::BackgroundCfger *ui;
};

#endif // BORDERCFGER_H
