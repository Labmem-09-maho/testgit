#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QTextEdit>
#include<QComboBox>
#include<QPushButton>
#include<QLineEdit>
#include"sessioninfolist.h"
#include"tcpserver.h"

#include"myjson.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void StartServer();
    void AcceptSession(std::shared_ptr<TcpSession> session);
public slots:

    void readSession(SessionInfo *info, const QByteArray &data, int size);

    void slot_send();

    void slot_Disconnect(int id);


    void slot_readChat(SessionInfo *sessionInfo,ChatPack pack);

signals:
    void signal_send();


private:
    TcpServer *m_Server;
    SessionInfoList *m_SessionInfoList;

    MyJson *m_myjson;

    QTextEdit *m_textEdit;
    QLineEdit *m_LineEdit;
    QComboBox *m_comboBox;
    QPushButton *m_btnSend;
};
#endif // WIDGET_H
