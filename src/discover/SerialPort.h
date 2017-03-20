#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <string>
#include <list>
#include <functional>

#include <QTimer>
#include <QtSerialPort/QSerialPort>

class SerialPort
{
public:
	enum ReceiveState {
		RECEIVE_WAIT = 0,
		RECEIVE_HEAD0,
		RECEIVE_HEAD1,
		RECEIVE_HEAD2,
		RECEIVE_DATA,
		RECEIVE_VERIFY0,
		RECEIVE_VERIFY1,
		RECEIVE_SUCCESS
	};

	enum {
		PROTOCOL_TIMEOUT = 4000
	};

	using DataReceiver = std::function<void (const char *data, int size)>;

	explicit SerialPort();
	~SerialPort();

	void setPort(const QString &port_);
	QString getPort() const;
	void setBaud(int baud_);
	int getBaud() const;
	void setReadSize(int readSize_);
	int getReadSize() const;
	bool open();
	bool isOpen() const;
	void close();
	int64_t readData(char *data, int64_t size, int timeout = 0);
	int64_t writeData(char *data, int64_t size, int timeout = 0);
	void setReceiver(DataReceiver recv);

private:
	ReceiveState peek(char c);
	void read();
	QTimer timer;
	ReceiveState state;
	QByteArray buf;
	int pos;

	int baud;
	int readSize;
	QString port;
	QSerialPort *serial = nullptr;
	DataReceiver receiver;
};

#endif // SERIALPORT_H
