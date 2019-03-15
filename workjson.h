#ifndef WORKJSON_H
#define WORKJSON_H

#include "player.h"
#include "hud.h"
#include "bullet.h"
#include "scene.h"
#include "camera.h"

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QMap>

class WorkJson : public QObject
{
    Q_OBJECT

private:
    WorkJson(const WorkJson& root) = delete;
    WorkJson& operator = (const WorkJson&) = delete;

    QString _nickname;

    QMap <QString, Player *> _players;
    QMap <int, Bullet *> _bullets;
    QMap <QString, Scene *> _gameScene;
    QMap <QString, qreal> _viewCenter;
    QMap <QString, qreal> _sizePlayer;
    QMap <QString, qreal> _resolution;

    QGraphicsScene *_scene;
    QGraphicsView *_view;
    Camera _camera;
    HUD *_hud;
    QGraphicsTextItem *_scoreBar;
    QGraphicsTextItem *_healthHud;

    bool checkFields(const QJsonObject dataJsonObj);

    void toScene(const QJsonObject dataJsonObj);
    void toPlayers(const QJsonObject dataJsonObj);
    void toBullets(const QJsonObject dataJsonObj);
    void toHealth(const int health);
    void toJsonResurrection();
    void toScore(const int score);

    QMap <QString, qreal> setPositionItems(QMap<QString, qreal> itemHUD);

    bool isStop(const QPointF posNew, const QPointF posOld);
public:
    WorkJson(){}
    static WorkJson& Instance();

    void fromJson(const QString &data);
    void setNickname(const QString &nickname);
    void toJsonKey(const QString &key, const bool hold);
    void setScene(QGraphicsScene *scene);
    void setViewCenter(const QMap<QString, qreal> viewCenter);
    void setSizePlayer(const QMap<QString, qreal> sizePlayer);
    void setView(QGraphicsView *view);
    void toJsonShot(const QMap<QString, qreal> click);
    void setResolution(const QMap<QString, qreal> resolution);
    void toJsonCursor(const QPointF cursor);

    QGraphicsView *getView();

    Player *getPlayer();

    QMap<QString, qreal> getViewCenter();

    QString getNickname();
    QString toJsonVerify(const QString &method);

signals:
    void signalSend(const QString &);
    void signalConnected();

};

#endif // WORKJSON_H
