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
    if (_healthHud)
    {
        return;
    }

    _bar->addHealth(health);
    _healthHud = _bar->getHealth();
    qDebug() << _healthHud->x() << _healthHud->y();
}

void WorkJson::toScore(const int score)
{
    if (_scoreBar)
    {
        _scoreBar->setPlainText(QString::number(score));

        return;
    }

    QMap <QString, qreal> positionScore;
    positionScore.insert("x", _view->width() - 400);
    positionScore.insert("y", 0);

    _scoreBar = _bar->addScore(score, positionScore);
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

        QMap <QString, qreal> positionBar;
        positionBar.insert("x", 0);
        positionBar.insert("y", 0);

        _bar = new HUD(positionBar, _resolution);

        QGraphicsPixmapItem *bar = _bar;
        _scene->addItem(bar);

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

void WorkJson::setResolution(const QMap <QString, qreal> resolution)
{
    _resolution = resolution;
}

void WorkJson::setScene(QGraphicsScene *scene)
{
    _scene = scene;
}

void WorkJson::setPositionScoreBar()
{
    if (_gameScene.isEmpty())
    {
        return;
    }
    _scoreBar->setPos(_view->width() - 100, -15);
//    _bar->setRect(0, 0, _view->width(), 70);
}

void WorkJson::setViewCenter(const QMap <QString, qreal> viewCenter)
{
    _viewCenter = viewCenter;
    qDebug() << "_viewCenter:" << _viewCenter;
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
