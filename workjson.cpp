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
    const QJsonObject dataJsonObj = QJsonDocument::fromJson(data.toUtf8()).object();
    const QString method = dataJsonObj.value("method").toString();

    if (method == "verify")
    {
        emit signalSend(toJsonVerify(method));
    }

    if (method == "connection")
    {
        QString nickname = dataJsonObj.value("nickname").toString();

        if (nickname == _nickname)
        {
            emit signalConnected();
        }
    }

    if (method == "disconnection")
    {
        const QString nickname = dataJsonObj.value("nickname").toString();

        if (!_players.contains(nickname))
        {
            qWarning() << "Warning! Player is not exist for delete from game scene!";
            return;
        }

        _players[nickname]->deleteLater();
        _players.remove(nickname);
    }

    if (method == "remove")
    {
        const int id = dataJsonObj.value("id_bullet").toInt();

        if (!_bullets.contains(id))
        {
            qWarning() << "Warning! Bullet is not exist for delete from game scene!";
            return;
        }

        _bullets[id]->deleteLater();
        _bullets.remove(id);
    }

    if (method == "objects")
    {
        if (!checkFields(dataJsonObj))
        {
            return;
        }

        toScene(dataJsonObj);
        toPlayers(dataJsonObj);
        toBullets(dataJsonObj);
    }

    if (method == "die")
    {
        const QString nickname = dataJsonObj.value("nickname").toString();

        if (!_players.contains(nickname))
        {
            qWarning() << "Warning! Player is not exist for delete from game scene!";
            return;
        }

        _players[nickname]->deleteLater();
        _players.remove(nickname);

        if (_nickname != nickname)
        {
            return;
        }

        toJsonResurrection();

        return;
    }
}

bool WorkJson::checkFields(const QJsonObject dataJsonObj)
{
    if (!dataJsonObj.contains("players"))
    {
        qWarning() << "Warning! Field 'players' is not exist!";
        return false;
    }

    if (!dataJsonObj.contains("scene"))
    {
        qWarning() << "Warning! Field 'scene' is not exist!";
        return false;
    }

    if (!dataJsonObj.value("players").isArray())
    {
        qWarning() << "Warning! Field 'players' is not array!";
        return false;
    }

    if (!dataJsonObj.value("scene").isObject())
    {
        qWarning() << "Warning! Field 'scene' is not object!";
        return false;
    }

    return true;
}

void WorkJson::toJsonCursor(const QPointF cursor)
{
    QJsonObject dataJsonObj;
    dataJsonObj.insert("method", "cursor");
    dataJsonObj.insert("pos_x", cursor.x());
    dataJsonObj.insert("pos_y", cursor.y());
    dataJsonObj.insert("nickname", _nickname);

    const QJsonDocument dataJsonDoc(dataJsonObj);
    const QString data(dataJsonDoc.toJson(QJsonDocument::Compact));

    emit signalSend(data);
}

QString WorkJson::toJsonVerify(const QString &method)
{
    QJsonObject dataJsonObj;
    dataJsonObj.insert("method", method);
    dataJsonObj.insert("width", _resolution["width"] / 2);
    dataJsonObj.insert("height", _resolution["height"] / 2);
    dataJsonObj.insert("nickname", _nickname);

    const QJsonDocument dataJsonDoc(dataJsonObj);
    const QString data(dataJsonDoc.toJson(QJsonDocument::Compact));

    return data;
}

void WorkJson::toJsonKey(const QString &key, const bool hold)
{
    QJsonObject dataJsonObj;
    dataJsonObj.insert("method", "control");
    dataJsonObj.insert("nickname", _nickname);
    dataJsonObj.insert("key", key);
    dataJsonObj.insert("hold", hold);

    const QJsonDocument dataJsonDoc(dataJsonObj);
    const QString data = dataJsonDoc.toJson(QJsonDocument::Compact);

    emit signalSend(data);
}

void WorkJson::toJsonShot(const QMap <QString, qreal> shot)
{ 
    if (!_players.contains(_nickname))
    {
        return;
    }

    const QMap <QString, qreal> newShot = _camera.setShot(shot);
    QJsonObject dataJsonObj;
    dataJsonObj.insert("method", "shot");
    dataJsonObj.insert("nickname", _nickname);
    dataJsonObj.insert("x", newShot["x"]);
    dataJsonObj.insert("y", newShot["y"]);

    const QJsonDocument dataJsonDoc(dataJsonObj);
    const QString data = dataJsonDoc.toJson(QJsonDocument::Compact);

    emit signalSend(data);
}

void WorkJson::toJsonResurrection()
{
    QJsonObject dataJsonObj;
    dataJsonObj.insert("method", "resurrection");
    dataJsonObj.insert("nickname", _nickname);

    const QJsonDocument dataJsonDoc(dataJsonObj);
    const QString data = dataJsonDoc.toJson(QJsonDocument::Compact);

    emit signalSend(data);
}

void WorkJson::setMagic()
{
    if (_magic)
    {
        _magic->deleteLater();
    }

    if (!_magic)
    {
        _magic = new Magic();
    }
}

Magic *WorkJson::getMagic()
{
    return _magic;
}
