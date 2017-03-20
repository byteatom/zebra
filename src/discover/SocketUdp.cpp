#include "SocketUdp.h"
#include <QObject>

SocketUdp::SocketUdp()
{
	socket = new QUdpSocket();

	ip.clear();
	netmask.clear();
	broadcast.clear();

	QObject::connect(socket, &QUdpSocket::readyRead, [this](){ read(); });
}

SocketUdp::~SocketUdp()
{
	socket->close();
	delete socket;
}

int64_t SocketUdp::writeData(const char *data, int64_t size, const QHostAddress &host, uint16_t port)
{
	return socket->writeDatagram(data, size, host, port);
}

int64_t SocketUdp::readData(char *data, int64_t size, QHostAddress *host, uint16_t *port)
{
	return socket->readDatagram(data, size, host, port);
}

bool SocketUdp::bind(const QHostAddress &address, uint16_t port)
{
	ip = address;
	return socket->bind(address, port);
}

void SocketUdp::setNetmask(const QHostAddress &address)
{
	netmask = address;
}

void SocketUdp::setBroadcast(const QHostAddress &address)
{
	broadcast = address;
}

const QHostAddress &SocketUdp::getNetmask() const
{
	return netmask;
}

const QHostAddress &SocketUdp::getBroadcast() const
{
	return broadcast;
}

QHostAddress SocketUdp::localAddress()
{
	return socket->localAddress();
}

void SocketUdp::setReceiver(DataReceiver recv)
{
	receiver = recv;
}

void SocketUdp::read()
{
	QByteArray datagram;
	QHostAddress peer;

	while (socket->hasPendingDatagrams()) {
		datagram.resize(socket->pendingDatagramSize());
		int size = socket->readDatagram(datagram.data(), datagram.size(), &peer);
		if (receiver) receiver(datagram.data(), size, peer);
	}
}
