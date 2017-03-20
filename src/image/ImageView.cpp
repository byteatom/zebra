#include "ImageView.h"

#include "AttrItem.h"

ImageView::ImageView(ImageAttr* attr):
	RegionView{attr},
	attr{attr}
{
}

void ImageView::play()
{
	RegionView::play();
	attr->playingRow = 0;
	if (attr->items->count() > 0)
		static_cast<AttrItem*>(attr->items->item(0))->playingPage = 0;
}

void ImageView::draw()
{
	if (attr->items->count() == 0) {
		return;
	}
	QRect rectContent = attr->contentRect->rect();
	QPainter painter(&sight);
	if (!playing) {
		AttrItem* item = static_cast<AttrItem*>(attr->items->currentItem());
		if (item->editingPage < item->pages.size())
			painter.drawPixmap(rectContent.topLeft(), item->pages[item->editingPage]);
	} else {
		AttrItem* item = static_cast<AttrItem*>(attr->items->item(attr->playingRow));
		painter.drawPixmap(rectContent.x(), yFade, item->pages[item->playingPage],
				0, 0, rectContent.width(), rectContent.bottom() - yFade);
		if (--yFade < rectContent.top()) {
			yFade = rectContent.bottom();
			item->playingPage++;
			if (item->playingPage == item->pages.size()) {
				item->playingPage = 0;
				attr->playingRow++;
				if (attr->playingRow == attr->items->count())
					attr->playingRow = 0;
			}
		}
	}
}

void ImageView::stop()
{
	RegionView::stop();
}
