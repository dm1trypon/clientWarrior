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

void WorkJson::toLifes(const int life)
{
//    if (life == _lifes.count())
//    {
//        return;
//    }

//    if (life < _lifes.count())
//    {
//        _lifes.last()->deleteLater();
//        return;
//    }

    QMap <QString, qreal> size;
    size.insert("width", 40);
    size.insert("height", 40);

    qreal padding = 0;

    while (life >= _lifes.count())
    {
        qDebug() << _lifes.count();
        QMap <QString, qreal> position;
        position.insert("x", padding + 20);
        position.insert("y", 20);

        padding = position["x"] + size["width"];

        qDebug() << position << size;
        Life *life = new Life(position, size);
        _scene->addItem(life);
        _lifes.append(life);
    }
}

void WorkJson::toScene(const QJsonObject dataJsonObj)
{
    QJsonObject sceneJsonObject = dataJsonObj.value("scene").toObject();

    QMap <QString, qreal> position;
    position.insert("x", sceneJsonObject.value("pos_x").toInt());
    position.insert("y", sceneJsonObject.value("pos_y").toInt());

    QMap <QString, qreal> size;
    size.insert("width", sceneJsonObject.value("width").toInt());
    size.insert("height", sceneJsonObject.value("height").toInt());

    if (!_gameScene.contains("scene"))
    {
        Scene *gameScene = new Scene(_camera.setPositionObjects(position), size);
        _scene->addItem(gameScene);
        _gameScene.insert("scene", gameScene);
    }

    _gameScene["scene"]->setPosition(_camera.setPositionObjects(position));
}

void WorkJson::toPlayers(const QJsonObject dataJsonObj)
{
    QJsonArray playersJsonArr = dataJsonObj.value("players").toArray();

    foreach (QJsonValue playerValue, playersJsonArr)
    {
        const QString nickname = playerValue.toObject().value("nickname").toString();

        QMap <QString, qreal> position;
        position.insert("x", playerValue.toObject().value("pos_x").toInt());
        position.insert("y", playerValue.toObject().value("pos_y").toInt());

        QMap <QString, qreal> size;
        size.insert("width", playerValue.toObject().value("width").toInt());
        size.insert("height", playerValue.toObject().value("height").toInt());

        if (nickname == _nickname)
        {
            _camera.setSizePlayer(size);
            _camera.setOffsetFactor(position, _viewCenter);
            toLifes(playerValue.toObject().value("life").toInt());
        }

        if (!_players.contains(nickname))
        {
            Player *player = new Player(nickname, _camera.setPositionObjects(position), size);
            _scene->addItem(player);
            _players.insert(nickname, player);
        }

        _players[playerValue.toObject().value("nickname").toString()]->setPosition(_camera.setPositionObjects(position));
    }
}

void WorkJson::toBullets(const QJsonObject dataJsonObj)
{
    QJsonArray bulletsJsonArr = dataJsonObj.value("bullets").toArray();

    foreach (QJsonValue bulletValue, bulletsJsonArr)
    {
        const QString nickname = bulletValue.toObject().value("nickname").toString();
        const int id = bulletValue.toObject().value("id_bullet").toInt();

        QMap <QString, qreal> position;
        position.insert("x", bulletValue.toObject().value("pos_x").toDouble());
        position.insert("y", bulletValue.toObject().value("pos_y").toDouble());

        if (!_bullets.contains(id))
        {
            QMap <QString, qreal> size;
            size.insert("width", bulletValue.toObject().value("width").toInt());
            size.insert("height", bulletValue.toObject().value("height").toInt());

            Bullet *bullet = new Bullet(nickname, id, _camera.setPositionObjects(position), size);
            _scene->addItem(bullet);
            _bullets.insert(id, bullet);
        }

        _bullets[id]->setPosition(_camera.setPositionObjects(position));
    }
}

void WorkJson::setScene(QGraphicsScene *scene)
{
    _scene = scene;
}

void WorkJson::setViewCenter(const QMap <QString, qreal> viewCenter)
{
    _viewCenter = viewCenter;
}

void WorkJson::setSizePlayer(const QMap <QString, qreal> sizePlayer)
{
    _sizePlayer = sizePlayer;
}

void WorkJson::setNickname(const QString &nickname)
{
    _nickname = nickname;
}

QString WorkJson::getNickname()
{
    return _nickname;
}

void WorkJson::setView(QGraphicsView *view)
{
    _view = view;
}

QGraphicsView* WorkJson::getView()
{
    return _view;
}

QString WorkJson::toJsonVerify(const QString &method)
{
    QJsonObject dataJsonObj;
    dataJsonObj.insert("method", method);
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

void WorkJson::toJsonClick(const QMap <QString, qreal> click)
{
    QMap <QString, qreal> newClick = _camera.setClick(click, _gameScene["scene"]->getPosition());
    QJsonObject dataJsonObj;
    dataJsonObj.insert("method", "shot");
    dataJsonObj.insert("nickname", _nickname);
    dataJsonObj.insert("x", newClick["x"]);
    dataJsonObj.insert("y", newClick["y"]);

    const QJsonDocument dataJsonDoc(dataJsonObj);
    const QString data = dataJsonDoc.toJson(QJsonDocument::Compact);

    emit signalSend(data);
}
