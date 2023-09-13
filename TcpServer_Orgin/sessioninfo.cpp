#include "SessionInfo.h"

SessionInfo::SessionInfo(std::shared_ptr<TcpSession> &session)
{
    this->Session_ = session;
    m_myjson = new MyJson;
    m_timer = new QTimer(this);
    connect(this->Session_.get(),
            &TcpSession::disconnected,
            this,
            &SessionInfo::SlotDisconnected);
    connect(this->Session_.get(),
            &TcpSession::SignalRead,
            this,
            &SessionInfo::SlotRead);
    connect(this->Session_.get(),
            &TcpSession::connected,
            this,
            &SessionInfo::SignalConnected);

    static int i = 1;
    m_id = i;
    i++;
}

SessionInfo::~SessionInfo()
{
    if(!this->Session_)
        return;
    disconnect(this->Session_.get(),
               &TcpSession::disconnected,
               this,
               &SessionInfo::SlotDisconnected);
    disconnect(this->Session_.get(),
               &TcpSession::SignalRead,
               this,
               &SessionInfo::SlotRead);
    disconnect(this->Session_.get(),
               &TcpSession::connected,
               this,
               &SessionInfo::SignalConnected);
    this->Session_ = nullptr;
}

void SessionInfo::Connect(const QString &host, quint16 port)
{
    if(this->Session_)
        this->Session_->ConnectToServer(host, port);
}

void SessionInfo::Disconnect()
{
    if(this->Session_)
        this->Session_->Disconnect();
}

void SessionInfo::AnalysisDataConnect()
{
    connect(m_myjson,&MyJson::signal_ChatPackData,this,&SessionInfo::SlotChatPack);
    connect(m_myjson,&MyJson::signal_HeartPackData,this,&SessionInfo::SlotHeartPack);
}

void SessionInfo::Write(const char *buffer, int size)
{
    if(this->Session_)
        this->Session_->Write(buffer, size);
}

void SessionInfo::Write(const QByteArray &buffer)
{
    this->Session_->write(buffer);
}

int SessionInfo::getId()
{
    return m_id;
}

void SessionInfo::SlotRead(const QByteArray &data, int size)
{
    m_myjson->AnalysisPack(data);
    emit this->SignalRead(this, data, size);
}

void SessionInfo::SlotDisconnected()
{
    emit this->SignalDisconnect(m_id);
    if(this->OnDisConnected)
        this->OnDisConnected(this);
}

void SessionInfo::SlotCheckHeart()
{
    if(m_HeartTime - m_HeartTime >= 30000)
    {
        this->disconnect();
    }
}

void SessionInfo::SlotHeartPack(HeartPack pack)
{
    if(pack.number > m_number || pack.number == 0)
    {
        m_HeartTimePrev = m_HeartTime;
        m_HeartTime = std::stoi(pack.time);
    }
}

void SessionInfo::SlotChatPack(ChatPack pack)
{
    emit SignalChatPack(this,pack);
}
