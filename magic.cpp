#include "magic.h"

#include <QJsonArray>
#include <QDebug>

Magic::Magic(QObject* parent)
    : QObject(parent)
{
}

QMap<QString, qreal> Magic::itemPos(QPointF pos, const qreal size)
{
    QMap<QString, qreal> itemPos;
    itemPos.insert("x", pos.rx());
    itemPos.insert("y", pos.ry());
    itemPos.insert("size", size);

    return itemPos;
}

void Magic::setWeapon(const QString &weapon)
{
    _weapon = weapon;
}

QString Magic::getWeapon()
{
    return _weapon;
}

void Magic::toHealth(const int health)
{
    if (!_healthHud) {
        const qreal width = 170;

        _hud->addTextElement(health, "health", setPositionItems(itemPos(
                                    QPointF(width, PADDING_TOP), SIZE_ITEM_HUD)), Qt::green);

        _healthHud = _hud->getTextElement("health");

        return;
    }

    if (_healthHud->toPlainText().toInt() == health) {
        return;
    }

    _healthHud->setPlainText(QString::number(health));
}

QMap<QString, qreal> Magic::setPositionItems(QMap<QString, qreal> itemHUD)
{
    const qreal width = _scene->width();
    const qreal height = _scene->height();

    itemHUD["x"] = width * itemHUD["x"] / TEMP_WIDTH;
    itemHUD["y"] = height - height * itemHUD["y"] / TEMP_HEIGHT;
    itemHUD["size"] = width * itemHUD["size"] / TEMP_WIDTH;

    return itemHUD;
}

void Magic::toScore(const int score)
{
    if (!_scoreBar) {
        const qreal width = 1750;

        _hud->addTextElement(score, "score", setPositionItems(itemPos(
                                    QPointF(width, PADDING_TOP), SIZE_ITEM_HUD)), Qt::yellow);

        _scoreBar = _hud->getTextElement("score");

        return;
    }

    if (_scoreBar->toPlainText().toInt() == score) {
        return;
    }

    _scoreBar->setPlainText(QString::number(score));
}

QList<QMap<QString, qreal>> Magic::configureObj(const QJsonObject itemJsObj)
{
    QMap<QString, qreal> position;
    position.insert("x", itemJsObj.value("pos_x").toDouble());
    position.insert("y", itemJsObj.value("pos_y").toDouble());

    QMap<QString, qreal> size;
    size.insert("width", itemJsObj.value("width").toDouble());
    size.insert("height", itemJsObj.value("height").toDouble());

    return { position, size };
}

void Magic::toScene(const QJsonObject dataJsonObj)
{
    const QJsonObject sceneJsObj = dataJsonObj.value("scene").toObject();
    const QList<QMap<QString, qreal> > sceneCfg = configureObj(sceneJsObj);
    const QMap<QString, qreal> position = _camera.setPositionObjects(sceneCfg.first());

    if (!_gameScene.contains("scene")) {
        Scene* gameScene = new Scene(position, sceneCfg.last());

        gameScene->addBorder(position, sceneCfg.last());

        _scene->addItem(gameScene);
        _scene->addItem(gameScene->getBorder());

        _hud = new HUD(TEMP_HUD_POS, _scene->sceneRect());

        QGraphicsPixmapItem* hud = _hud;
        _scene->addItem(hud);

        _gameScene.insert("scene", gameScene);
        _gameScene["scene"]->setPosition(position);
    }

    _gameScene["scene"]->setPosition(position);
}

void Magic::toPlayers(const QJsonObject dataJsonObj)
{
    const QJsonArray playersJsonArr = dataJsonObj.value("players").toArray();

    foreach (QJsonValue playerValue, playersJsonArr) {
        const QJsonObject playerJsObj = playerValue.toObject();
        const QString nickname = playerJsObj.value("nickname").toString();
        const QList<QMap<QString, qreal> > playerCfg = configureObj(playerJsObj);

        if (isMine(nickname)) {
            const int score = playerJsObj.value("score").toInt();

            _camera.setSizePlayer(playerCfg.last());
            _camera.setOffsetFactor(playerCfg.first(), _scene->sceneRect());

            toHealth(playerJsObj.value("life").toInt());
            toScore(score);
        }

        const QMap<QString, qreal> position = _camera.setPositionObjects(playerCfg.first());

        if (!_players.contains(nickname)) {
            Player* player = new Player(nickname, position, playerCfg.last());
            _scene->addItem(player);
            _players.insert(nickname, player);
        }

        _players[nickname]->setRotation(playerJsObj.value("rotation").toDouble());

        if (isStop(QPointF(playerCfg.first()["x"], playerCfg.first()["y"]),
                _players[nickname]->pos())) {
            return;
        }

        _players[nickname]->setPosition(position);
    }
}

void Magic::toBullets(const QJsonObject dataJsonObj)
{
    const QJsonArray bulletsJsArr = dataJsonObj.value("bullets").toArray();

    foreach (QJsonValue bulletValue, bulletsJsArr) {
        const QJsonObject bulletJsObj = bulletValue.toObject();
        const QString weapon = bulletJsObj.value("weapon").toString();
        const int id = bulletJsObj.value("id_bullet").toInt();
        const QList<QMap<QString, qreal>> bulletCfg = configureObj(bulletJsObj);
        const QMap<QString, qreal> position = _camera.setPositionObjects(bulletCfg.first());

        if (!_bullets.contains(id)) {
            Bullet* bullet = new Bullet(weapon, position, bulletCfg.last());
            _scene->addItem(bullet);
            bullet->setRotation(bulletJsObj.value("rotation").toDouble());
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

QMap<QString, Player*> Magic::getPlayers() { return _players; }

QMap<int, Bullet*> Magic::getBullets() { return _bullets; }

void Magic::delPlayer(const QString& nickname)
{
    _players[nickname]->deleteLater();
    _players.remove(nickname);
}

void Magic::delBullets(const int id)
{
    _bullets[id]->deleteLater();
    _bullets.remove(id);
}

void Magic::setResolution(const QMap<QString, qreal> resolution)
{
    _resolution = resolution;
}

QMap<QString, qreal> Magic::getResolution() { return _resolution; }

QMap<QString, qreal> Magic::setShot(const QMap<QString, qreal> shot)
{
    return _camera.setShot(shot);
}

void Magic::setScene(QGraphicsScene* scene) { _scene = scene; }

void Magic::setSizePlayer(const QMap<QString, qreal> sizePlayer)
{
    _sizePlayer = sizePlayer;
}

void Magic::setNickname(const QString& nickname) { _nickname = nickname; }

QString Magic::getNickname() { return _nickname; }

bool Magic::isStop(const QPointF posNew, const QPointF posOld)
{
    return posNew == posOld;
}

bool Magic::isMine(const QString& nickname)
{
    return nickname == _nickname;
}
