#include "PictureItem.h"

#include <QFileInfo>

#include "ImageAttr.h"
#include "PictureEditor.h"
#include "UtilQt.h"

const QString PictureItem::filter()
{
	return UtilQt::imageFilters();
}

PictureItem::PictureItem(ImageAttr* attr, const QString& file, Json* jnode):
	AttrItem{attr, Type::PICTURE, QFileInfo(file).fileName(), jnode},
	fileItem{file, jnode}
{
}

void PictureItem::exportProjJson(Json& jnode)
{
	AttrItem::exportProjJson(jnode);
	fileItem.exportProjJson(jnode);
}

bool PictureItem::editImpl()
{
	/*PictureEditor editor{listWidget(), attr->contentRect->size, this};
	return QDialog::Accepted == editor.exec();*/
	return false;
}

void PictureItem::printImpl()
{
	pages.clear();
	editingPage = 0;

	QPixmap src{fileItem.file};
	if (src.isNull()) return;
	QPixmap dst = UtilQt::scale(src, UtilQt::STRECH, attr->contentRect->size);
	pages.push_back(dst);
}
