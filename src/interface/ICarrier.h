#ifndef ICARRIER_H
#define ICARRIER_H

#include <functional>

#include <QWidget>

#include "IProjJson.h"
#include "IMsg.h"

#define DEFAULT_SOCKET_PORT 5050

typedef std::function<void (IMsgSp rsp)> MsgRecver;

class ICarrier : public IProjJson
{
public:
	enum Type {
		UDP,
		TCP,
		SERIAL,
		MAX_TYPE
	};
	struct TypeName {
		Type type;
		QString name;
	};

	struct Peer {
		QString localAddr;	/**<UDP, empty means reset to default, ill-formed means no change*/
		QString remoteAddr;	/**<UDP, TCP, empty means reset to default, ill-formed means no change*/
		QString port;	/**<UDP or TCP remote port, SERIAL port, empty means reset to default, ill-formed means no change*/
	};

	virtual TypeName typeName() = 0;
	virtual QString portName() = 0;
	virtual QString description() = 0;
	virtual QWidget* newCfger() = 0;/*caller's duty to free*/
	virtual void bind(const Peer& peer) = 0;
	virtual void open() = 0;
	virtual void recv(MsgRecver recver) = 0;
	virtual void send(IMsgSp msg) = 0;
	virtual void close() = 0;

	virtual ~ICarrier() = default;
};

#endif // ICARRIER_H
