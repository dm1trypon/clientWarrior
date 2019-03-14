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

void WorkJson::toHealth(const int health)
{
    if (!_healthHud)
    {
        QMap <QString, qreal> itemHUD;
        itemHUD.insert("x", 170);
        itemHUD.insert("y", 62);
        itemHUD.insert("size", 30);

        _hud->addHealth(health, setPositionItems(itemHUD));
        _healthHud = _hud->getHealth();

        return;
    }

    if (_healthHud->toPlainText().toInt() == health)
    {
        return;
    }

    _healthHud->setPlainText(QString::number(health));
}

QMap <QString, qreal> WorkJson::setPositionItems(QMap <QString, qreal> itemHUD)
{
    itemHUD["x"] = _scene->width() * itemHUD["x"] / 1920;
    itemHUD["y"] = _scene->height() - _scene->height() * itemHUD["y"] / 1080;
    itemHUD["size"] = _scene->width() * itemHUD["size"] / 1920;

    return itemHUD;
}

void WorkJson::toScore(const int score)
{
    if (!_scoreBar)
    {
        QMap <QString, qreal> itemHUD;
        itemHUD.insert("x", 1750);
        itemHUD.insert("y", 62);
        itemHUD.insert("size", 30);

        _hud->addScore(score, setPositionItems(itemHUD));
        _scoreBar = _hud->getScore();

        return;
    }

    if (_scoreBar->toPlainText().toInt() == score)
    {
        return;
    }

    _scoreBar->setPlainText(QString::number(score));

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
        gameScene->addBorder(_camera.setPositionObjects(position), size);
        _scene->addItem(gameScene);
        _scene->addItem(gameScene->getBorder());

        QMap <QString, qreal> positionHUD;
        positionHUD.insert("x", 0);
        positionHUD.insert("y", 0);

        QMap <QString, qreal> sizeHUD;
        sizeHUD.insert("width", _scene->width());
        sizeHUD.insert("height", _scene->height());

        _hud = new HUD(positionHUD, sizeHUD);

        QGraphicsPixmapItem *hud = _hud;
        _scene->addItem(hud);

        _gameScene.insert("scene", gameScene);
    }

    const QMap <QString, qreal> posCamScene = _camera.setPositionObjects(position);

    if (posCamScene["x"] == _gameScene["scene"]->x() &&
            posCamScene["y"] == _gameScene["scene"]->y())
    {
        return;
    }

    _gameScene["scene"]->setPosition(posCamScene);
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
            const int score = playerValue.toObject().value("score").toInt();

            _camera.setSizePlayer(size);
            _camera.setOffsetFactor(position, _viewCenter);
            toHealth(playerValue.toObject().value("life").toInt());
            toScore(score);
        }

        if (!_players.contains(nickname))
        {
            Player *player = new Player(nickname, _camera.setPositionObjects(position), size);
            _scene->addItem(player);
            _players.insert(nickname, player);
        }

        if (position["x"] == _players[nickname]->x() &&
                position["y"] == _players[nickname]->y())
        {
            return;
        }

        _players[nickname]->setPosition(_camera.setPositionObjects(position));
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

void WorkJson::setResolution(const QMap <QString, qreal> resolution)
{
    _resolution = resolution;
}

void WorkJson::setScene(QGraphicsScene *scene)
{
    _scene = scene;
}

void WorkJson::setViewCenter(const QMap <QString, qreal> viewCenter)
{
    _viewCenter = viewCenter;
}

QMap <QString, qreal> WorkJson::getViewCenter()
{
    return _viewCenter;
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

void WorkJson::onCursor(const QPointF cursor)
{
    emit signalCursor(cursor);
}

void WorkJson::setView(QGraphicsView *view)
{
    _view = view;
}

QGraphicsView* WorkJson::getView()
{
    return _view;
}

Player* WorkJson::getPlayer()
{
    qDebug() << _nickname << _players.keys();
    return _players[_nickname];
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
