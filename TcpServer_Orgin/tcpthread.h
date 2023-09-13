#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QObject>
#include<QThread>
#include<atomic>
class TcpThread : public QThread
{
    Q_OBJECT
public:
    TcpThread();
    ~TcpThread();

    void run();

    void SubOne();
    void AddOne();
    //会话数
    std::atomic_uint32_t SessionCount = 0;
    //std::atomic<std::uint32_t> Sessioncount = 0;

signals:

};

#endif // TCPTHREAD_H
