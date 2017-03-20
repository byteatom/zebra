#include "Glow.h"

#include <cstdlib>

#include <QMovie>
#include <QPainter>

#include "GlowCfger.h"
#include "ISetting.h"
#include "UtilQt.h"

extern ISetting* setting;

Glow::Glow(Json* jnode, Attr<KRect>* geometry, Attr<KRect>* contentRect)
	:geometry{geometry}, contentRect{contentRect}
{

	if (jnode) {
		jtov2(effectBg.enabled, "glowBg");
		jtov2(effectBg.speed, "glowBgSpeed");
		jtov2(effectBg.paths, "glowBgPaths");
		jtov2(effectFg.enabled, "glowFg");
		jtov2(effectFg.speed, "glowFgSpeed");
		jtov2(effectFg.paths, "glowFgPaths");
		jtov2(effectFg.paths, "glowFgPaths");
		jtov2(effectFg.overlap, "glowFgOverlap");
	}
}

QWidget* Glow::newCfger()
{
	return new GlowCfger(this);
}

void Glow::cfgChanged(const std::function<void ()>& slot)
{
	changed.connect(slot);
}

void Glow::exportProjJson(Json& jnode)
{
	if (effectBg.enabled) {
		vtoj2(effectBg.enabled, "glowBg");
		vtoj2(effectBg.speed, "glowBgSpeed");
		vtoj2(effectBg.paths, "glowBgPaths");
	}
	if (effectFg.enabled) {
		vtoj2(effectFg.enabled, "glowFg");
		vtoj2(effectFg.speed, "glowFgSpeed");
		vtoj2(effectFg.paths, "glowFgPaths");
		vtoj2(effectFg.overlap, "glowFgOverlap");
	}
}

QString Glow::exportCoreJson(Json& jNodes)
{
	if (effectBg.enabled) {
		Json jPages;
		int pageCount = 0;
		for (QStr& path: effectBg.paths) {
			QMovie movie{path};
			if (!movie.isValid()) continue;
			for (int i = 0; i < movie.frameCount(); ++i) {
				if (!movie.jumpToFrame(i)) break;
				QPixmap dst{(*geometry)->size};
				dst.fill(Qt::transparent);
				QPainter painter(&dst);
				QPixmap src = movie.currentPixmap().scaledToHeight(dst.height(), Qt::SmoothTransformation);
				div_t multi = div(dst.width(), src.width());
				int space = multi.rem / (multi.quot + 1);
				if (multi.quot > 0) {
					for (int i = 0; i < multi.quot; ++i) {
						painter.drawPixmap((i+1)*space + i*src.width(), 0, src);
					}
				} else {
					painter.drawPixmap(-(src.width() - dst.width()) / 2, 0, src);
				}
				QString tmpPath = UtilQt::uuidFile(setting->tempDir(), ".bmp");
				dst.save(tmpPath);
				Json jPage = {
					{"startX", 0},
					{"startY", 0},
					{"width", (*geometry)->size->width()},
					{"height", (*geometry)->size->height()},
					{"content", tmpPath}
				};
				jPages.push_back(jPage);
				++pageCount;
			 }
		}
		Json jBgNode = {
			{"name", "none"},
			{"type", "colorful"},
			{"coordX", (*geometry)->point->x()},
			{"coordY", (*geometry)->point->y()},
			{"coordWidth", (*geometry)->size->width()},
			{"coordHeight", (*geometry)->size->height()},
			{"areaDescribe", {
				{"display", "background"},
				{"speed", effectBg.speed},
				{"pictureTotal", pageCount},
				{"pictureFuseArray", jPages}
			}}
		};
		jNodes.push_back(jBgNode);
	}
	if (effectFg.enabled) {
		int width = (*geometry)->size->width();
		int height = (*geometry)->size->height();
		int coordX = 0;
		int coordY = 0;
		int coordWidth = width;
		int coordHeight = height;
		if (!effectFg.overlap) {
			width = (*contentRect)->size->width();
			height = (*contentRect)->size->height();
			coordX = (*geometry)->point->x() + (*contentRect)->point->x();
			coordY = (*geometry)->point->y() + (*contentRect)->point->y();
			coordWidth = width;
			coordHeight = height;
		}

		Json jPages;
		int pageCount = 0;

		for (QStr& path: effectFg.paths) {
			QMovie movie{path};
			if (!movie.isValid()) continue;
			for (int i = 0; i < movie.frameCount(); ++i) {
				if (!movie.jumpToFrame(i)) break;
				QPixmap pixmap = movie.currentPixmap();
				QString tmpPath = UtilQt::uuidFile(setting->tempDir(), ".bmp");
				pixmap.scaled(width, height).save(tmpPath);
				Json jPage = {
					{"startX", 0},
					{"startY", 0},
					{"width", width},
					{"height", height},
					{"content", tmpPath}
				};
				jPages.push_back(jPage);
				++pageCount;
			 }
		}
		Json jFgNode = {
			{"name", "none"},
			{"type", "colorful"},
			{"coordX", coordX},
			{"coordY", coordY},
			{"coordWidth", coordWidth},
			{"coordHeight", coordHeight},
			{"areaDescribe", {
					{"display", "foreground"},
					{"speed", effectFg.speed},
					{"pictureTotal", pageCount},
					{"pictureFuseArray", jPages}
				}
			}
		};
		jNodes.push_back(jFgNode);
	}

	return QString();
}

void Glow::play()
{

}

void Glow::draw()
{

}

void Glow::stop()
{

}


void Glow::setSurface(QPaintDevice* surface)
{

}
