#ifndef SESSIONTHREADS_H
#define SESSIONTHREADS_H
#include <vector>
#include <unordered_map>
#include <stdint.h>
#include <memory>
#include <mutex>
#include"tcpthread.h"
#include"tcpsession.h"


class SessionThreads : public QObject
{
    Q_OBJECT
public:
    SessionThreads();
    ~SessionThreads();

    //启动线程池, 非线程安全
    bool Start(uint32_t threadNum);
    //关闭, 非线程安全
    void Stop();
    //获取最小会话数线程
    TcpThread *PickMinThread();
    //获取会话数
    std::vector<uint32_t> GetSessionSize()const;
    //添加会话
    void AddSession(std::shared_ptr<TcpSession> &session);

private slots:
    //会话断开
    void SlotSessionDisConnected(void *id);

private:
    std::vector<TcpThread*> ThreadList_;
    //互斥量
    std::mutex Lock_;
    //会话列表
    std::unordered_map<void*, std::shared_ptr<TcpSession>> SessionList_;

    bool IsRunning_ = false;
};

#endif // SESSIONTHREADS_H
