#ifndef MYJSON_H
#define MYJSON_H

#include <QObject>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QDateTime>
#include<QFile>
#define SUCCESS 0
#define FAIL -1

typedef struct HeartPack
{
    std::string time ;
    int number;
}HeartPack;


typedef struct ChatPack
{
    std::string chatBuff;
}ChatPack;


class MyJson : public QObject
{
    Q_OBJECT
public:

    MyJson(QWidget *parent = nullptr);

     int AnalysisPack(QByteArray str);

     int WriteJson(QByteArray &BA_Json , const HeartPack pack);
     int ReadJson(const QJsonObject obj , HeartPack &pack);

     int WriteJson(QByteArray &BA_Json , const ChatPack pack);
     int ReadJson(const QJsonObject obj , ChatPack &pack);

signals:
    void signal_HeartPackData(HeartPack pack);
    void signal_ChatPackData(ChatPack pack);
};

#endif // MYJSON_H
