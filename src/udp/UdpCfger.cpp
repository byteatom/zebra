#include "UdpCfger.h"
#include "ui_UdpCfger.h"

#include <QNetworkInterface>

UdpCfger::UdpCfger(Udp* udp_):
	ui(new Ui::UdpCfger),
	udp{udp_}
{
	ui->setupUi(this);

	lastRemoteAddr = udp->remoteAddr;

	QList<QNetworkInterface> intfs = QNetworkInterface::allInterfaces();
	for (QNetworkInterface& intf: intfs) {
		QNetworkInterface::InterfaceFlags flags = intf.flags();
		if (intf.hardwareAddress().isEmpty()
			|| intf.humanReadableName().contains("VMware", Qt::CaseInsensitive)
			|| !intf.isValid()
			|| !flags.testFlag(QNetworkInterface::IsUp)
			|| !flags.testFlag(QNetworkInterface::IsRunning)
			|| !flags.testFlag(QNetworkInterface::CanBroadcast)
			|| flags.testFlag(QNetworkInterface::IsLoopBack))
			continue;
		QList<QNetworkAddressEntry> entries = intf.addressEntries();
		for (QNetworkAddressEntry& entry: entries) {
			QHostAddress addr = entry.ip();
			if (addr.protocol() != QAbstractSocket::IPv4Protocol) continue;
			ui->localAddr->addItem(addr.toString());
			broadcasts.push_back(entry.broadcast().toString());
		}
	}
	broadcasts.push_back("255.255.255.255");

	connect(ui->remoteAddr, &QIpAddrEditor::addrChanged,
			[this](const QString& addr){
		udp->remoteAddr.setAddress(addr);
		int index = 0;
		int count = broadcasts.size();
		for (; index < count; ++index) {
			if (broadcasts[index] == addr) break;
		}
		bool broadcast = index < count;
		ui->localAddrTag->setEnabled(broadcast);
		ui->localAddr->setEnabled(broadcast);
		if (index < count - 1)
			ui->localAddr->setCurrentIndex(index);
		udp->localAddr.setAddress(broadcast ? ui->localAddr->currentText() : "");
	});

	connect(ui->remotePort, static_cast<void (QSpinBox::*)(int value)>(&QSpinBox::valueChanged),
			[this](int value) {
		udp->remotePort = value;
	});

	connect(ui->localAddr, &QComboBox::currentTextChanged,
			[this](const QString& text) {
		udp->localAddr.setAddress(text);
	});

	ui->localAddr->setCurrentText(udp->localAddr.toString());
	ui->remoteAddr->setAddress(udp->remoteAddr);
	ui->remotePort->setValue(udp->remotePort);
}

UdpCfger::~UdpCfger()
{
	delete ui;
}
