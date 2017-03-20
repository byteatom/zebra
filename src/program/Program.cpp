#include "Program.h"

#include "IRegion.h"

ProgramData::ProgramData(INodeBox* box, Json* jnode, IBackground* background, IBorder* border)
{
	attr = new ProgramAttr{box, jnode, background, border},
	view = new ProgramView{attr},
	cfger = new ProgramCfger{attr};
}

ProgramData::~ProgramData()
{
	delete cfger;
	delete view;
	delete attr;
}

Program::Program(INodeBox *box, Json* jnode, IBackground* background, IBorder* border) :
	ProgramData{box, jnode, background, border},
	Node{attr, view, cfger}
{
}

void Program::setData(INodeData* data)
{
	Node::setData(data);
	view->data = data;
}

void Program::zoom(qreal factor)
{
	view->zoom(factor);
	for (INode* child: NodeBox::children) {
		child->zoom(factor);
	}
}

void Program::append(INode* child)
{
	NodeBox::append(child);
	view->attach(child->view());
}

void Program::activateView(QWidget* child)
{
	attr->active = true;
	box()->activateView(view);
	static_cast<View*>(view)->activate();
	view->activate(child);
}

void Program::exportProjJson(Json& jnode)
{
	attr->exportProjJson(jnode);

	Json jnodes;
	for (INode* child: children) {
		Json jchild;
		child->exportProjJson(jchild);
		jnodes.push_back(jchild);
	}
	jnode["~regions"] = jnodes;
}

QString Program::exportCoreJson(Json& jnode)
{
	if (children.empty()) return QString(tr("Program \"%1\" has no region!")).arg(attr->name);

	Json jnodes;
	bool onlyTime = true;
	for (INode* child: children) {
		INode::Type type = child->type();
		if (type != CLOCK && type != CALENDAR && type != TIMER) onlyTime = false;
		QString ret = child->exportCoreJson(jnodes);
		if (!ret.isEmpty()) return ret;
	}

	ProgramAttr::Mode mode = static_cast<ProgramAttr::Mode>(attr->mode);
	int fixedValue = attr->fixedValue;
	if (onlyTime) {
		attr->mode = ProgramAttr::FIXED;
		attr->fixedValue = 5;
	}

	QString ret = attr->exportCoreJson(jnode);

	if (onlyTime) {
		attr->mode = mode;
		attr->fixedValue = fixedValue;
	}

	if (!ret.isEmpty()) return ret;

	jnode["areaArray"] = jnodes;

	return QString();
}

void Program::play()
{
	view->play();
	playing = true;
	animates.clear();
	for (INode* child: children) {
		view->activate(child->view());
		IAnimate* animate = dynamic_cast<IAnimate*>(child);
		if (animate) animates.push_back(animate);
	}
	for (IAnimate* animate: animates) {
		animate->play();
	}
}

void Program::draw()
{
	view->draw();
	for (IAnimate* animate: animates) {
		animate->draw();
	}
}

void Program::stop()
{
	playing = false;
	for (IAnimate* animate: animates) {
		animate->stop();
		animate->draw();
	}
	view->stop();
}

Attr<KRect>& Program::contentRect()
{
	return attr->contentRect;
}
