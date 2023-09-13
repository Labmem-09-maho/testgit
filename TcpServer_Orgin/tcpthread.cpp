#include "tcpthread.h"


TcpThread::TcpThread()
{

}

TcpThread::~TcpThread()
{

}

void TcpThread::run()
{
    exec();
}
void TcpThread::SubOne()
{
    --SessionCount;
}

void TcpThread::AddOne()
{
    ++SessionCount;
}
