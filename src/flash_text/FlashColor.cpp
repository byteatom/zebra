#include "FlashColor.h"
#include <QWidgetAction>
#include "ColorWidget.h"

FlashColor::FlashColor() :
    color(-1)
{
    QWidgetAction *colorAction = new QWidgetAction(this);
    ColorWidget *colorWidget = new ColorWidget(&color, this);
    colorAction->setDefaultWidget(colorWidget);
    colorAction->setText(tr("color"));
    addAction(colorAction);
}

int FlashColor::getFlashColor()
{
    FlashColor flashColor;
    flashColor.exec(QCursor::pos());
    return flashColor.color;
}
