#include "TextView.h"

TextView::TextView(TextAttr* attr):
	RegionView{attr},
	attr{attr}
{
}

void TextView::play()
{
	RegionView::play();

	playingPage = 0;
	offset = 0;
}

void TextView::draw()
{
	if (attr->pages.size() == 0) return;
	QRect rectContent = attr->contentRect->rect();
	QPainter painter(&sight);
	painter.drawPixmap(rectContent.x(), rectContent.y(),
					   attr->pages[playing ? playingPage : attr->editingPage],
					   offset, 0, rectContent.width(), rectContent.height());
	if (playing) {
		if (++offset == rectContent.width()) {
			if (playingPage + 1 < attr->pages.size())
				playingPage++;
			else
				playingPage = 0;
			offset = 0;
		}
	}
}

void TextView::stop()
{
	playingPage = 0;
	offset = 0;
	RegionView::stop();
}
