#ifndef FLASHFONTCOLOR_H
#define FLASHFONTCOLOR_H

#include <QWidget>

namespace Ui {
class FlashFontColor;
}

class FlashFontColor : public QWidget
{
    Q_OBJECT

public:
    explicit FlashFontColor(QWidget *parent = 0);
    ~FlashFontColor();

private:
    Ui::FlashFontColor *ui;
};

#endif // FLASHFONTCOLOR_H
