#ifndef TCPSESSION_H
#define TCPSESSION_H
#include <QTcpSocket>
#include <functional>
#include <QHostAddress>
#include"tcpthread.h"

class TcpSession : public QTcpSocket
{
    Q_OBJECT
signals:
    void SignalRead(const QByteArray &, int);
    void SignalDisConnected(void *);
    void SignalDoWrite();
    void SignalDoDisConnect();
    void SignalDoConnectToServer(const QString &, quint16);

public:
    TcpSession(TcpThread *thread);
    ~TcpSession();

    void ConnectToServer(const QString &host, quint16 port);
    //断开连接
    void Disconnect();
    void Write(const char *data, qint64 len);

public:
    //断开连接回调
    std::function<void(void*)> OnDisConnected = nullptr;
    //读数据回调
    std::function<void(const QByteArray&)> OnRead = nullptr;

private slots:
    //连接Server
    void SlotDoConnectToServer(const QString &host, quint16 port);
    //开始读数据
    void SlotStartRead();
    //断开连接回调
    void SlotDisConnected();
    //写数据
    void SlotDoWrite();
    //断开连接
    void SlotDoDisconnect();

private:
    TcpThread *Thread_ = nullptr;
    QByteArray Buffer_ = nullptr;
    QByteArray WriteBuffer_ = nullptr;
};

#endif // TCPSESSION_H
