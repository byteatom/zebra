#include "DiscoverDlg.h"
#include "ui_DiscoverDlg.h"

#include <QNetworkInterface>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QStyleFactory>

#include "message.h"

enum {
	COL_NAME,
	COL_MODEL,
	COL_VERSION,
	COL_WIDTH,
	COL_HEIGHT,
	COL_ID,
	COL_INTF,
	COL_INFO,
	COL_MAX,
	COL_CARD = COL_NAME
};

const struct {
	const char *name;
	int width;
} colPara[COL_MAX] = {
	{QT_TRANSLATE_NOOP("DiscoverDlg", "Name"), 100},
	{QT_TRANSLATE_NOOP("DiscoverDlg", "Model"), 64},
	{QT_TRANSLATE_NOOP("DiscoverDlg", "Version"), 64},
	{QT_TRANSLATE_NOOP("DiscoverDlg", "Width"), 48},
	{QT_TRANSLATE_NOOP("DiscoverDlg", "Height"), 48},
	{QT_TRANSLATE_NOOP("DiscoverDlg", "ID"), 120},
	{QT_TRANSLATE_NOOP("DiscoverDlg", "Interface"), 60},
	{QT_TRANSLATE_NOOP("DiscoverDlg", "Info"), 120},
};

DiscoverDlg::DiscoverDlg(IMainWnd *mainWnd_, QWidget *parent) :
	QDialog(parent, Qt::MSWindowsFixedSizeDialogHint),
	mainWnd(mainWnd_),
	settings{"data/discover.ini", QSettings::IniFormat},
	ui(new Ui::DiscoverDlg)
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setWindowIcon(QIcon(QPixmap("image/icon/discover_128.png")));

	screen = dynamic_cast<IScreen*>(mainWnd->currentNode(INode::SCREEN));

	ui->cards->setStyle(QStyleFactory::create("Fusion"));
	ui->cards->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->cards->setSelectionMode(QAbstractItemView::SingleSelection);
	QStringList headers;
	ui->cards->setColumnCount(std::extent<decltype(colPara)>::value);
	for (int i = 0; i < std::extent<decltype(colPara)>::value; ++i) {
		ui->cards->setColumnWidth(i, colPara[i].width);
		headers << tr(colPara[i].name);
	}
	ui->cards->setHorizontalHeaderLabels(headers);
	ui->cards->horizontalHeader()->setSectionResizeMode(COL_NAME, QHeaderView::Stretch);

	connect(ui->cards, &QTableWidget::itemChanged, this, &DiscoverDlg::itemChanged);
	connect(ui->refresh, &QPushButton::clicked, [this](){
		start();
		timer.stop();
		timer.start();
	});
	connect(ui->ok, &QPushButton::clicked, this, &DiscoverDlg::accept);
	connect(ui->cancel, &QPushButton::clicked, this, &QDialog::reject);

	connect(&timer, &QTimer::timeout, [this](){
		start();
		const int interval = 10000;
		if (timer.interval() != interval)
			timer.setInterval(interval);
	});
	timer.start();
}

DiscoverDlg::~DiscoverDlg()
{
	stop();
	delete ui;
}

void DiscoverDlg::start()
{
	stop();

	ui->cards->setRowCount(0);

	struct NetAddr {
		QHostAddress ip;
		QHostAddress mask;
		QHostAddress broadcast;
	};
	std::list<NetAddr> netAddrs;
	for (QNetworkInterface intf : QNetworkInterface::allInterfaces())
	{
		for (QNetworkAddressEntry entry : intf.addressEntries())
		{
			if ((entry.ip() != QHostAddress::LocalHost)
			  && (entry.ip().protocol() == QAbstractSocket::IPv4Protocol)) {
				NetAddr addr;
				addr.ip = entry.ip();
				addr.mask = entry.netmask();
				addr.broadcast = entry.broadcast();
				netAddrs.push_back(addr);
			}
		}
	}

	for (NetAddr addr : netAddrs) {
		SocketUdp *udp = new SocketUdp();
		socketUdps.push_back(udp);

		udp->bind(addr.ip);
		udp->setNetmask(addr.mask);
		udp->setBroadcast(addr.broadcast);

		udp->setReceiver([this, udp](const char *data, int size, const QHostAddress &peer) {
			Msg retMsg(data, size);
			if (!retMsg.valid()) return;
			CardInfo* card = new CardInfo(QByteArray(retMsg.data(), retMsg.dataSize()));
			card->remote = peer;
			card->local = udp->localAddress();
			card->netmask = udp->getNetmask();
			card->broadcast = udp->getBroadcast();
			card->type = ICarrier::UDP;
			card->port = "5050";
			card->intf = tr("Net udp");
			card->info = QString("%1.%2.%3.%4")
							.arg((peer.toIPv4Address()>>24)&0xff)
							.arg((peer.toIPv4Address()>>16)&0xff)
							.arg((peer.toIPv4Address()>>8)&0xff)
							.arg((peer.toIPv4Address()>>0)&0xff);
			addCard(card);
		});

		InfoGetReqMsg msg;
		udp->writeData(msg.buf(), msg.size(), QHostAddress::Broadcast, 8888);
	}

	std::list<QSerialPortInfo> serialInfos = QSerialPortInfo::availablePorts().toStdList();
	for (auto info : serialInfos) {
		SerialPort *serial = new SerialPort();
		serialPorts.push_back(serial);

		serial->setPort(info.portName());
		serial->setBaud(57600);
		serial->setReadSize(1024);

		serial->setReceiver([this, serial](const char *data, int size) {
			Msg retMsg(data, size);
			if (retMsg.valid()) {
				CardInfo* card = new CardInfo(QByteArray(retMsg.data(), retMsg.dataSize()));
				card->type = ICarrier::SERIAL;
				card->port = serial->getPort();
				card->intf = tr("Serial");
				card->info = card->port;
				addCard(card);
			}
			serial->close();
		});

		if(!serial->open()) continue;

		InfoGetReqMsg msg;
		serial->writeData(msg.buf(), msg.size());
	}
}

void DiscoverDlg::stop()
{
	for (auto udp : socketUdps) delete udp;
	socketUdps.clear();

	for (auto serial : serialPorts) delete serial;
	serialPorts.clear();
}

void DiscoverDlg::addCard(CardInfo *card)
{
	card->name = settings.value(card->id).toString();

	QList<QTableWidgetItem*> items = ui->cards->findItems(card->id, Qt::MatchFixedString);
	int row = -1;
	if (!items.empty()) {
		row = items.back()->row();
	} else {
		row = ui->cards->rowCount();
		ui->cards->insertRow(row);
	}

	QString cols[COL_MAX] = {
		card->name,
		card->model,
		card->version,
		card->width,
		card->height,
		card->id,
		card->intf,
		card->info
	};

	for (int col = COL_MAX - 1; col >= 0 ; --col) {
		QTableWidgetItem *item = ui->cards->item(row, col);
		if (!item) {
			if (col == COL_CARD)
				item = new CardItem{cols[col], card};
			else
				item = new QTableWidgetItem{cols[col]};
			ui->cards->setItem(row, col, item);
		} else
			item->setText(cols[col]);
		item->setFlags(col == COL_NAME
						? item->flags() | Qt::ItemIsEditable
						: item->flags() & ~Qt::ItemIsEditable);
	}
	ui->cards->setCurrentCell(row, 0);
	ui->cards->setFocus();
}

CardInfo* DiscoverDlg::getCard(int row)
{
	CardItem* item = static_cast<CardItem*>(ui->cards->item(row, COL_CARD));
	if (!item) return nullptr;
	return item->card;
}

void DiscoverDlg::itemChanged(QTableWidgetItem *item)
{
	if (item->column() != COL_NAME) return;

	if (item->text() == "")
		settings.remove(ui->cards->item(item->row(), COL_ID)->text());
	else
		settings.setValue(ui->cards->item(item->row(), COL_ID)->text(), item->text());
}

void DiscoverDlg::accept()
{

	CardInfo* card = getCard(ui->cards->currentRow());

	if (!screen || !card) {
		QDialog::accept();
		return;
	}

	IScreen::ScreenPara &scrpara = screen->screenPara();
	scrpara.cardModel = card->model;

	QSize size(card->width.toInt(), card->height.toInt());
	if (scrpara.size != size
		&& QMessageBox::question(this, tr("Confirm"),
								tr("The screen and the selected device have different size. "
								   "Do you want to use the device's size?"))
					== QMessageBox::Yes)
	{
		scrpara.size = size;
	}

	IProtocol *protocol = screen->protocol();
	if (!protocol) {
		QDialog::accept();
		return;
	}

	IProtocol::Session session;
	session.type = card->type;
	session.peer.localAddr = card->local.toString();
	session.peer.remoteAddr = card->remote.toString();
	session.peer.port = card->port;
	if (session.type == ICarrier::UDP
		&& (card->remote.toIPv4Address() & card->netmask.toIPv4Address())
		!= (card->local.toIPv4Address() & card->netmask.toIPv4Address())) {
			QMessageBox::information(this, tr("Warning"),
							tr("This computer and seleted device exist in different network segment, "
							   "we'll use broadcast to communiate with the device."));
			session.peer.remoteAddr = "";
			session.peer.port = "";
	}
	protocol->bind(session);

	QDialog::accept();
}

