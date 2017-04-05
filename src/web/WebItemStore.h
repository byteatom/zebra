#ifndef WEBITEMSTORE_H
#define WEBITEMSTORE_H

#include <QDialog>
#include <WebItemListWidget.h>

#include "WebAttr.h"

namespace Ui {
class WebItemStore;
}

class WebItemStore : public QDialog
{
	Q_OBJECT

public:
	explicit WebItemStore(QWidget *parent, WebAttr* attr);
	virtual ~WebItemStore();

private:

	static const int barStep = 64;
	Ui::WebItemStore *ui;
	WebItemListWidgetItem* shelfHoverItem;
	QWidget* shelfBar;
	WebItemListWidgetItem* cartHoverItem;
	QWidget* cartBar;
	WebAttr* attr;

	static QWidget* createBar(QWidget* parent);
	static QPushButton* addBarBtn(QWidget* bar, const QString& iconPath);
	static void showBar(QListWidgetItem* item, QWidget* bar);

	void shelfItemEntered(QListWidgetItem* item);
	void shelfItemNull();
	void add();
	void shelfPlay();
	void cartItemEntered(QListWidgetItem* item);
	void cartItemNull();
	void remove();
	void cartPlay();

	void ok();
};

#endif // WEBITEMSTORE_H
