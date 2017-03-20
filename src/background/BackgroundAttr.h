#ifndef BORDERATTR_H
#define BORDERATTR_H

#include <QCoreApplication>
#include <QImage>
#include <QPixmap>
#include "QStr.h"
#include "Json.h"
#include "Attr.h"
#include "KGBackground.h"
#include "ISetting.h"

class BackgroundAttr
{
	Q_DECLARE_TR_FUNCTIONS(BackgroundAttr)
public:
	explicit BackgroundAttr(ISetting* setting_);
	explicit BackgroundAttr(Json& jnode, ISetting* setting_);
	virtual ~BackgroundAttr();

	void exportProjJson(Json& jnode);
	QString exportCoreJson(Json& jnode);
    void toInputAttr(KGCreateMode mode);

	BackgroundInput input;
	BackgroundOutput output;

	bool checked = false;
    int select;
    int style;
    int speed;
    int delay;

	bool playing = false;
	QPixmap *sight = nullptr;
	Attr<bool> notify;

	ISetting *setting = nullptr;
};

#endif // BORDERATTR_H
