#include "Image.h"

ImageData::ImageData(INodeBox* box, Json* jnode)
{
	attr = new ImageAttr{box, jnode},
	view = new ImageView{attr},
	cfger = new ImageCfger{attr};
}

ImageData::~ImageData()
{
	delete cfger;
	delete view;
	delete attr;
}

Image::Image(INodeBox *box, Json* jnode):
	ImageData{box, jnode},
	Region{attr, view, cfger}
{

}

void Image::userDelete()
{
	attr->userDelete();
}

void Image::exportProjJson(Json& jnode)
{
	attr->exportProjJson(jnode);
}

void Image::play()
{
	Region::play();
}

void Image::draw()
{
	Region::draw();
}

void Image::stop()
{
	Region::stop();
}

boost::signals2::connection Image::pageObserve(PageObserver observer)
{
	return attr->pageObserve(observer);
}

void Image::pageFirst()
{
	return attr->pageFirst();
}

void Image::pagePrevious()
{
	return attr->pagePrevious();
}

void Image::pageNext()
{
	return attr->pageNext();
}

void Image::pageLast()
{
	return attr->pageLast();
}
