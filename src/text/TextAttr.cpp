#include "TextAttr.h"

#include <QPainter>

#include "INode.h"
#include "ISetting.h"
#include "IFadeFactory.h"
#include "UtilQt.h"

extern ISetting* setting;
extern IFadeFactory* fadeFactory;

TextAttr::TextAttr(INodeBox* box, Json* jnode):
	RegionAttr{box, INode::Type::TEXT, jnode},
	fade{fadeFactory ? fadeFactory->create(jnode, 3, true) : nullptr}
{
	contentRect.connect([this](){print();});

	if (jnode) {
		jtov(html);
		jtov(bgColor);
		jtov_enum(align);
		jtov(letterSpace);
		jtov(strechHor);
		jtov(strechVer);
		jtov(offset);
	}
}

TextAttr::~TextAttr()
{
	if (fade) delete fade;
}

void TextAttr::print()
{
	pages.clear();

	QRect rectContent = contentRect->rect();

	int y = (rectContent.height() - snapshot.height()) / 2 + offset;
	for (int i = 0; i < snapshot.width(); i+=rectContent.width()) {
		QPixmap page{rectContent.size()};
		page.fill(QColor::fromRgba(bgColor));
		QPainter painter{&page};
		int x = 0;
		if (i + rectContent.width() >= snapshot.width() && align != Qt::AlignLeft) {
			if (align == Qt::AlignRight)
				x = i + rectContent.width() - snapshot.width();
			else if (align == Qt::AlignCenter)
				x = (i + rectContent.width() - snapshot.width()) / 2;
		}
		painter.drawPixmap(x, y, snapshot, i, 0, rectContent.width(), snapshot.height());
		pages.push_back(page);
	}
	editingPage = 0;
}

boost::signals2::connection TextAttr::pageObserve(IRegion::PageObserver observer)
{
	auto slot = [this, observer](){
		observer(pages.size(), editingPage);
	};
	slot();
	return changed.connect(slot);
}

void TextAttr::pageFirst()
{
	if (pages.empty()) return;

	editingPage = 0;

	changed();
}

void TextAttr::pagePrevious()
{
	if (pages.empty()) return;

	if (editingPage > 0)
		editingPage--;
	else
		editingPage = pages.size() - 1;

	changed();
}

void TextAttr::pageNext()
{
	if (pages.empty()) return;

	if (editingPage + 1 < pages.size())
		editingPage++;
	else
		editingPage = 0;

	changed();
}

void TextAttr::pageLast()
{
	if (pages.empty()) return;

	editingPage = pages.size() - 1;

	changed();
}

void TextAttr::exportProjJson(Json& jnode)
{
	RegionAttr::exportProjJson(jnode);

	vtoj(html);
	vtoj(bgColor);
	vtoj(align);
	vtoj(letterSpace);
	vtoj(strechHor);
	vtoj(strechVer);
	vtoj(offset);

	IProjJson* fadeJson = dynamic_cast<IProjJson*>(fade);
	if (fadeJson) fadeJson->exportProjJson(jnode);
}

QString TextAttr::exportCoreJsonArea(Json& jArea)
{
	QRect rectContent = contentRect->rect();

	QString ret;

	Json jFade;
	jFade["width"] = rectContent.width();
	jFade["height"] = rectContent.height();
	if (fade) {
	ICoreJson* fadeJson = dynamic_cast<ICoreJson*>(fade);
	if (fadeJson) {
		ret = fadeJson->exportCoreJson(jFade);
		if (!ret.isEmpty()) return ret;
		}
	}

	if (pages.empty()) return QString(tr("Region \"%1\" is empty!")).arg(name);

	Json jPages;
	if (fade && fade->continuous()) {
		QStr path = UtilQt::uuidFile(setting->tempDir(), ".bmp");
		snapshot.save(path);
		Json jPage = jFade;
		jPage["content"] = path;
		jPages.push_back(jPage);
	} else {
		for (QPixmap& page: pages) {
			QStr path = UtilQt::uuidFile(setting->tempDir(), ".bmp");
			page.save(path);
			Json jPage = jFade;
			jPage["content"] = path;
			jPages.push_back(jPage);
		}
	}
	jArea["pictureFuseArray"] = jPages;

	return QString();
}

