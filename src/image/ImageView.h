#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include <QPixmap>

#include "RegionView.h"
#include "ImageAttr.h"

class ImageView : public RegionView
{
public:
	explicit ImageView(ImageAttr* attr);
	virtual ~ImageView() = default;

private:
	virtual void play() override;
	virtual void draw()override;
	virtual void stop()override;

	ImageAttr* attr = nullptr;

	int yFade = 0;
};

#endif // TEXTVIEW_H
