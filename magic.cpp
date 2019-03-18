#include "magic.h"

Magic::Magic(QObject *parent) : QObject(parent)
{

}

void Magic::toHealth(const int health)
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

QMap <QString, qreal> Magic::setPositionItems(QMap <QString, qreal> itemHUD)
{
    itemHUD["x"] = _scene->width() * itemHUD["x"] / 1920;
    itemHUD["y"] = _scene->height() - _scene->height() * itemHUD["y"] / 1080;
    itemHUD["size"] = _scene->width() * itemHUD["size"] / 1920;

    return itemHUD;
}

void Magic::toScore(const int score)
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

void Magic::toScene(const QJsonObject dataJsonObj)
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

    if (isStop(QPointF(posCamScene["x"], posCamScene["y"]),
               _gameScene["scene"]->pos()))
    {
        return;
    }

    _gameScene["scene"]->setPosition(posCamScene);
}

void Magic::toPlayers(const QJsonObject dataJsonObj)
{
    QJsonArray playersJsonArr = dataJsonObj.value("players").toArray();

    foreach (QJsonValue playerValue, playersJsonArr)
    {
        QString nickname = playerValue.toObject().value("nickname").toString();

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

        _players[nickname]->setRotation(playerValue.toObject().value("rotation").toDouble());

        if (isStop(QPointF(position["x"], position["y"]),
                   _players[nickname]->pos()))
        {
            return;
        }

        _players[nickname]->setPosition(_camera.setPositionObjects(position));
    }
}

void Magic::toBullets(const QJsonObject dataJsonObj)
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

void Magic::setResolution(const QMap <QString, qreal> resolution)
{
    _resolution = resolution;
}

void Magic::setScene(QGraphicsScene *scene)
{
    _scene = scene;
}

void Magic::setViewCenter(const QMap <QString, qreal> viewCenter)
{
    _viewCenter = viewCenter;
}

QMap <QString, qreal> Magic::getViewCenter()
{
    return _viewCenter;
}

void Magic::setSizePlayer(const QMap <QString, qreal> sizePlayer)
{
    _sizePlayer = sizePlayer;
}

void Magic::setNickname(const QString &nickname)
{
    _nickname = nickname;
}

QString Magic::getNickname()
{
    return _nickname;
}

void Magic::setView(QGraphicsView *view)
{
    _view = view;
}

QGraphicsView* Magic::getView()
{
    return _view;
}

Player* Magic::getPlayer()
{
    if (!_players.contains(_nickname))
    {
        return nullptr;
    }

    return _players[_nickname];
}

bool WorkJson::isStop(const QPointF posNew, const QPointF posOld)
{
    return posNew == posOld;
}
