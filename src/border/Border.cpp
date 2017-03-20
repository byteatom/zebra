#include "Border.h"
#include <QPainter>

BorderData::BorderData(Json* jnode)
{
	attr = jnode ? new BorderAttr{*jnode} : new BorderAttr{};
}

BorderData::~BorderData()
{
	delete attr;
}

Border::Border(Json* jnode, ISetting *setting):
	BorderData{jnode}
{
	attr->borderDir = "image/border";
	attr->createDir = setting->tempDir();
}

void Border::exportProjJson(Json& jnode)
{
	attr->exportProjJson(jnode);
}

QString Border::exportCoreJson(Json& jnode)
{
	return attr->exportCoreJson(jnode);
}

void Border::setSurface(QPaintDevice* surface)
{
	attr->sight = static_cast<QPixmap*>(surface);
}

int Border::width()
{
	if (attr->checked)
		return attr->image.height();

	return 0;
}

void Border::play()
{
	attr->playing = true;
	attr->step = 0;
}

void Border::draw()
{
	int regionWidth = attr->sight->width();
	int regionHeight = attr->sight->height();
	int imageWidth = attr->image.width();
	int imageHeight = attr->image.height();

	if (regionWidth == 0 || regionHeight == 0 || imageWidth == 0 || imageHeight == 0)
		return;

	if (attr->checked)
	{
		QPainter painter(attr->sight);
		QRectF dst1, src1, dst2, src2;
		int start, imageW, imageH;

		// direct >= 3 is flick
		if (attr->direct >= 3 && attr->step%4 >= 2)
		{
			painter.setBackground(Qt::transparent);
		}
		//static
		else if (attr->direct%3 == 0)
		{
			imageW = imageWidth;
			imageH = imageHeight;
			start = 0;
			for (;;)
			{
				dst1 = QRectF(start, 0, imageW, imageH);
				src1 = QRectF(0, 0, imageW, imageH);
				dst2 = QRectF(regionWidth - start - imageW, regionHeight - imageH, imageW, imageH);
				src2 = QRectF(0, 0, imageW, imageH);
				start += imageW;
				painter.drawPixmap(dst1, attr->pixUp, src1);
				painter.drawPixmap(dst2, attr->pixDown, src2);
				if (start >= regionWidth) break;
			}

			imageW = imageHeight;
			imageH = imageWidth;
			start = 0;
			for (;;)
			{
				dst1 = QRectF(regionWidth - imageW, start, imageW, imageH);
				src1 = QRectF(0, 0, imageW, imageH);
				dst2 = QRectF(0, regionHeight - start - imageH, imageW, imageH);
				src2 = QRectF(0, 0, imageW, imageH);
				start += imageH;
				painter.drawPixmap(dst1, attr->pixRight, src1);
				painter.drawPixmap(dst2, attr->pixLeft, src2);
				if (start >= regionHeight) break;
			}
		}
		//clockwise
		else if (attr->direct%3 == 1)
		{
			imageW = imageWidth;
			imageH = imageHeight;
			start = 0;
			for (;;)
			{
				if (start == 0) {
					dst1 = QRectF(0, regionHeight - imageH, imageW - attr->step, imageH);
					src1 = QRectF(attr->step, 0, imageW - attr->step, imageH);
					dst2 = QRectF(regionWidth - imageW + attr->step, 0, imageW - attr->step, imageH);
					src2 = QRectF(0, 0, imageW - attr->step, imageH);
					start += imageW - attr->step;
				} else {
					dst1 = QRectF(start, regionHeight - imageH, imageW, imageH);
					src1 = QRectF(0, 0, imageW, imageH);
					dst2 = QRectF(regionWidth - start - imageW, 0, imageW, imageH);
					src2 = QRectF(0, 0, imageW, imageH);
					start += imageW;
				}
				painter.drawPixmap(dst1, attr->pixDown, src1);
				painter.drawPixmap(dst2, attr->pixUp, src2);
				if (start >= regionWidth) break;
			}

			imageW = imageHeight;
			imageH = imageWidth;
			start = 0;
			for (;;)
			{
				if (start == 0) {
					dst1 = QRectF(0, 0, imageW, imageH - attr->step);
					src1 = QRectF(0, attr->step, imageW, imageH - attr->step);
					dst2 = QRectF(regionWidth - imageW, regionHeight - imageH + attr->step, imageW, imageH - attr->step);
					src2 = QRectF(0, 0, imageW, imageH - attr->step);
					start += imageH - attr->step;
				} else {
					dst1 = QRectF(0, start, imageW, imageH);
					src1 = QRectF(0, 0, imageW, imageH);
					dst2 = QRectF(regionWidth - imageW, regionHeight - start - imageH, imageW, imageH);
					src2 = QRectF(0, 0, imageW, imageH);
					start += imageH;
				}
				painter.drawPixmap(dst1, attr->pixLeft, src1);
				painter.drawPixmap(dst2, attr->pixRight, src2);
				if (start >= regionHeight) break;
			}
		}
		//anti clockwise
		else if (attr->direct%3 == 2)
		{
			imageW = imageWidth;
			imageH = imageHeight;
			start = 0;
			for (;;)
			{
				if (start == 0) {
					dst1 = QRectF(0, 0, imageW - attr->step, imageH);
					src1 = QRectF(attr->step, 0, imageW - attr->step, imageH);
					dst2 = QRectF(regionWidth - imageW + attr->step, regionHeight - imageH, imageW - attr->step, imageH);
					src2 = QRectF(0, 0, imageW - attr->step, imageH);
					start += imageW - attr->step;
				} else {
					dst1 = QRectF(start, 0, imageW, imageH);
					src1 = QRectF(0, 0, imageW, imageH);
					dst2 = QRectF(regionWidth - start - imageW, regionHeight - imageH, imageW, imageH);
					src2 = QRectF(0, 0, imageW, imageH);
					start += imageW;
				}
				painter.drawPixmap(dst1, attr->pixUp, src1);
				painter.drawPixmap(dst2, attr->pixDown, src2);
				if (start >= regionWidth) break;
			}

			imageW = imageHeight;
			imageH = imageWidth;
			start = 0;
			for (;;)
			{
				if (start == 0) {
					dst1 = QRectF(regionWidth - imageW, 0, imageW, imageH - attr->step);
					src1 = QRectF(0, attr->step, imageW, imageH - attr->step);
					dst2 = QRectF(0, regionHeight - imageH + attr->step, imageW, imageH - attr->step);
					src2 = QRectF(0, 0, imageW, imageH - attr->step);
					start += imageH - attr->step;
				} else {
					dst1 = QRectF(regionWidth - imageW, start, imageW, imageH);
					src1 = QRectF(0, 0, imageW, imageH);
					dst2 = QRectF(0, regionHeight - start - imageH, imageW, imageH);
					src2 = QRectF(0, 0, imageW, imageH);
					start += imageH;
				}
				painter.drawPixmap(dst1, attr->pixRight, src1);
				painter.drawPixmap(dst2, attr->pixLeft, src2);
				if (start >= regionHeight) break;
			}
		}

		if (attr->playing) {
			if (++attr->speedCnt > attr->speed) {
				attr->speedCnt = 0;
				if (++attr->step >= imageWidth)
					attr->step = 0;
			}
		}
	}
}

void Border::stop()
{
	attr->playing = false;
	attr->step = 0;
}

void Border::cfgChanged(const std::function<void ()>& slot)
{
	attr->notify.connect(slot);
}

QWidget* Border::newCfger()
{
	BorderData::cfger = new BorderCfger{attr};;
	return BorderData::cfger;
}

