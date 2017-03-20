#include "Udp.h"

#include "Message.h"

#include "UdpCfger.h"

Udp::Udp(Json* jnode)
{
	connect(&socket, &QUdpSocket::readyRead,
			this, &Udp::read);

	if (jnode) {
		Json::iterator jit;
		jit = jnode->find("localAddr");
		if (jit != jnode->end())	localAddr.setAddress(QString::fromStdString(*jit));
		jit = jnode->find("remoteAddr");
		if (jit != jnode->end())	remoteAddr.setAddress(QString::fromStdString(*jit));
		jtov2(remotePort, "remotePort");
	}
}

Udp::~Udp()
{
	close();
}

ICarrier::TypeName Udp::type_name()
{
	return ICarrier::TypeName{ICarrier::UDP, tr("UDP")};
}

ICarrier::TypeName Udp::typeName()
{
	return type_name();
}

QString Udp::portName()
{
	return QString::number(remotePort);
}

QString Udp::description()
{
	if (localAddr.isNull())
		return remoteAddr.toString() + ":" + QString::number(remotePort);
	else
		return localAddr.toString() + "->" +
				remoteAddr.toString() + ":" + QString::number(remotePort);
}

void Udp::bind(const Peer& peer)
{
	localAddr.setAddress(peer.localAddr);

	if (!peer.remoteAddr.isEmpty()) {
		remoteAddr.setAddress(peer.remoteAddr);
	} else {
		remoteAddr.setAddress("255.255.255.255");
	}

	if (peer.port.isEmpty())
		remotePort = DEFAULT_SOCKET_PORT;
	else {
		bool isUshort;
		peer.port.toUShort(&isUshort);
		if (isUshort)
			remotePort = peer.port.toUShort();
	}
}

QWidget* Udp::newCfger()
{
	return new UdpCfger(this);
}

void Udp::open()
{
	if (!localAddr.isNull()) {
		socket.bind(localAddr);
	}
}

void Udp::recv(MsgRecver recver)
{
	this->recver = recver;
}

void Udp::send(IMsgSp msg)
{
	socket.writeDatagram(msg->buf(), msg->size(), remoteAddr, remotePort);
}

void Udp::read()
{
	while (socket.hasPendingDatagrams()) {
		IMsgSp rsp{new Msg{(int)socket.pendingDatagramSize()}};
		socket.readDatagram(rsp->buf(), rsp->size());
		if (recver) recver(rsp);
	}
}

void Udp::close()
{
	recver = nullptr;
	socket.close();
}

void Udp::exportProjJson(Json& jnode)
{
	jnode["localAddr"] = localAddr.toString();
	jnode["remoteAddr"] = remoteAddr.toString();
	jnode["remotePort"] = remotePort;
}
