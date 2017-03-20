#ifndef SOCKETCFGER_H
#define SOCKETCFGER_H

#include <QFrame>
#include <QHostAddress>
#include <vector>

#include "Udp.h"

namespace Ui {
class UdpCfger;
}

class UdpCfger : public QFrame
{
	Q_OBJECT

public:
	explicit UdpCfger(Udp* udp);
	~UdpCfger();

private:
	Ui::UdpCfger *ui;
	Udp* udp;
	QHostAddress lastRemoteAddr;
	std::vector<QString> broadcasts;
};

#endif // SOCKETCFGER_H
