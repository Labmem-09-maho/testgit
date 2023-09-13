#include "SessionThreads.h"

SessionThreads::SessionThreads()
{

}

SessionThreads::~SessionThreads()
{
    this->Stop();
}


bool SessionThreads::Start(uint32_t threadNum)
{
    if(IsRunning_)
        return true;
    for(uint32_t i = 0; i < threadNum; ++i)
    {
        TcpThread *thread = new TcpThread();
        ThreadList_.push_back(thread);
        thread->start();
    }
    IsRunning_ = true;
    return true;
}

void SessionThreads::Stop()
{
    if(!IsRunning_)
        return;
    //先关已有连接
    {
        std::lock_guard<std::mutex> locker(this->Lock_);
        std::unordered_map<void*, std::shared_ptr<TcpSession>>::iterator itor = SessionList_.begin();
        //关闭连接
        for(itor = SessionList_.begin(); itor != SessionList_.end(); ++itor)
        {
            std::shared_ptr<TcpSession> session = itor->second;
            if(session.get())
            {
                disconnect(session.get(),
                           &TcpSession::SignalDisConnected,
                           this,
                           &SessionThreads::SlotSessionDisConnected);
                session.get()->Disconnect();
            }
        }
    }
    for(TcpThread *thread : this->ThreadList_)
    {
        thread->exit();
        thread->wait();
    }
    for(TcpThread *thread : this->ThreadList_)
        delete thread;
    this->ThreadList_.clear();
    this->SessionList_.clear();
    IsRunning_ = false;
}

TcpThread *SessionThreads::PickMinThread()
{
    TcpThread *thread = nullptr;
    uint32_t mincount = 0;
    bool first = true;
    for(TcpThread *tmpthread : this->ThreadList_)
    {
        uint32_t tmpcount = tmpthread->SessionCount;
        if(first || tmpcount < mincount)
        {
            mincount = tmpcount;
            thread = tmpthread;
        }
        first = false;
    }
    return thread;
}

std::vector<uint32_t> SessionThreads::GetSessionSize() const
{
    std::vector<uint32_t> vec;
    for(TcpThread *thread : this->ThreadList_)
        vec.push_back(thread->SessionCount);
    return vec;
}

void SessionThreads::AddSession(std::shared_ptr<TcpSession> &session)
{
    connect(session.get(),
            &TcpSession::SignalDisConnected,
            this,
            &SessionThreads::SlotSessionDisConnected,
            Qt::QueuedConnection);
    //加锁
    std::lock_guard<std::mutex> locker(this->Lock_);
    this->SessionList_[session.get()] = session;
}

//会话断开
void SessionThreads::SlotSessionDisConnected(void *id)
{
    //加锁
    std::lock_guard<std::mutex> locker(this->Lock_);
    std::unordered_map<void*, std::shared_ptr<TcpSession>>::iterator itor = SessionList_.begin();
    itor = SessionList_.find(id);
    if(itor != SessionList_.end())
    {
        SessionList_.erase(itor);
    }
}

