#ifndef SESSIONINFOLIST_H
#define SESSIONINFOLIST_H
#include <mutex>
#include <memory>
#include <vector>
#include"sessioninfo.h"

class SessionInfoList
{
public:
    SessionInfoList();
    ~SessionInfoList();

    SessionInfo* NewSessionInfo(std::shared_ptr<TcpSession> &session);
    void Clear();
    SessionInfo* FindByID(const void *id);
    SessionInfo* FindByID(int id);
private:
    void SessionDisconnected(void *id);
private:
    std::mutex Lock_;
    std::vector<SessionInfo*> List_;
};

#endif // SESSIONINFOLIST_H
