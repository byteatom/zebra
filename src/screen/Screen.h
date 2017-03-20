#ifndef SCREEN_H
#define SCREEN_H

#include <QCoreApplication>

#include "ScreenAttr.h"
#include "ScreenView.h"
#include "ScreenCfger.h"
#include "Node.h"
#include "NodeBox.h"
#include "IScreen.h"
#include "ICore.h"

struct ScreenData
{
	explicit ScreenData(INodeBox* box, Json* jnode);
	virtual ~ScreenData();

	ScreenAttr* attr;
	ScreenView* view;
	ScreenCfger* cfger;
};

class Screen : private ScreenData, public Node, public NodeBox, public IScreen
{
	Q_DECLARE_TR_FUNCTIONS(Screen)
public:
	explicit Screen(INodeBox* box, Json* jnode, ICore* core, IProtocol* protocol);

private:
	virtual ~Screen() = default;

	virtual void zoom(qreal factor) override;

	virtual void append(INode* child) override;
	virtual void activate() override;
	virtual void activateView(QWidget* child) override;
	virtual Attr<int>& colorMode() override;
	virtual Attr<KRect>& contentRect() override;

	virtual void exportProjJson(Json& jnode) override;
	virtual QString exportCoreJson(Json& jnode) override;

	virtual qreal zoomFactor() override;
	virtual Module& module() override;
	virtual ScreenPara& screenPara() override;
	virtual OnOff& onOff() override;
	virtual SyncTime& syncTime() override;
	virtual Bright& bright() override;
	virtual QString buildFiles(const QString& dir, bool allEndian) override;
	virtual IProtocol* protocol() override;

	using ScreenData::attr;
	using ScreenData::view;
	using ScreenData::cfger;

	ICore* core;
	IProtocol* iprotocol;
};

#endif // SCREEN_H
