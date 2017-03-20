#ifndef SCREENATTR_H
#define SCREENATTR_H

#include <QCoreApplication>
#include <QSize>

#include "NodeAttr.h"
#include "IScreen.h"
#include "AttrRect.h"

class ScreenAttr : public NodeAttr
{
	Q_DECLARE_TR_FUNCTIONS(ProgramAttr)
public:
	explicit ScreenAttr(INodeBox* box, Json* jnode);

	IScreen::TopCfg top;
	IScreen::Module led;
	IScreen::ScreenPara para;
	IScreen::OnOff power;
	IScreen::SyncTime sync;
	IScreen::Bright bright;

	Attr<KRect> contentRect;
	void exportProjJson(Json& jnode);
	QString exportCoreJson(Json& jnode);
};

#endif // SCREENATTR_H
