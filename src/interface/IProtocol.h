#ifndef IPROTOCL_H
#define IPROTOCL_H

#include <list>

#include <QWidget>
#include <QByteArray>

#include "IProjJson.h"
#include "QStr.h"
#include "ICarrier.h"
#include "IMsg.h"

typedef std::function<void (QByteArray data)> DataRecver;
typedef std::function<void (bool ok)> BoolRecver;
typedef std::function<void (qint64 chunk)> ProgressRecver;

class IProtocol : public IProjJson
{
public:
	struct Session {
		ICarrier::Type type;
		ICarrier::Peer peer;
	};
	typedef std::list<ICarrier::TypeName> TypeNames;

	virtual ICarrier::TypeName typeName() = 0;
	virtual QString portName() = 0;
	virtual TypeNames typeNames() = 0;
	virtual QString description() = 0;
	virtual QWidget* newCfger() = 0;
	virtual void bind(const Session& session) = 0;
	virtual void open() = 0;
	virtual void send(IMsgSp req, DataRecver recver,
					  int timeout = 3000, int retry = 1) = 0;
	virtual void send(IMsgSp req, BoolRecver recver,
					  int timeout = 3000, int retry = 1) = 0;
	virtual void sendFile(const QString& path, BoolRecver boolRecver, ProgressRecver progressRecver = nullptr) = 0;
	virtual void close() = 0;
};

#endif // IPROTOCL_H
