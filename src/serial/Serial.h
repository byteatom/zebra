#ifndef SERIAL_H
#define SERIAL_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>

#include "ICarrier.h"
#include "Message.h"

class Serial : public QObject, public ICarrier
{
	Q_OBJECT
public:
	Serial(Json* jnode);
	static TypeName type_name();
private:
	virtual ~Serial();
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

	inline int recved();
	void read();
	QSerialPort serial;
	QStr port;
	bool opened = false;
	QByteArray buf;
	qint64 pad;
	MsgRecver recver;

	friend class SerialCfger;
};

#endif // SERIAL_H
