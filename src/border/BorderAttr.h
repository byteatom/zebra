#ifndef BORDERATTR_H
#define BORDERATTR_H

#include <QCoreApplication>
#include <QImage>
#include <QPixmap>
#include "QStr.h"
#include "Json.h"
#include "Attr.h"

class BorderAttr
{
    Q_DECLARE_TR_FUNCTIONS(BorderAttr)
public:
    explicit BorderAttr();
    explicit BorderAttr(Json& jnode);
    virtual ~BorderAttr();

	void exportProjJson(Json& jnode);
	QString exportCoreJson(Json& jnode);

    QStr borderPath;
    bool checked = false;
    int style = 0;
    int speed = 5;
    int direct = 1;

    QStr borderDir;
    QStr createDir;
    int speedCnt = 0;
    int step = 0;
    bool playing = false;
    QPixmap *sight = nullptr;
    Attr<bool> notify;
    QImage image;
    QPixmap pixUp;
    QPixmap pixRight;
    QPixmap pixDown;
    QPixmap pixLeft;
};

#endif // BORDERATTR_H
