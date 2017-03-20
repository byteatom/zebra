#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <list>
#include <memory>

#include <QTimer>

#include "IProtocol.h"
#include "ICarrierFactory.h"
#include "Json.h"
#include "Message.h"

typedef std::array<ICarrierFactory*, ICarrier::MAX_TYPE> CarrierFactories;

class Protocol : public IProtocol
{
public:
	explicit Protocol(const CarrierFactories& carrierFactories, Json* jnode);
private:
	virtual ~Protocol();

	virtual ICarrier::TypeName typeName() override;
    virtual QString portName() override;
	virtual TypeNames typeNames() override;
	virtual QString description() override;
	virtual QWidget* newCfger() override;
	virtual void bind(const Session& session) override;
	virtual void open() override;
	virtual void send(IMsgSp req, DataRecver recver, int timeout, int retry) override;
	virtual void send(IMsgSp req, BoolRecver recver, int timeout, int retry) override;
	virtual void sendFile(const QString& path, BoolRecver boolRecver, ProgressRecver fileProgressRecver) override;
	virtual void close() override;

	virtual void exportProjJson(Json& jnode) override;

	void runTask();
	void endTask(IMsgSp rsp);
	void recv(IMsgSp rsp);
	void timeout();

	const CarrierFactories& carrierFactories;
	ICarrier* carrier = nullptr;
	struct Task {
		IMsgSp req;
		DataRecver dataRecver;
		BoolRecver boolRecver;
		int timeout;
		int retry;
		int tried;
		bool ready;
	};
	typedef std::shared_ptr<Task> TaskSp;
	std::list<TaskSp> tasks;
	QTimer timer;

	void runSendFile();
	void recvFile(bool ok, IMsgSp msg);
	std::unique_ptr<FileMsgFactory> fileMsgfactory;
	BoolRecver fileBoolRecver;
	ProgressRecver fileProgressRecver;
};

#endif // PROTOCOL_H
