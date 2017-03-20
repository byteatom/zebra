#include "SheetItem.h"

#include <QPainter>

#include "ImageAttr.h"
#include "SheetEditor.h"

SheetItem::SheetItem(ImageAttr* attr, Json* jnode):
	CacheItem{attr, Type::SHEET, tr("Sheet %1").arg(attr->applyId()), jnode}
{
	if (jnode) {
		jtov(html);
		jtov(rowHeight);
		jtov(colWidth);
	}
}

void SheetItem::exportProjJson(Json& jnode)
{
	CacheItem::exportProjJson(jnode);
	vtoj(html);
	vtoj(rowHeight);
	vtoj(colWidth);
}

bool SheetItem::editImpl()
{
	SheetEditor editor{listWidget(), attr->contentRect->size, this};
	return QDialog::Accepted == editor.exec();
}

void SheetItem::printImpl()
{
	pages.clear();
	editingPage = 0;
	if (snaps.empty()) return;
	QPixmap& snap = snaps.front();
	QSize contentSize = attr->contentRect->size;
	for (int y = 0; y < snap.height(); y+=contentSize.height()) {
		QPixmap page{attr->contentRect->size};
		page.fill(Qt::transparent);
		QPainter pagePainter{&page};
		pagePainter.drawPixmap(0, 0, snap, 0, y, contentSize.width(), contentSize.height());
		pages.push_back(page);
	}
}

