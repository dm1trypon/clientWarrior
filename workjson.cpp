#include "workjson.h"

#include <QJsonArray>

WorkJson &WorkJson::Instance()
{
    static WorkJson theSingleInstance;
    return theSingleInstance;
}

void WorkJson::fromJson(const QString &data)
{
    QJsonObject dataJsonObj = QJsonDocument::fromJson(data.toUtf8()).object();
    QString method = dataJsonObj.value("method").toString();

    if (method == "verify")
    {
        emit signalSend(toJsonVerify(method));
    }

    if (method == "connection")
    {
        _nickname = dataJsonObj.value("nickname").toString();
        QMap <QString, qreal> position;
        position.insert("x", dataJsonObj.value("pos_x").toInt());
        position.insert("y", dataJsonObj.value("pos_y").toInt());
        _players.insert(_nickname, new Player(position));

        if (dataJsonObj.value("nickname").toString() == _nickname)
        {
            emit signalConnected();
        }
    }

    if (method == "objects")
    {
        QJsonArray playersJsonArr = dataJsonObj.value("players").toArray();

        foreach (QJsonValue player, playersJsonArr)
        {
            QMap <QString, qreal> position;
            position.insert("x", player.toObject().value("pos_x").toInt());
            position.insert("y", player.toObject().value("pos_y").toInt());
//            player.toObject->setPosition(position);
        }
    }
}

void WorkJson::setNickname(QString nickname)
{
    _nickname = nickname;
}

QString WorkJson::getNickname()
{
    return _nickname;
}

QString WorkJson::toJsonVerify(QString method)
{
    QJsonObject dataJsonObj;
    dataJsonObj.insert("method", method);
    dataJsonObj.insert("nickname", _nickname);
    QJsonDocument dataJsonDoc(dataJsonObj);
    QString data(dataJsonDoc.toJson(QJsonDocument::Compact));
    return data;
}

QString WorkJson::toJsonKey(const QString &key, bool hold)
{
    QJsonObject dataJsonObj;
    dataJsonObj.insert("method", "control");
    dataJsonObj.insert("nickname", _nickname);
    dataJsonObj.insert("key", key);
    dataJsonObj.insert("hold", hold);
    QJsonDocument dataJsonDoc(dataJsonObj);
    QString data(dataJsonDoc.toJson(QJsonDocument::Compact));
    return data;
}
