#include "WebItemStore.h"
#include "ui_WebItemStore.h"

#include <QApplication>
#include <QScreen>

#include "WebItemEditor.h"

WebItemStore::WebItemStore(QWidget *parent, WebAttr* attr) :
	QDialog(parent),
	ui(new Ui::WebItemStore),
	shelfHoverItem{nullptr},
	cartHoverItem{nullptr},
	attr{attr}
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	static const int margin = 100;
	QSize size = qApp->primaryScreen()->size();
	setGeometry(margin, margin, size.width() - margin * 2, size.height() - margin * 2);

	ui->shelf->setIconSize({WebItem::thumbWidth, WebItem::thumbHeight});
	ui->shelf->setStyleSheet("QListView::item {background-color: lightGray;}");
	WebItems shelf = WebItem::origin();
	for (const WebItem& item: shelf) {
		new WebItemListWidgetItem{item, ui->shelf};
	}
	connect(ui->shelf, &WebItemListWidget::itemEntered, this, &WebItemStore::shelfItemEntered);
	connect(ui->shelf, &WebItemListWidget::itemNull, this, &WebItemStore::shelfItemNull);

	shelfBar = createBar(ui->shelf);
	connect(addBarBtn(shelfBar, "image/icon/check_128.png"), &QPushButton::clicked,
			this, &WebItemStore::add);
	connect(addBarBtn(shelfBar, "image/icon/play_128.png"), &QPushButton::clicked,
			this, &WebItemStore::shelfPlay);

	ui->cart->setFixedWidth(WebItem::thumbWidth + 30);
	ui->cart->setIconSize({WebItem::thumbWidth, WebItem::thumbHeight});
	ui->cart->setStyleSheet("QListView::item {background-color: lightGray;}");
	for (const WebItem& item: attr->items) {
		new WebItemListWidgetItem{item, ui->cart};
	}
	connect(ui->cart, &WebItemListWidget::itemEntered, this, &WebItemStore::cartItemEntered);
	connect(ui->cart, &WebItemListWidget::itemNull, this, &WebItemStore::cartItemNull);

	cartBar = createBar(ui->cart);
	connect(addBarBtn(cartBar, "image/icon/remove_128.png"), &QPushButton::clicked,
			this, &WebItemStore::remove);
	connect(addBarBtn(cartBar, "image/icon/play_128.png"), &QPushButton::clicked,
			this, &WebItemStore::cartPlay);

	connect(ui->ok, &QPushButton::clicked, this, &WebItemStore::ok);
	connect(ui->cancel, &QPushButton::clicked, this, &WebItemStore::reject);
}

WebItemStore::~WebItemStore()
{
	delete ui;
}

QWidget* WebItemStore::createBar(QWidget* parent)
{
	QWidget* bar = new QWidget{parent};
	bar->hide();
	bar->setStyleSheet("background-color: rgba(20, 20, 20, 50%);");
	bar->resize(0, barStep);
	return bar;
}

QPushButton* WebItemStore::addBarBtn(QWidget* bar, const QString& iconPath)
{
	QPushButton* btn = new QPushButton{QIcon{iconPath}, nullptr, bar};
	btn->setGeometry(bar->width(), 0, barStep, barStep);
	btn->setIconSize({barStep - 16, barStep - 16});
	bar->resize(bar->width() + barStep, barStep);
	return btn;
}

void WebItemStore::showBar(QListWidgetItem* item, QWidget* bar)
{
	QRect rect = item->listWidget()->visualItemRect(item);
	int x = rect.x() + (rect.width() - bar->width()) / 2;
	int y = rect.y() + (rect.height() - bar->height()) / 2;
	bar->move(x, y);
	bar->show();
}

void WebItemStore::shelfItemEntered(QListWidgetItem* item)
{
	if (!item) {
		shelfItemNull();
		return;
	}
	shelfHoverItem = static_cast<WebItemListWidgetItem*>(item);
	showBar(item, shelfBar);
}

void WebItemStore::shelfItemNull()
{
	shelfBar->hide();
	shelfHoverItem = nullptr;
}

void WebItemStore::add()
{
	if (!shelfHoverItem) return;
	WebItemListWidgetItem* item = new WebItemListWidgetItem{*shelfHoverItem};
	ui->cart->addItem(item);
}

void WebItemStore::shelfPlay()
{

}

void WebItemStore::cartItemEntered(QListWidgetItem* item)
{
	if (!item) {
		cartItemNull();
		return;
	}
	cartHoverItem = static_cast<WebItemListWidgetItem*>(item);
	showBar(item, cartBar);
}

void WebItemStore::cartItemNull()
{
	cartHoverItem = nullptr;
	cartBar->hide();
}

void WebItemStore::remove()
{
	if (!cartHoverItem) return;
	delete cartHoverItem;
	cartItemNull();
}

void WebItemStore::cartPlay()
{
	WebItemEditor editor{this, attr->contentRect->size, cartHoverItem->item};
	editor.exec();
}

void WebItemStore::ok()
{
	attr->items.clear();

	for (int row = 0; row < ui->cart->count(); ++row) {
		WebItemListWidgetItem* item = static_cast<WebItemListWidgetItem*>(ui->cart->item(row));
		attr->items.push_back(item->item);
	}
	QDialog::accept();
}
