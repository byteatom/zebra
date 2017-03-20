#include "Text.h"

TextData::TextData(INodeBox* box, Json* jnode)
{
	attr = new TextAttr{box, jnode},
	view = new TextView{attr},
	cfger = new TextCfger{attr};
}

TextData::~TextData()
{
	delete cfger;
	delete view;
	delete attr;
}

Text::Text(INodeBox *box, Json* jnode):
	TextData{box, jnode},
	Region{attr, view, cfger}
{

}

void Text::exportProjJson(Json& jnode)
{
	attr->exportProjJson(jnode);
}

void Text::play()
{
	Region::play();
}

void Text::draw()
{
	Region::draw();
}

void Text::stop()
{
	Region::stop();
}

boost::signals2::connection Text::pageObserve(PageObserver observer)
{
	return attr->pageObserve(observer);
}

void Text::pageFirst()
{
	return attr->pageFirst();
}

void Text::pagePrevious()
{
	return attr->pagePrevious();
}

void Text::pageNext()
{
	return attr->pageNext();
}

void Text::pageLast()
{
	return attr->pageLast();
}
