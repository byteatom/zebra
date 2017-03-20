#include "Serial.h"

#include "SerialCfger.h"

Serial::Serial(Json* jnode)
{
	buf.reserve(Msg::MSG_MAX_SIZE * 16);

	connect(&serial, &QSerialPort::readyRead, this, &Serial::read);

	if (jnode) {
		jtov2(port, "serialPort");
	}

	if (port.isEmpty()) {
		QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
		if (!infos.isEmpty())
			port = infos.front().portName();
	}
	serial.setBaudRate(QSerialPort::Baud57600);
	//serial.setDataBits(QSerialPort::Data8);
	//serial.setFlowControl(QSerialPort::NoFlowControl);
	//serial.setParity(QSerialPort::NoParity);
	//serial.setStopBits(QSerialPort::OneStop);
}

Serial::~Serial()
{
	close();
}

ICarrier::TypeName Serial::type_name()
{
	return ICarrier::TypeName{ICarrier::SERIAL, Serial::tr("Serial Port")};
}


ICarrier::TypeName Serial::typeName()
{
	return type_name();
}

QString Serial::portName()
{
	return port;
}
QString Serial::description()
{
	return port;
}

void Serial::bind(const Peer& peer)
{
	if (peer.port.isEmpty()) {
		QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
		if (!infos.isEmpty())
			port = infos.front().portName();
	} else
		port = peer.port;
}

QWidget* Serial::newCfger()
{
	return new SerialCfger(this);
}

void Serial::open()
{
	buf.resize(0);
	pad = 0;
	serial.setPortName(port);
	opened = serial.open(QIODevice::ReadWrite);
	if (opened) {
		//serial.setBreakEnabled(true);
		//serial.setDataTerminalReady(true);
		//serial.setRequestToSend(true);
	}
}

void Serial::recv(MsgRecver recver)
{
	this->recver = recver;
}

void Serial::send(IMsgSp msg)
{
	if (!opened) return;
	serial.write(msg->buf(), msg->size());
}

void Serial::read()
{
	qint64 size = buf.size();
	qint64 read = serial.bytesAvailable();
	if (size + read > buf.capacity()) {
		buf.remove(0, pad);
		pad = 0;
		size = buf.size();
	}
	buf.resize(size + read);
	serial.read(buf.data() + size, read);
	size = buf.size();

	if (size - pad < Msg::MSG_MIN_SIZE) return;

	//get message
	const char* data = buf.constData();
	for (int pos = pad; pos <= size - sizeof(Msg::PROLOGUE); ++pos) {
		if (memcmp(data + pos, Msg::PROLOGUE, sizeof(Msg::PROLOGUE)) != 0) {
			++pad;
			continue;
		}
		if (size - pos < Msg::MSG_MIN_SIZE) return;
		Msg msg{data + pos};
		int msgSize = msg.size();
		if (size - pos < msgSize) return;
		if (msg.valid() && recver) {
			IMsgSp msgsp{new Msg{msg.buf(), msgSize}};
			recver(msgsp);
		}
		pos += (msgSize - 1);
		pad += msgSize;
	}
}

void Serial::close()
{
	recver = nullptr;
	opened = false;
	serial.close();
}

void Serial::exportProjJson(Json& jnode)
{
	vtoj2(port, "serialPort");
}
