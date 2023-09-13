#include"sessioninfolist.h"

SessionInfoList::SessionInfoList()
{

}

SessionInfoList::~SessionInfoList()
{
    this->Clear();
}

SessionInfo *SessionInfoList::NewSessionInfo(std::shared_ptr<TcpSession> &session)
{
    std::lock_guard<std::mutex> lock(this->Lock_);
    SessionInfo *info = new SessionInfo(session);
    //
    static int i = 1;
    info->m_id = i;
    i++;
    //
    this->List_.push_back(info);
    info->OnDisConnected = std::bind(&SessionInfoList::SessionDisconnected,
                                     this,
                                     std::placeholders::_1);
    return info;
}

void SessionInfoList::Clear()
{
    std::lock_guard<std::mutex> lock(Lock_);
    for(SessionInfo* info : this->List_)
    {
        if(info)
            delete info;
    }
    this->List_.clear();
}

SessionInfo *SessionInfoList::FindByID(const void *id)
{
    if (!id)
        return nullptr;
    std::lock_guard<std::mutex> lock(this->Lock_);
    for (size_t i = 0; i < this->List_.size(); ++i)
    {
        SessionInfo *info = this->List_[i];
        if (info == id)
            return info;
    }
    return nullptr;
}

SessionInfo *SessionInfoList::FindByID(int id)
{
    if (!id)
        return nullptr;
    std::lock_guard<std::mutex> lock(this->Lock_);
    for (size_t i = 0; i < this->List_.size(); ++i)
    {
        SessionInfo *info = this->List_[i];
        if (info->m_id == id)
            return info;
    }
    return nullptr;

}

void SessionInfoList::SessionDisconnected(void *id)
{
    SessionInfo *info = nullptr;
    std::lock_guard<std::mutex> lock(this->Lock_);
    //将session从列表中删除
    std::vector<SessionInfo*>::iterator it;
    for (it = this->List_.begin(); it != this->List_.end(); ++it)
    {
        info = *it;
        if (info == id)
        {
            it = this->List_.erase(it);
            info->OnDisConnected = nullptr;
            delete info;
            return;
        }
    }
}
