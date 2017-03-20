#include "SerialPort.h"
#include <QObject>

SerialPort::SerialPort()
{
	serial = new QSerialPort();
	setBaud(57600);
	setReadSize(1024);
	state = RECEIVE_WAIT;
	pos = 0;

	QObject::connect(serial, &QSerialPort::readyRead, [this](){ read(); });

	QObject::connect(&timer, &QTimer::timeout, [this](){
		timer.stop();
		state = RECEIVE_WAIT;
		pos = 0;
	});
}

SerialPort::~SerialPort()
{
	serial->close();
	delete serial;
}

static QSerialPort::BaudRate getQtBaud(int baud)
{
	switch (baud)
	{
	case 1200:
		return QSerialPort::Baud1200;
	case 2400:
		return QSerialPort::Baud2400;
	case 4800:
		return QSerialPort::Baud4800;
	case 9600:
		return QSerialPort::Baud9600;
	case 19200:
		return QSerialPort::Baud19200;
	case 38400:
		return QSerialPort::Baud38400;
	case 57600:
		return QSerialPort::Baud57600;
	case 115200:
		return QSerialPort::Baud115200;
	default:
		return QSerialPort::Baud57600;
	}

	return QSerialPort::Baud57600;
}

bool SerialPort::isOpen() const
{
	return serial->isOpen();
}

void SerialPort::setPort(const QString &port_)
{
	port = port_;
}

QString SerialPort::getPort() const
{
	return port;
}

void SerialPort::setBaud(int baud_)
{
	baud = baud_;
}

int SerialPort::getBaud() const
{
	return baud;
}

void SerialPort::setReadSize(int readSize_)
{
	readSize = readSize_;
	buf.resize(readSize_);
}

int SerialPort::getReadSize() const
{
	return readSize;
}

bool SerialPort::open()
{
	if (serial->isOpen())  return true;
	serial->setPortName(port);
	serial->setBaudRate(getQtBaud(baud));
	serial->setParity(QSerialPort::NoParity);
	serial->setDataBits(QSerialPort::Data8);
	serial->setStopBits(QSerialPort::OneStop);
	serial->setFlowControl(QSerialPort::NoFlowControl);
	serial->setReadBufferSize(readSize);
	return serial->open(QSerialPort::ReadWrite);
}

void SerialPort::close()
{
	if (serial->isOpen()) {
		serial->clear();
		serial->close();
	}
}

int64_t SerialPort::readData(char *data, int64_t size, int timeout)
{
	int64_t len = 0;

	for (;;)
	{
		int64_t n = serial->read(data + len, size - len);
		if (n == -1)
			return -1;
		else if (n == 0 && !serial->waitForReadyRead(timeout)) {
			if (len > 0)
				break;
			else
				return -2;
		} else {
			len += n;
			if (size == len)
				break;
		}
	}

	return len;
}

int64_t SerialPort::writeData(char *data, int64_t size, int timeout)
{
	int64_t len = 0;

	for (;;)
	{
		int64_t n = serial->write(data + len, size - len);
		if (n == -1)
			return -1;
		else {
			len += n;
			if (size == len)
				break;
		}
	}

	if (timeout != 0)
		serial->waitForBytesWritten(timeout);

	return len;
}

void SerialPort::setReceiver(DataReceiver recv)
{
	receiver = recv;
}

const char *head = "KLB";
const char *tail = "KLE";
SerialPort::ReceiveState SerialPort::peek(char c)
{
	switch(state)
	{
	case RECEIVE_WAIT:
		if (c == head[0]) {
			pos = 0;
			buf[pos] = c;
			state = RECEIVE_HEAD1;
		}
		break;
	case RECEIVE_HEAD1:
		if (c == head[1]) {
			buf[++pos] = c;
			state = RECEIVE_HEAD2;
		} else {
			state = RECEIVE_WAIT;
		}
		break;
	case RECEIVE_HEAD2:
		if (c == head[2]) {
			buf[++pos] = c;
			state = RECEIVE_DATA;
		} else {
			state = RECEIVE_WAIT;
		}
		break;
	case RECEIVE_DATA:
		if (pos + 1 <= buf.size()) {
			buf[++pos] = c;
			if (memcmp(buf.data() + pos - 2, tail, strlen(tail)) == 0) {
				state = RECEIVE_VERIFY0;
			} else {
				state = RECEIVE_DATA;
			}
		} else {
			state = RECEIVE_WAIT;
		}
		break;
	case RECEIVE_VERIFY0:
		buf[++pos] = c;
		state = RECEIVE_VERIFY1;
		break;
	case RECEIVE_VERIFY1:
		buf[++pos] = c;
		state = RECEIVE_SUCCESS;
		break;
	default:
		state = RECEIVE_WAIT;
		break;
	}

	return state;
}

void SerialPort::read()
{
	timer.start(PROTOCOL_TIMEOUT);

	QByteArray chunk = serial->readAll();
	for (auto c : chunk) {
		if (peek(c) == RECEIVE_SUCCESS) {
			if (receiver) receiver(buf.data(), pos + 1);
			state = RECEIVE_WAIT;
			pos = 0;
			break;
		}
	}
}
