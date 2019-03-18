#ifndef MAGIC_H
#define MAGIC_H

#include "player.h"
#include "hud.h"
#include "bullet.h"
#include "scene.h"
#include "camera.h"

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QMap>

class Magic : public QObject
{
    Q_OBJECT
public:
    Magic(QObject *parent);

    void setScene(QGraphicsScene *scene);   //
    void setNickname(const QString &nickname);  //
    void setViewCenter(const QMap<QString, qreal> viewCenter);  //
    void setSizePlayer(const QMap<QString, qreal> sizePlayer);  //
    void setView(QGraphicsView *view);  //
    void setResolution(const QMap<QString, qreal> resolution);  //

private:
    QMap <QString, Player *> _players;  //
    QMap <int, Bullet *> _bullets;  //
    QMap <QString, Scene *> _gameScene; //

    QMap <QString, qreal> _viewCenter;  //
    QMap <QString, qreal> _sizePlayer;  //
    QMap <QString, qreal> _resolution;  //

    QGraphicsScene *_scene; //
    QGraphicsView *_view;   //

    Camera _camera; //
    HUD *_hud;  //

    bool isStop(const QPointF posNew, const QPointF posOld); //

    void toScene(const QJsonObject dataJsonObj);    //
    void toPlayers(const QJsonObject dataJsonObj);  //
    void toBullets(const QJsonObject dataJsonObj);  //
    void toHealth(const int health);    //
    void toScore(const int score);  //

    QGraphicsTextItem *_scoreBar;   //
    QGraphicsTextItem *_healthHud;  //

    QGraphicsView *getView();   //

    Player *getPlayer();    //

    QMap <QString, qreal> getViewCenter();  //

    QString getNickname();  //

    QMap <QString, qreal> setPositionItems(QMap<QString, qreal> itemHUD);
};

#endif // MAGIC_H
