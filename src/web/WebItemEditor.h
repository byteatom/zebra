#ifndef WEBPLAYER_H
#define WEBPLAYER_H

#include <QDialog>
#include <QWebEngineView>

#include <WebItem.h>

namespace Ui {
class WebItemEditor;
}

class WebItemEditor : public QDialog
{
	Q_OBJECT

public:
	explicit WebItemEditor(QWidget *parent, const QSize& size, WebItem& item);
	virtual ~WebItemEditor();

private:
	Ui::WebItemEditor *ui;
	WebItem& item;
	std::string lang;

	void addQuery(Json::iterator it);
	QUrlQuery getQuery();
	void refresh();
	void ok();
};

#endif // WEBPLAYER_H
