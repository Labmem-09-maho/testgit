#include "myjson.h"


MyJson::MyJson(QWidget *parent)
{

}

int MyJson::AnalysisPack(QByteArray str)
{
    QJsonDocument doc = QJsonDocument::fromJson(str);
    if(doc.isObject())
    {
        QJsonObject obj = doc.object();
        QStringList keys = obj.keys();
        for(int i = 0 ; i <keys.size();++i)
        {
            QString key = keys.at(i);
            QJsonValue value = obj.value(key);
            if(key == "HeartPack")
            {
                QJsonObject subobj = value.toObject();
                HeartPack pack;
                ReadJson(subobj,pack);
                emit signal_HeartPackData(pack);
            }
            if(key == "ChatPack")
            {
                QJsonObject subobj = value.toObject();
                ChatPack pack;
                ReadJson(subobj,pack);
                emit signal_ChatPackData(pack);
            }
        }
    }
    return SUCCESS;
}

int MyJson::WriteJson(QByteArray &BA_Json, const HeartPack pack)
{
    QJsonObject obj;
    QJsonObject SubObj;
    SubObj.insert("time",QString::fromStdString(pack.time));
    SubObj.insert("number",pack.number);
    obj.insert("HeartPack",SubObj);
    QJsonDocument doc(obj);
    BA_Json = doc.toJson();
    return SUCCESS;
}

int MyJson::ReadJson(const QJsonObject obj, HeartPack &pack)
{
    if(obj.isEmpty())
        return FAIL;
    QStringList keys = obj.keys();
    for(int i = 0 ; i <keys.size();++i)
    {
        QString key = keys.at(i);
        QJsonValue value = obj.value(key);
        if(key == "number")
        {
            pack.number = value.toInt();
        }
        if(key == "time")
        {
            pack.time = value.toString().toStdString();
        }
    }
    return SUCCESS;
}



int MyJson::WriteJson(QByteArray &BA_Json, const ChatPack pack)
{
    QJsonObject obj;
    QJsonObject SubObj;
    SubObj.insert("chatBuff",QString::fromStdString(pack.chatBuff));
    obj.insert("ChatPack",SubObj);
    QJsonDocument doc(obj);
    BA_Json = doc.toJson();
    return SUCCESS;
}

int MyJson::ReadJson(const QJsonObject obj, ChatPack &pack)
{
    if(obj.isEmpty())
        return FAIL;
    QStringList keys = obj.keys();
    for(int i = 0 ; i <keys.size();++i)
    {
        QString key = keys.at(i);
        QJsonValue value = obj.value(key);
        if(key == "chatBuff")
        {
            pack.chatBuff = value.toString().toStdString();
        }

    }
    return SUCCESS;
}


