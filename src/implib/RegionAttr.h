#ifndef REGIONATTR_H
#define REGIONATTR_H

#include <chrono>
#include <random>

#include <QResizeEvent>
#include <QRect>
#include <QSize>

#include "NodeAttr.h"
#include "IBorder.h"
#include "INodeBox.h"
#include "INode.h"
#include "Attr.h"
#include "IScreen.h"
#include "IProgram.h"
#include "IBorderFactory.h"
#include "IGlowFactory.h"

extern IBorderFactory* borderFactory;
extern IGlowFactory* glowFactory;

class RegionAttr : public NodeAttr
{
public:
	enum {MIN_SIZE = 8};

	Attr<KRect> geometry;
	Attr<KRect> contentRect;

	IBorder* border;
	IGlow* glow;

	explicit RegionAttr(INodeBox* box, int type, Json* jnode):
		NodeAttr{box, type, jnode},
		geometry{&geometry},
		contentRect{&contentRect},
		border{borderFactory->create(jnode)},
		glow{glowFactory->create(jnode, &geometry, &contentRect)}
	{
		boxRectConn = box->contentRect().connect([this]() {
			const KRect& boxRect = this->box->contentRect().ref();
			QRect geo{geometry->point, geometry->size};
			if (geometry->point->x() < boxRect.point->x()) {
				geo.moveLeft(boxRect.point->x());
			}
			if (geometry->point->y() < boxRect.point->y()) {
				geo.moveTop(boxRect.point->y());
			}
			if (geometry->rightReal() > boxRect.rightReal()) {
				if (geometry->size->width() > boxRect.size->width()) {
					geo.moveLeft(boxRect.point->x());
					geo.setWidth(boxRect.size->width());
				} else {
					geo.moveLeft(boxRect.point->x() + boxRect.size->width() - geometry->size->width());
				}
			}
			if (geometry->bottomReal() > boxRect.bottomReal()) {
				if (geometry->size->height() > boxRect.size->height()) {
					geo.moveTop(boxRect.point->y());
					geo.setHeight(boxRect.size->height());
				} else {
					geo.moveTop(boxRect.point->y() + boxRect.size->height() - geometry->size->height());
				}
			}
			geometry->setRect(geo);
		});
		auto slot = [this](){updateContentRect();};
		geometry.connect(slot);
		border->cfgChanged(slot);

		const Attr<KRect>& boxRect = box->contentRect();
		int width = boxRect->size->width() / 2;
		int height = boxRect->size->height() / 2;
		int x = boxRect->point->x();
		int y = boxRect->point->y();
		/*int x = width / 2 + boxRect->point->x();
		int y = height / 2 + boxRect->point->y();*/

		if (jnode) {
			jtov(x);
			jtov(y);
			jtov(width);
			jtov(height);
		} else {
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
			std::default_random_engine engine{seed};
			std::uniform_int_distribution<int> xDist(x, x + width);
			x = xDist(engine);
			std::uniform_int_distribution<int> yDist(y, y + height);
			y = yDist(engine);
		}

		geometry->setRect(x, y, width, height);
	}

	virtual ~RegionAttr()
	{
		boxRectConn.disconnect();
		delete border;
		delete glow;
	}

	void exportProjJson(Json& jnode)
	{
		NodeAttr::exportProjJson(jnode);

		jnode["x"] = geometry->point->x();
		jnode["y"] = geometry->point->y();
		jnode["width"] = geometry->size->width();
		jnode["height"] = geometry->size->height();

		border->exportProjJson(jnode);
		glow->exportProjJson(jnode);
	}

	QString exportCoreJson(Json& jNodes)
	{
		static const char* typeStr[INode::Type::MAX_TYPE] {
			nullptr,
			nullptr,
			"word",
			"word",
			"watch",
			"time",
			"count",
			"shine",
			"humiture",
			"variable"
		};

		Json jArea;
		QString ret = exportCoreJsonArea(jArea);
		if (!ret.isEmpty()) return ret;
		Json jNode = {
			{"name", "none"},
			{"type", typeStr[type]},
			{"coordX", geometry->point->x()},
			{"coordY", geometry->point->y()},
			{"coordWidth", geometry->size->width()},
			{"coordHeight", geometry->size->height()},
			{"areaDescribe", jArea}
		};

		ret = border->exportCoreJson(jNode);
		if (!ret.isEmpty()) return ret;

		jNodes.push_back(jNode);

		ret = glow->exportCoreJson(jNodes);
		if (!ret.isEmpty()) return ret;

		return QString();
	}

	virtual QString exportCoreJsonArea(Json& jnode) = 0;

	IScreen* screen()
	{
		INode* program = dynamic_cast<INode*>(box);
		if (program) {
			INodeBox* screen = program->box();
			if (screen) {
				return dynamic_cast<IScreen*>(screen);
			}
		}
		return nullptr;
	}

	IProgram* program()
	{
		INode* program = dynamic_cast<INode*>(box);
		if (program) {
			return dynamic_cast<IProgram*>(program);
		}
		return nullptr;
	}

private:
	void updateContentRect()
	{
		int borderWidth = border->width();

		contentRect->setRect(borderWidth, borderWidth,
							 geometry->size->width() - borderWidth * 2,
							 geometry->size->height() - borderWidth * 2);
	}

	boost::signals2::connection boxRectConn;
};

#endif // REGIONATTR_H
