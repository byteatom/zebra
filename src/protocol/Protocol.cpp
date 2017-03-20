#include "Protocol.h"

Protocol::Protocol(const CarrierFactories& carrierFactories, Json* jnode):
	carrierFactories{carrierFactories}
{
	int type{ICarrier::Type::UDP};
	if (jnode) {
		type = jnode->value("commType", (int)ICarrier::Type::UDP);
	}
	carrier = carrierFactories.at(type)->create(jnode);
	timer.connect(&timer, &QTimer::timeout, [this](){timeout();});
}

Protocol::~Protocol()
{
	close();
	delete carrier;
}

ICarrier::TypeName Protocol::typeName()
{
	return carrier->typeName();
}

QString Protocol::portName()
{
	return carrier->portName();
}
IProtocol::TypeNames Protocol::typeNames()
{
	TypeNames typeNames;
	for (ICarrierFactory* factory: carrierFactories) {
		if (factory) typeNames.push_back(factory->typeName());
	}
	return typeNames;
}

QString Protocol::description()
{
	return typeName().name + " " + carrier->description();
}

QWidget* Protocol::newCfger()
{
	return carrier->newCfger();
}

void Protocol::bind(const Session& session)
{
	close();
	delete carrier;
	carrier = carrierFactories.at(session.type)->create(nullptr);
	carrier->bind(session.peer);
}

void Protocol::open()
{
	carrier->open();
	carrier->recv([this](IMsgSp rsp){recv(rsp);});
}

void Protocol::send(IMsgSp req, DataRecver recver, int timeout, int retry)
{
	tasks.push_back(TaskSp(new Task{req, recver, nullptr, timeout, retry, 0, true}));
	runTask();
}

void Protocol::send(IMsgSp req, BoolRecver recver, int timeout, int retry)
{
	tasks.push_back(TaskSp(new Task{req, nullptr, recver, timeout, retry, 0, true}));
	runTask();
}

void Protocol::recv(IMsgSp rsp)
{
	if (tasks.empty()) return;

	TaskSp task = tasks.front();
	if (rsp->isRspFor(task->req)) {
		endTask(rsp);
		runTask();
	}
}

void Protocol::timeout()
{
	if (tasks.empty()) return;

	TaskSp task = tasks.front();
	if (task->tried >= task->retry + 1)
		endTask(nullptr);
	else
		task->ready = true;
	runTask();
}

void Protocol::runTask()
{
	if (tasks.empty()) return;

	TaskSp task = tasks.front();
	if (!task->ready) return;

	task->ready = false;
	task->tried = task->tried + 1;
	carrier->send(task->req);

	if (!timer.isActive()
		&& task.get() == tasks.front().get()
		&& task->timeout >= 0) {
		timer.setInterval(task->timeout);
		timer.start();
	}
}

void Protocol::endTask(IMsgSp rsp)
{
	timer.stop();
	if (tasks.empty()) return;
	TaskSp task = tasks.front();
	tasks.pop_front();
	if (task->boolRecver)
		task->boolRecver(rsp ? true : false);
	if (task->dataRecver) {
		QByteArray data{rsp ? rsp->data() : 0, rsp ? rsp->dataSize() : 0};
		task->dataRecver(data);
	}
}

void Protocol::sendFile(const QString& path, BoolRecver boolRecver, ProgressRecver progressRecver)
{
	fileMsgfactory = std::make_unique<FileMsgFactory>(path);
	fileBoolRecver = boolRecver;
	fileProgressRecver = progressRecver;
	runSendFile();
}

void Protocol::runSendFile()
{
	IMsgSp msg = fileMsgfactory->next();
	if (msg) {
		send(msg, static_cast<BoolRecver>(std::bind(&Protocol::recvFile, this, std::placeholders::_1, msg)), 3000, 1);
	} else {
		fileMsgfactory.reset();
		if (fileBoolRecver)
			fileBoolRecver(true);
	}
}

void Protocol::recvFile(bool ok, IMsgSp msg)
{
	if (ok) {
		if (fileProgressRecver)
			fileProgressRecver(static_cast<FileSetReqMsg*>(msg.get())->fileDataSize);
		runSendFile();
	} else {
		fileMsgfactory.reset();
		if (fileBoolRecver)
			fileBoolRecver(false);
	}
}

void Protocol::close()
{
	timer.stop();
	tasks.clear();
	fileMsgfactory.reset();
	fileBoolRecver = nullptr;
	fileProgressRecver = nullptr;
	carrier->close();
}

void Protocol::exportProjJson(Json& jnode)
{
	jnode["commType"] = typeName().type;
	jnode["commName"] = typeName().name;
	carrier->exportProjJson(jnode);
}
