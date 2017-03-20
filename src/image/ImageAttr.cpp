#include "ImageAttr.h"

#include "INode.h"
#include "TextItem.h"
#include "PictureItem.h"
#include "SheetItem.h"
#include "WordItem.h"
#include "PptItem.h"
#include "ExcelItem.h"
#include "ISetting.h"
#include "UtilQt.h"

extern ISetting* setting;

ImageAttr::ImageAttr(INodeBox* box, Json* jnode):
	RegionAttr{box, INode::Type::IMAGE, jnode},
	items{new QListWidget}
{
	contentRect.connect([this](){
		for (int i = 0; i < items->count(); ++i) {
			AttrItem* attrItem = static_cast<AttrItem*>(items->item(i));
			attrItem->print();
		}
	});

	if (jnode) {
		jtov(itemNameId);

		Json::iterator jitNode;
		jitNode = jnode->find("items");
		if (jitNode != jnode->end())	{
			Json jitems = *jitNode;
			for (Json::iterator jitItem = jitems.begin(); jitItem != jitems.end(); ++jitItem) {
				Json jitem  = *jitItem;
				Json::iterator jitType = jitem.find("type");
				if (jitType == jitem.end()) continue;
				int type = *jitType;
				AttrItem* item = nullptr;
				switch (type) {
				case AttrItem::Type::TEXT:
					item = new TextItem(this, &jitem);
					break;
				case AttrItem::Type::PICTURE:
					item = new PictureItem(this, "", &jitem);
					break;
				case AttrItem::Type::SHEET:
					item = new SheetItem(this, &jitem);
					break;
				case AttrItem::Type::WORD:
					item = new WordItem(this, "", &jitem);
					break;
				case AttrItem::Type::PPT:
					item = new PptItem(this, "", &jitem);
					break;
				case AttrItem::Type::EXCEL:
					item = new ExcelItem(this, "", &jitem);
					break;
				default:
					break;
				}
				if (item) item->print();
			}
		}
	}
}

ImageAttr::~ImageAttr()
{
}

void ImageAttr::userDelete()
{
	for (int i = 0; i < items->count(); ++i) {
		AttrItem* attrItem = static_cast<AttrItem*>(items->item(i));
		attrItem->clearCaches();
	}
}

int ImageAttr::applyId()
{
	return ++itemNameId;
}

boost::signals2::connection ImageAttr::pageObserve(IRegion::PageObserver observer)
{
	auto slot = [this, observer](){
		int count = 0;
		for (int i = 0; i < items->count(); ++i) {
			AttrItem* attrItem = static_cast<AttrItem*>(items->item(i));
			count += attrItem->pages.size();
		}

		int index = 0;
		for (int i = 0; i < items->count(); ++i) {
			AttrItem* attrItem = static_cast<AttrItem*>(items->item(i));
			if (i < items->currentRow()) {
				index += attrItem->pages.size();
			} else {
				index += attrItem->editingPage;
				break;
			}
		}
		observer(count, index);
	};
	slot();
	return changed.connect(slot);
}

void ImageAttr::pageFirst()
{
	if (items->count() == 0) return;

	items->setCurrentRow(0);
	static_cast<AttrItem*>(items->item(0))->editingPage = 0;
	changed();
}

void ImageAttr::pagePrevious()
{
	if (items->count() == 0) return;

	AttrItem* currentItem = static_cast<AttrItem*>(items->currentItem());
	if (currentItem->editingPage > 0)
		currentItem->editingPage -= 1;
	else {
		if (items->currentRow() > 0)
			items->setCurrentRow(items->currentRow() - 1);
		else
			items->setCurrentRow(items->count() - 1);
		currentItem = static_cast<AttrItem*>(items->currentItem());
		currentItem->editingPage = currentItem->pages.size() - 1;
	}
	changed();
}

void ImageAttr::pageNext()
{
	if (items->count() == 0) return;

	AttrItem* currentItem = static_cast<AttrItem*>(items->currentItem());
	if (currentItem->editingPage + 1 < currentItem->pages.size())
		currentItem->editingPage += 1;
	else {
		if (items->currentRow() + 1 < items->count())
			items->setCurrentRow(items->currentRow() + 1);
		else
			items->setCurrentRow(0);
		currentItem = static_cast<AttrItem*>(items->currentItem());
		currentItem->editingPage = 0;
	}
	changed();
}

void ImageAttr::pageLast()
{
	if (items->count() == 0) return;

	items->setCurrentRow(items->count() - 1);
	AttrItem* currentItem = static_cast<AttrItem*>(items->currentItem());
	currentItem->editingPage = currentItem->pages.size() - 1;
	changed();
}

void ImageAttr::exportProjJson(Json& jnode)
{
	RegionAttr::exportProjJson(jnode);

	vtoj(itemNameId);

	Json jitems;
	for (int i = 0; i < items->count(); ++i) {
		AttrItem* attrItem = static_cast<AttrItem*>(items->item(i));
		Json jitem;
		attrItem->exportProjJson(jitem);
		jitems.push_back(jitem);
	}
	jnode["items"] = jitems;
}

QString ImageAttr::exportCoreJsonArea(Json& jArea)
{
	if (items->count() == 0) return QString(tr("Region \"%1\" is empty!")).arg(name);

	QRect rectContent = contentRect->rect();

	Json jpages;
	for (int i = 0; i < items->count(); ++i) {
		AttrItem* attrItem = static_cast<AttrItem*>(items->item(i));
		for (QPixmap& page: attrItem->pages) {
			Json jpage;
			jpage["width"] = rectContent.width();
			jpage["height"] = rectContent.height();
			ICoreJson* core = dynamic_cast<ICoreJson*>(attrItem->fade);
			if (core) {
				QString ret = core->exportCoreJson(jpage);
				if (!ret.isEmpty()) return ret;
			}

			QStr path = UtilQt::uuidFile(setting->tempDir(), ".bmp");
			page.save(path);
			jpage["content"] = path;
			jpages.push_back(jpage);
		}
	}
	jArea["pictureFuseArray"] = jpages;

	return QString();
}

