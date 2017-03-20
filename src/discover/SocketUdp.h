#ifndef SOCKETUDP_H
#define SOCKETUDP_H

#include <list>
#include <functional>
#include <QUdpSocket>

class SocketUdp
{
public:
    using DataReceiver = std::function<void (const char *data, int size, const QHostAddress &host)>;

    explicit SocketUdp();
    ~SocketUdp();

    int64_t writeData(const char *data, int64_t size, const QHostAddress &host, uint16_t port);
    int64_t readData(char *data, int64_t size, QHostAddress *host = nullptr, uint16_t *port = nullptr);
    bool bind(const QHostAddress &address, uint16_t port = 0);
    QHostAddress localAddress();
    void setReceiver(DataReceiver recv);

    void setNetmask(const QHostAddress &address);
    void setBroadcast(const QHostAddress &address);
    const QHostAddress &getNetmask() const;
    const QHostAddress &getBroadcast() const;

private:
    void read();

    QUdpSocket *socket = nullptr;
    DataReceiver receiver;

    QHostAddress ip;
    QHostAddress netmask;
    QHostAddress broadcast;

};

#endif // SOCKETUDP_H
