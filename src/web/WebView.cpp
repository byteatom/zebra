#include "WebView.h"

#include <algorithm>
#include <QWebEngineSettings>

#include "WebManager.h"
#include "WebItem.h"

WebView::WebView(WebAttr* attr):
	RegionView{attr},
	attr{attr},
	webView{new QWebEngineView{this}},
	playIndex{0},
	interval{0},
	count{0}
{
	webView->setContextMenuPolicy(Qt::NoContextMenu);
	webView->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	webView->move(1, 1);
	webView->resize(size() - QSize{2, 2});
	QWebEnginePage* page = webView->page();
	page->setBackgroundColor(Qt::transparent);
	webView->show();

	if (!attr->items.empty()) webView->load(attr->items[playIndex].uri());
}

void WebView::play()
{
	RegionView::play();

	if (attr->items.empty()) return;

	interval = setting->preset().value("drawInterval").toInt();
	count = 0;

	playIndex = 0;
	webView->load(attr->items[playIndex].uri());
}

void WebView::draw()
{
	if (playing) {
		if (attr->items.empty()) return;

		++count;
		if (count * interval < attr->items[playIndex].duration)  return;
		count = 0;
		if (playIndex < attr->items.size() - 1)
			++playIndex;
		else
			playIndex = 0;
		webView->load(attr->items[playIndex].uri());
	} else {
		QString uri = attr->items.empty() ? "" : attr->items[attr->editIndex].uri().toString();
		if (uri != webView->url().url()) {
			webView->load(uri);
			//player->page()->setBackgroundColor(Qt::transparent);
			//player->setPage(nullptr);
		}
	}
}

void WebView::stop()
{
	RegionView::stop();

	if (attr->items.empty()) return;

	webView->stop();
	playIndex = 0;
}

void WebView::resizeEvent(QResizeEvent *event)
{
	webView->resize(event->size() - QSize{2, 2});
}
