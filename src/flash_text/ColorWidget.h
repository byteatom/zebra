#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QWidget>
#include "FlashTextAttr.h"

namespace Ui {
class ColorWidget;
}

class ColorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorWidget(int *color, QWidget *parent = 0);
    ~ColorWidget();

private:
    virtual void closeEvent(QCloseEvent *e) override;
    Ui::ColorWidget *ui;
};

#endif // COLORWIDGET_H
