#include "FlashTextView.h"

#include <QtGlobal>
#include <QtMath>
#include <QTime>
#include <QStaticText>
#include <QPainter>

FlashTextView::FlashTextView( FlashTextAttr* attr):
	RegionView{attr},
	attr{attr}
{
	memset(&inputBK, 0, sizeof(inputBK));
	memset(&outputBK, 0, sizeof(outputBK));
}

FlashTextView::~FlashTextView()
{
}

void FlashTextView::play()
{
	RegionView::play();
	attr->playing = true;
	curFrame = 0;
	speedCnt = 0;
}

void FlashTextView::draw()
{
	if (attr->playing == false) {
		attr->toInputAttr(KG_CREATE_MEMORY);
		freeShineFontArea(&outputBK);
		int bkStyle = attr->input.style;
		attr->input.style = 0;
		shineFontAreaWork(&attr->input, &outputBK);
		attr->input.style = bkStyle;

		QPainter painter(&sight);
		for (int i = 0; i < outputBK.elementTotal; ++i) {
			int picNum = outputBK.elementPtr[i].startPictureNumber;
			QImage image(outputBK.picturePtr[picNum].bitmap.data,
						 outputBK.picturePtr[picNum].bitmap.width,
						 outputBK.picturePtr[picNum].bitmap.height,
						 QImage::Format_ARGB32);
			painter.drawPixmap(attr->contentRect->point->x() + outputBK.elementPtr[i].startX,
							   attr->contentRect->point->y() + outputBK.elementPtr[i].startY,
							   QPixmap::fromImage(image), 0, 0,
							   outputBK.picturePtr[picNum].bitmap.width,
							   outputBK.picturePtr[picNum].bitmap.height);
		}
	} else {
		bool change = false;

		attr->toInputAttr(KG_CREATE_MEMORY);
		if (memcmp(&inputBK, &attr->input, sizeof(inputBK)) || curFrame == 0) {
			freeShineFontArea(&outputBK);
			shineFontAreaWork(&attr->input, &outputBK);
			memcpy(&inputBK, &attr->input, sizeof(inputBK));
			change = true;
		}

		if (!element || change) {
			if (change) delete[] element;
			element = new ElementData[outputBK.elementTotal];
			memset(element, 0, sizeof(ElementData) * outputBK.elementTotal);
		}

		++speedCnt;

		for (int i = 0; i<outputBK.elementTotal; i++) {
			ShineElement *curElement = &outputBK.elementPtr[i];
			ShineLocus *curLocus = &curElement->locusPtr[element[i].locusNum];
			QImage *image = nullptr;

			if(curFrame > 0) {
				if(curLocus->nextPic == 1){
					element[i].picNum = curLocus->pictureNumber;
				} else {
					if (speedCnt >= attr->speed) {
						element[i].picNum += curLocus->pictureNumber;
					}
				}
			} else {
				element[i].picNum = curElement->startPictureNumber;
			}

			if (element[i].picNum >= 0) {
				image = new QImage(outputBK.picturePtr[element[i].picNum].bitmap.data,
							 outputBK.picturePtr[element[i].picNum].bitmap.width,
							 outputBK.picturePtr[element[i].picNum].bitmap.height,
							 QImage::Format_ARGB32);
			} else {
				if(curFrame > 0) {
					if (speedCnt >= attr->speed) {
						if (curLocus->nextPic == 1) {
							if(++element[i].locusNum >= curElement->locusTotal)
								element[i].locusNum = curElement->locusTotal - 1;
						} else {
							if(++element[i].cycleCnt > curLocus->recycleTotal) {
								element[i].cycleCnt = 0;
								if(++element[i].locusNum >= curElement->locusTotal)
									element[i].locusNum = curElement->locusTotal - 1;
							}
						}
					}
				}
				continue;
			}

			int x,y,width,height;
			if(curFrame == 0) {
				x = attr->input.coord.x + curElement->startX;
				y = attr->input.coord.y + curElement->startY;
			} else {
				x = attr->input.coord.x + curElement->startX + curLocus->offsetX;
				y = attr->input.coord.y + curElement->startY + curLocus->offsetY;
			}
			width  = outputBK.picturePtr[element[i].picNum].width;
			height = outputBK.picturePtr[element[i].picNum].height;

			if (image) {
				QPainter painter(&sight);
				painter.drawPixmap(x, y, QPixmap::fromImage(*image), 0, 0, width, height);
				delete image;
			}

			if (curFrame > 0) {
				if (speedCnt >= attr->speed) {
					if (element[i].locusNum < curElement->locusTotal) {
						curElement->startX += curLocus->offsetX;
						curElement->startY += curLocus->offsetY;
					}
				}
			}

			if (curFrame > 0) {
				if (speedCnt >= attr->speed) {
					if(curLocus->nextPic == 1) {
						if (++element[i].locusNum >= curElement->locusTotal)
							element[i].locusNum = curElement->locusTotal - 1;
					} else {
						if (++element[i].cycleCnt > curLocus->recycleTotal) {
							element[i].cycleCnt = 0;
							if(++element[i].locusNum >= curElement->locusTotal)
								element[i].locusNum = curElement->locusTotal - 1;
						}
					}
				}
			}
		}

		if (curFrame > 0) {
			if (speedCnt >= attr->speed) {
				speedCnt = 0;
				if (++curFrame > outputBK.frameTotal){
					curFrame = 0;
					memset(element, 0, sizeof(ElementData) * outputBK.elementTotal);
				}
			}
		} else {
			if (++curFrame > outputBK.frameTotal){
				curFrame = 0;
				memset(element, 0, sizeof(ElementData) * outputBK.elementTotal);
			}
		}
	}
}

void FlashTextView::stop()
{
	RegionView::stop();
	attr->playing = false;
	if (element) {
		delete[] element;
		element = nullptr;
	}
}
