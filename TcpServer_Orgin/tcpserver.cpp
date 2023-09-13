#include "TcpServer.h"


TcpServer::~TcpServer()
{
    this->Stop();
}

bool TcpServer::Start(ServerData &conf)
{
    if(IsRunning_)
        return true;
    //验证数据
    conf.Verify();
    //启动线程池
    SessionThreads_.Start(conf.ThreadNum);
    //监听端口
    if(!this->listen(QHostAddress::Any, (quint16)conf.Port))
        return false;
    IsRunning_ = true;
    return true;
}

void TcpServer::Stop()
{
    if(!IsRunning_)
        return;
    //关闭监听
    this->close();
    //关闭线程池
    SessionThreads_.Stop();
    IsRunning_ = false;
}

bool TcpServer::IsStart()
{
    return IsRunning_;
}

std::vector<uint32_t> TcpServer::GetSessionSize() const
{
    return this->SessionThreads_.GetSessionSize();
}

void TcpServer::incomingConnection(qintptr handle)
{
    std::shared_ptr<TcpSession> session = this->CreateSession(handle);
    if(this->OnAccepted)
        this->OnAccepted(session);
}

std::shared_ptr<TcpSession> TcpServer::CreateSession(qintptr handle)
{
    TcpThread *thread = this->SessionThreads_.PickMinThread();
    std::shared_ptr<TcpSession> session = std::make_shared<TcpSession>(thread);
    session->setSocketDescriptor(handle);
    this->SessionThreads_.AddSession(session);
    session->moveToThread(thread);
    if(thread)
        thread->AddOne();
    return session;
}

