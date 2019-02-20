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
        if (!checkFields(dataJsonObj))
        {
            return;
        }

        toScene(dataJsonObj);
        toPlayers(dataJsonObj);
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
        Scene *gameScene = new Scene(_camera.setPositionObjects(position, _sizePlayer), size);
        _scene->addItem(gameScene);
        _gameScene.insert("scene", gameScene);
    }

    _gameScene["scene"]->setPosition(_camera.setPositionObjects(position, _sizePlayer));
}

void WorkJson::toPlayers(const QJsonObject dataJsonObj)
{
    QJsonArray playersJsonArr = dataJsonObj.value("players").toArray();

    foreach (QJsonValue playerValue, playersJsonArr)
    {
        QString nickname = playerValue.toObject().value("nickname").toString();

        QMap <QString, qreal> position;
        position.insert("x", playerValue.toObject().value("pos_x").toInt());
        position.insert("y", playerValue.toObject().value("pos_y").toInt());

        if (nickname == _nickname)
        {
            _camera.setOffsetFactor(position, _viewCenter);
        }

        if (!_players.contains(nickname))
        {
            QMap <QString, qreal> size;
            size.insert("width", playerValue.toObject().value("width").toInt());
            size.insert("height", playerValue.toObject().value("height").toInt());

            Player *player = new Player(nickname, _camera.setPositionObjects(position, _sizePlayer), size);
            _scene->addItem(player);
            _players.insert(playerValue.toObject().value("nickname").toString(), player);
        }

        _players[playerValue.toObject().value("nickname").toString()]->setPosition(_camera.setPositionObjects(position, _sizePlayer));
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
    QJsonDocument dataJsonDoc(dataJsonObj);
    QString data(dataJsonDoc.toJson(QJsonDocument::Compact));
    return data;
}

void WorkJson::toJsonKey(const QString &key, const bool hold)
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
