#ifndef FLASHCOLOR_H
#define FLASHCOLOR_H

#include <QMenu>
#include "FlashTextAttr.h"
#include "KGMarco.h"

class FlashColor : public QMenu
{
public:
    FlashColor();
    static int getFlashColor();
private:
    int color;
};

#endif // FLASHCOLOR_H
