#ifndef BORDERCFGER_H
#define BORDERCFGER_H

#include <QGroupBox>
#include <QButtonGroup>

#include "BorderAttr.h"

namespace Ui {
class BorderCfger;
}

class BorderCfger : public QGroupBox
{
	Q_OBJECT

public:
    explicit BorderCfger(BorderAttr *attr);
    ~BorderCfger();

private:
    int updataBorderList(const QString &path);
    void styleChanged(int index);
    void borderChecked(bool check);
    void speedChanged(int speed);
    void directChanged(int direct);
    BorderAttr *attr = nullptr;
    Ui::BorderCfger *ui;
};

#endif // BORDERCFGER_H
