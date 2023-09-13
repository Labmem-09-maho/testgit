#ifndef SESSIONINFO_H
#define SESSIONINFO_H
#include <QObject>
#include<QTimer>
#include"tcpsession.h"
#include"myjson.h"

class SessionInfo : public QObject
{
    Q_OBJECT

    friend class SessionInfoList;
signals:
    void SignalRead(SessionInfo*, const QByteArray &, int);
    void SignalDisconnect(int id);
    void SignalConnected();




public:
    SessionInfo(std::shared_ptr<TcpSession> &session);
    ~SessionInfo();


    //连接到服务端
    void Connect(const QString &host, quint16 port);
    //断开连接
    void Disconnect();
    //解包函数连接
    void AnalysisDataConnect();

    void Write(const char*buffer, int size);
    void Write(const QByteArray &buffer);

    int getId();

public:
    //断开连接回调
    std::function<void(void*)> OnDisConnected = nullptr;

private slots:
    void SlotRead(const QByteArray &data, int size);
    void SlotDisconnected();
    void SlotCheckHeart();

    void SlotHeartPack(HeartPack pack);
    void SlotChatPack(ChatPack pack);


signals:
    void SignalHeartPack(SessionInfo* , HeartPack);
    void SignalChatPack(SessionInfo* , ChatPack);

private:
    std::shared_ptr<TcpSession> Session_;
    MyJson *m_myjson;
    int m_id = 0;
    QTimer *m_timer;

    int m_number = 0;
    int m_HeartTimePrev;
    int m_HeartTime;

};

#endif // SESSIONINFO_H
