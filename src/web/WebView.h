#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QPixmap>
#include <QWebEngineView>

#include "RegionView.h"
#include "WebAttr.h"

class WebView : public RegionView
{
public:
	explicit WebView(WebAttr* attr);
	virtual ~WebView() = default;

private:
	virtual void play() override;
	virtual void draw() override;
	virtual void stop() override;
	virtual void resizeEvent(QResizeEvent *event) override;

	WebAttr* attr = nullptr;
	QWebEngineView *webView = nullptr;
	int playIndex;
	int interval;
	int count;
};

#endif // WEBVIEW_H
