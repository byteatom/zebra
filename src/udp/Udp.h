#ifndef UDP_H
#define UDP_H

#include <QUdpSocket>

#include "ICarrier.h"

class Udp : public QObject, public ICarrier
{
	Q_OBJECT
public:
	Udp(Json* jnode);
	static TypeName type_name();
private:
	virtual ~Udp();
	virtual TypeName typeName() override;
    virtual QString portName() override;
	virtual QString description() override;
	virtual void bind(const Peer& peer) override;
	virtual QWidget* newCfger() override;
	virtual void open() override;
	virtual void recv(MsgRecver recver) override;
	virtual void send(IMsgSp msg) override;
	virtual void close() override;
	virtual void exportProjJson(Json& jnode) override;

	void read();
	QUdpSocket socket;
	QHostAddress localAddr;
	QHostAddress remoteAddr{QHostAddress::Broadcast};
	quint16 remotePort{DEFAULT_SOCKET_PORT};
	MsgRecver recver;

	friend class UdpCfger;
};

#endif // UDP_H
