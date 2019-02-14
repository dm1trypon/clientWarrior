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
        QString nickname = dataJsonObj.value("nickname").toString();

        if (dataJsonObj.value("nickname").toString() == nickname)
        {
            emit signalConnected();
        }
    }

    if (method == "disconnection")
    {
        QString nickname = dataJsonObj.value("nickname").toString();

        if (!_players.contains(nickname))
        {
            qWarning() << "Warning! Player is not exist for delete from game scene!";
            return;
        }

        _players[nickname]->deleteLater();
        _players.remove(nickname);
    }

    if (method == "objects")
    {
        QJsonArray playersJsonArr = dataJsonObj.value("players").toArray();

        foreach (QJsonValue playerValue, playersJsonArr)
        {
            QString nickname = playerValue.toObject().value("nickname").toString();

            QMap <QString, qreal> position;
            position.insert("x", playerValue.toObject().value("pos_x").toInt());
            position.insert("y", playerValue.toObject().value("pos_y").toInt());

            if (!_players.contains(nickname))
            {
                Player *player = new Player(position);
                _scene->addItem(player);
                _players.insert(playerValue.toObject().value("nickname").toString(), player);
            }

            _players[playerValue.toObject().value("nickname").toString()]->setPosition(position);
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
