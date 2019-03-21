#include "magic.h"

#include <QJsonArray>
#include <QDebug>

Magic::Magic(QObject *parent) : QObject(parent)
{

}

void Magic::toHealth(const int health)
{
    if (!_healthHud) {
        QMap <QString, qreal> itemHUD;
        itemHUD.insert("x", 170);
        itemHUD.insert("y", 62);
        itemHUD.insert("size", 30);

        _hud->addHealth(health, setPositionItems(itemHUD));
        _healthHud = _hud->getHealth();

        return;
    }

    if (_healthHud->toPlainText().toInt() == health) {
        return;
    }

    _healthHud->setPlainText(QString::number(health));
}

QMap <QString, qreal> Magic::setPositionItems(QMap <QString, qreal> itemHUD)
{
    const qreal width = _scene->width();
    const qreal height = _scene->height();

    itemHUD["x"] = width * itemHUD["x"] / 1920;
    itemHUD["y"] = height - height * itemHUD["y"] / 1080;
    itemHUD["size"] = width * itemHUD["size"] / 1920;

    return itemHUD;
}

void Magic::toScore(const int score)
{
    if (!_scoreBar) {
        QMap <QString, qreal> itemHUD;
        itemHUD.insert("x", 1750);
        itemHUD.insert("y", 62);
        itemHUD.insert("size", 30);

        _hud->addScore(score, setPositionItems(itemHUD));
        _scoreBar = _hud->getScore();

        return;
    }

    if (_scoreBar->toPlainText().toInt() == score) {
        return;
    }

    _scoreBar->setPlainText(QString::number(score));

}

void Magic::toScene(const QJsonObject dataJsonObj)
{
    const QJsonObject sceneJsonObj = dataJsonObj.value("scene").toObject();

    QMap <QString, qreal> position;
    position.insert("x", sceneJsonObj.value("pos_x").toInt());
    position.insert("y", sceneJsonObj.value("pos_y").toInt());

    position = _camera.setPositionObjects(position);

    QMap <QString, qreal> size;
    size.insert("width", sceneJsonObj.value("width").toInt());
    size.insert("height", sceneJsonObj.value("height").toInt());

    if (!_gameScene.contains("scene")) {
        Scene *gameScene = new Scene(position, size);

        gameScene->addBorder(position, size);

        _scene->addItem(gameScene);
        _scene->addItem(gameScene->getBorder());

        _hud = new HUD(QPointF (0, 0), _scene->sceneRect());

        QGraphicsPixmapItem *hud = _hud;
        _scene->addItem(hud);

        _gameScene.insert("scene", gameScene);
        _gameScene["scene"]->setPosition(position);

    }

    if (isStop(QPointF(position["x"], position["y"]),
               _gameScene["scene"]->pos())) {
        return;
    }

    _gameScene["scene"]->setPosition(position);
}

void Magic::toPlayers(const QJsonObject dataJsonObj)
{
    const QJsonArray playersJsonArr = dataJsonObj.value("players").toArray();

    foreach (QJsonValue playerValue, playersJsonArr) {
        const QJsonObject playerJsonObj = playerValue.toObject();
        const QString nickname = playerJsonObj.value("nickname").toString();

        QMap <QString, qreal> position;
        position.insert("x", playerJsonObj.value("pos_x").toInt());
        position.insert("y", playerJsonObj.value("pos_y").toInt());

        QMap <QString, qreal> size;
        size.insert("width", playerJsonObj.value("width").toInt());
        size.insert("height", playerJsonObj.value("height").toInt());

        if (isConfigured(nickname)) {
            const int score = playerJsonObj.value("score").toInt();

            _camera.setSizePlayer(size);
            _camera.setOffsetFactor(position, _scene->sceneRect());

            toHealth(playerJsonObj.value("life").toInt());
            toScore(score);
            _isSet = true;
        }

        position = _camera.setPositionObjects(position);

        if (!_players.contains(nickname)) {
            Player *player = new Player(nickname, position, size);
            _scene->addItem(player);
            _players.insert(nickname, player);
        }

        _players[nickname]->setRotation(playerJsonObj.value("rotation").toDouble());

//        if (isStop(QPointF(position["x"], position["y"]),
//                   _players[nickname]->pos())) {
//            return;
//        }

        _players[nickname]->setPosition(position);
    }
}

void Magic::toBullets(const QJsonObject dataJsonObj)
{
    const QJsonArray bulletsJsonArr = dataJsonObj.value("bullets").toArray();

    foreach (QJsonValue bulletValue, bulletsJsonArr) {
        const QJsonObject bulletJsonObj = bulletValue.toObject();
        const QString nickname = bulletJsonObj.value("nickname").toString();
        const int id = bulletJsonObj.value("id_bullet").toInt();

        QMap <QString, qreal> position;
        position.insert("x", bulletJsonObj.value("pos_x").toDouble());
        position.insert("y", bulletJsonObj.value("pos_y").toDouble());

        position = _camera.setPositionObjects(position);

        if (!_bullets.contains(id)) {
            QMap <QString, qreal> size;
            size.insert("width", bulletJsonObj.value("width").toInt());
            size.insert("height", bulletJsonObj.value("height").toInt());

            Bullet *bullet = new Bullet(nickname, id, position, size);
            _scene->addItem(bullet);
            _bullets.insert(id, bullet);
        }

        _bullets[id]->setPosition(position);
    }
}

void Magic::draw(const QJsonObject dataJsonObj)
{
    toScene(dataJsonObj);
    toPlayers(dataJsonObj);
    toBullets(dataJsonObj);
}

QMap <QString, Player *> Magic::getPlayers()
{
    return _players;
}

QMap <int, Bullet *> Magic::getBullets()
{
    return _bullets;
}

void Magic::delPlayer(const QString &nickname)
{
    _players[nickname]->deleteLater();
    _players.remove(nickname);
}

void Magic::delBullets(const int id)
{
    _bullets[id]->deleteLater();
    _bullets.remove(id);
}

void Magic::setResolution(const QMap <QString, qreal> resolution)
{
    _resolution = resolution;
}

QMap <QString, qreal> Magic::getResolution()
{
    return _resolution;
}

QMap <QString, qreal> Magic::setShot(const QMap <QString, qreal> shot)
{
    return _camera.setShot(shot);
}

void Magic::setScene(QGraphicsScene *scene)
{
    _scene = scene;
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

bool Magic::isStop(const QPointF posNew, const QPointF posOld)
{
    return posNew == posOld;
}

bool Magic::isConfigured(const QString &nickname)
{
    return nickname == _nickname;
}
