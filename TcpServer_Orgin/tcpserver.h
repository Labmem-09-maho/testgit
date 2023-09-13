#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QTcpServer>
#include <vector>
#include <thread>
#include <stdint.h>
#include <memory>
#include"sessionthreads.h"

    //Server数据格式
    struct ServerData
{
    uint16_t Port;
    uint32_t ThreadNum;
    void Verify()
    {
        if(Port == 0)
            Port = 12345;
        if(ThreadNum)
            ThreadNum = std::thread::hardware_concurrency();
        if(ThreadNum == 0)
            ThreadNum = 1;
    }
};

//Server端
class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    TcpServer() = default;
    ~TcpServer();

    //启动 非线程安全
    bool Start(ServerData &conf);
    //关闭  非线程安全
    void Stop();

    bool IsStart();
    //获取会话数
    std::vector<uint32_t> GetSessionSize() const;

public:
    //新连接回调
    std::function<void(std::shared_ptr<TcpSession> &)> OnAccepted = nullptr;

protected:
    virtual void incomingConnection(qintptr handle);
    std::shared_ptr<TcpSession> CreateSession(qintptr handle);

private:
    bool IsRunning_ = false;
    SessionThreads SessionThreads_;
};

#endif // TCPSERVER_H
