#ifndef MAGIC_H
#define MAGIC_H

#include "player.h"
#include "hud.h"
#include "bullet.h"
#include "scene.h"
#include "camera.h"

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMap>
#include <QJsonObject>

class Magic : public QObject
{
    Q_OBJECT
public:
    Magic(QObject *parent);

    QString getNickname();

    void setScene(QGraphicsScene *scene);
    void setNickname(const QString &nickname);
    void setSceneCenter(const QMap<QString, qreal> viewCenter);
    void setSizePlayer(const QMap<QString, qreal> sizePlayer);
    void setResolution(const QMap<QString, qreal> resolution);

    QMap <QString, Player *> getPlayers();
    QMap<int, Bullet *> getBullets();

    void draw(const QJsonObject dataJsonObj);
    void delPlayer(const QString &nickname);
    void delBullets(const int id);

    QMap<QString, qreal> getResolution();
    QMap<QString, qreal> setShot(const QMap<QString, qreal> shot);

private:
    QString _nickname;

    QMap <QString, Player *> _players;
    QMap <int, Bullet *> _bullets;
    QMap <QString, Scene *> _gameScene;

    QMap <QString, qreal> _sceneCenter;
    QMap <QString, qreal> _sizePlayer;
    QMap <QString, qreal> _resolution;

    QGraphicsScene *_scene = nullptr;

    Camera _camera;
    HUD *_hud;

    bool isStop(const QPointF posNew, const QPointF posOld);

    void toScene(const QJsonObject dataJsonObj);
    void toPlayers(const QJsonObject dataJsonObj);
    void toBullets(const QJsonObject dataJsonObj);
    void toHealth(const int health);
    void toScore(const int score);

    QGraphicsTextItem *_scoreBar = nullptr;
    QGraphicsTextItem *_healthHud = nullptr;

    QMap <QString, qreal> getSceneCenter();
    QMap <QString, qreal> setPositionItems(QMap<QString, qreal> itemHUD);
};

#endif // MAGIC_H
