#include "workjson.h"

#include <QJsonArray>
#include <QDebug>

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
        Player *player = new Player(position);
        qDebug() << "1";
        _scene->addItem(player);
        qDebug() << "2";
        _players.insert(_nickname, player);
        qDebug() << "3";

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
            _players[player.toObject().value("nickname").toString()]->setPosition(position);
        }
    }
}

void WorkJson::setScene(QGraphicsScene *scene)
{
    _scene = scene;
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

void WorkJson::toJsonKey(const QString &key, bool hold)
{
    QJsonObject dataJsonObj;
    dataJsonObj.insert("method", "control");
    dataJsonObj.insert("nickname", _nickname);
    dataJsonObj.insert("key", key);
    dataJsonObj.insert("hold", hold);
    QJsonDocument dataJsonDoc(dataJsonObj);
    QString data = dataJsonDoc.toJson(QJsonDocument::Compact);
    emit signalSend(data);
}
