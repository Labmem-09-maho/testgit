#include "TcpSession.h"


TcpSession::TcpSession(TcpThread *thread)
{
    this->Thread_ = thread;
    connect(this, &TcpSession::readyRead,
            this, &TcpSession::SlotStartRead);
    connect(this, &TcpSession::disconnected,
            this, &TcpSession::SlotDisConnected);
    connect(this, &TcpSession::SignalDoDisConnect,
            this, &TcpSession::SlotDoDisconnect);
    connect(this, &TcpSession::SignalDoWrite,
            this, &TcpSession::SlotDoWrite);
    connect(this, &TcpSession::SignalDoConnectToServer,
            this, &TcpSession::SlotDoConnectToServer);
}

TcpSession::~TcpSession()
{
    disconnect(this, &TcpSession::readyRead,
               this, &TcpSession::SlotStartRead);
    disconnect(this, &TcpSession::disconnected,
               this, &TcpSession::SlotDisConnected);
    disconnect(this, &TcpSession::SignalDoDisConnect,
               this, &TcpSession::SlotDoDisconnect);
    disconnect(this, &TcpSession::SignalDoWrite,
               this, &TcpSession::SlotDoWrite);
    disconnect(this, &TcpSession::SignalDoConnectToServer,
               this, &TcpSession::SlotDoConnectToServer);
}

void TcpSession::ConnectToServer(const QString &host, quint16 port)
{
    emit this->SignalDoConnectToServer(host, port);
}

void TcpSession::Disconnect()
{
    emit this->SignalDoDisConnect();
}

void TcpSession::Write(const char *data, qint64 len)
{
    WriteBuffer_ = QByteArray(data, len);
    emit this->SignalDoWrite();
}

void TcpSession::SlotDoConnectToServer(const QString &host, quint16 port)
{
    this->connectToHost(QHostAddress(host), port);

}

void TcpSession::SlotStartRead()
{
    Buffer_ = this->readAll();
    if(OnRead)
        OnRead(Buffer_);
    emit this->SignalRead(Buffer_.toStdString().c_str(), Buffer_.length());
}

void TcpSession::SlotDisConnected()
{
    if(Thread_)
        Thread_->SubOne();

    //通知会话断开连接
    if(OnDisConnected)
        OnDisConnected(this);
    emit this->SignalDisConnected(this);
}

void TcpSession::SlotDoWrite()
{
    this->write(WriteBuffer_);
}

void TcpSession::SlotDoDisconnect()
{
    this->disconnectFromHost();
}


