#ifndef PROGRAM_H
#define PROGRAM_H

#include <list>

#include <QCoreApplication>

#include "ProgramView.h"
#include "ProgramCfger.h"
#include "ProgramAttr.h"
#include "Node.h"
#include "NodeBox.h"
#include "IProgram.h"
#include "IAnimate.h"

struct ProgramData
{
	explicit ProgramData(INodeBox* box, Json* jnode, IBackground* background, IBorder* border);
	~ProgramData();

	ProgramAttr* attr;
	ProgramView* view;
	ProgramCfger* cfger;
};

class Program : private ProgramData, public Node, public NodeBox, public IProgram
{
	Q_DECLARE_TR_FUNCTIONS(Program)
public:
	explicit Program(INodeBox *box, Json* jnode, IBackground* background, IBorder* border);

private:
	virtual ~Program() = default;

	virtual void setData(INodeData* data) override;
	virtual void zoom(qreal factor) override;

	virtual void append(INode* child) override;
	virtual void activateView(QWidget* child) override;

	virtual void exportProjJson(Json& jnode) override;
	virtual QString exportCoreJson(Json& jnode) override;

	virtual void play() override;
	virtual void draw() override;
	virtual void stop() override;

	virtual Attr<KRect>& contentRect() override;

	using ProgramData::attr;
	using ProgramData::view;
	using ProgramData::cfger;
	bool playing = false;
	std::list<IAnimate*> animates;
};

#endif // PROGRAM_H
