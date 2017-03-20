#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include <QPixmap>

#include "RegionView.h"
#include "TextAttr.h"

class TextView : public RegionView
{
public:
	explicit TextView(TextAttr* attr);
	virtual ~TextView() = default;

private:
	virtual void play() override;
	virtual void draw()override;
	virtual void stop()override;

	TextAttr* attr = nullptr;

	int playingPage = 0;
	int offset = 0;
};

#endif // TEXTVIEW_H
