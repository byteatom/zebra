#ifndef FLASHTEXTVIEW_H
#define FLASHTEXTVIEW_H

#include <QStackedLayout>
#include "RegionView.h"
#include "FlashTextAttr.h"
#include "RichEditor.h"
#include "KGShineFont.h"

struct ElementData {
	int startX;
	int startY;
	int picNum;
	int locusNum;
	int cycleCnt;
};

class FlashTextView : public RegionView
{
public:
	explicit FlashTextView(FlashTextAttr* attr);
	~FlashTextView();

private:
	virtual void play() override;
	virtual void draw()override;
	virtual void stop()override;

	FlashTextAttr *attr = nullptr;
	ShineFontAreaInput inputBK;
	ShineFontAreaOutput outputBK;

	int curFrame = 0;
	int speedCnt = 0;
	ElementData *element = nullptr;
};

#endif // CLOCKVIEW_H
