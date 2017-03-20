#include "Screen.h"

#include <boost/filesystem/fstream.hpp>
namespace bfs = boost::filesystem;

#include "AttrRect.h"

ScreenData::ScreenData(INodeBox* box, Json* jnode)
{
	attr = new ScreenAttr{box, jnode};
	view = new ScreenView{attr};
	cfger = new ScreenCfger{attr};
}

ScreenData::~ScreenData()
{
	delete cfger;
	delete view;
	delete attr;
}

Screen::Screen(INodeBox* box, Json* jnode, ICore* core, IProtocol* protocol):
	ScreenData{box, jnode},
	Node{attr, view, cfger},
	core{core},
	iprotocol{protocol}
{
}

void Screen::zoom(qreal factor)
{
	view->zoom(factor);
	for (INode* child: NodeBox::children) {
		child->zoom(factor);
	}
}

void Screen::append(INode* child)
{
	NodeBox::append(child);
	view->attach(child->view());
}

void Screen::activate()
{
	Node::activate();
	DispInfo info;
	info.cardModel = attr->para.cardModel;
	info.screenWidth = attr->para.size.value().width();
	info.screenHeight = attr->para.size.value().height();
	info.baseColor = attr->led.baseColor;
	info.communication = tr("U disk");
	if (iprotocol) {
		info.communication.append(", ").append(iprotocol->typeName().name);
		info.port = iprotocol->portName();
	}
	cfger->updateDispInfo(info);
}
void Screen::activateView(QWidget* child)
{
	attr->active = true;
	box()->activateView(view);
	static_cast<View*>(view)->activate();
	view->activate(child);
}

Attr<int>& Screen::colorMode()
{
	return attr->led.baseColor;
}

Attr<KRect>& Screen::contentRect()
{
	return attr->contentRect;
}

void Screen::exportProjJson(Json& jnode)
{
	attr->exportProjJson(jnode);

	if (iprotocol) iprotocol->exportProjJson(jnode);

	Json jnodes;
	for (INode* child: children) {
		Json jchild;
		child->exportProjJson(jchild);
		jnodes.push_back(jchild);
	}
	jnode["~programs"] = jnodes;
}

QString Screen::exportCoreJson(Json& jnode)
{
	QString ret = attr->exportCoreJson(jnode);
	if (!ret.isEmpty()) return ret;

	if (children.empty()) return QString(tr("Screen \"%1\" has no program!")).arg(attr->name);

	Json jnodes;
	for (INode* child: children) {
		Json jchild;
		ret = child->exportCoreJson(jchild);
		if (!ret.isEmpty()) return ret;
		jnodes.push_back(jchild);
	}
	jnode["programDescribe"] = jnodes;

	return QString();
}

qreal Screen::zoomFactor()
{
	return view->zoomFactor;
}

IScreen::Module& Screen::module()
{
	return attr->led;
}

IScreen::ScreenPara& Screen::screenPara()
{
	return attr->para;
}

IScreen::OnOff& Screen::onOff()
{
	return attr->power;
}

IScreen::SyncTime& Screen::syncTime()
{
	return attr->sync;
}

IScreen::Bright& Screen::bright()
{
	return attr->bright;
}

QString Screen::buildFiles(const QString& dir, bool allEndian)
{
	Json json;

	QString ret = exportCoreJson(json);
	if (!ret.isEmpty()){
		return ret;
	}

	if (allEndian) json["topLevelConfig"]["dataEndian"] = "all";
	json["topLevelConfig"]["outFilePath"] = dir;

	QString jsonFile = dir + "/ALL.json";
	bfs::ofstream ofs{bfs::path{jsonFile.toStdString()},
					  std::ios::out | std::ios::binary | std::ios::trunc};
	ofs << std::setw(4) << json << std::endl;

	std::string buf = json.dump();
	if (!core->build(buf.c_str())) {
		return tr("Failed to create device file!");
	}

	return QString();
}

IProtocol* Screen::protocol()
{
	return iprotocol;
}
