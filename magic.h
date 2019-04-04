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

class Magic : public QObject {
    Q_OBJECT
public:
    Magic(QObject* parent);

    QString getNickname(); // get your nickname

    void setScene(QGraphicsScene* scene); // sets game scene
    void setNickname(const QString& nickname); // set your nickname
    void setSizePlayer(const QMap<QString, qreal> sizePlayer); // set size your player
    void setResolution(const QMap<QString, qreal> resolution); // set resolution game

    void draw(const QJsonObject dataJsonObj); // draw objects on game scene
    void delPlayer(const QString& nickname); // delete players by nickname
    void delBullets(const int id); // delete bullets by id
    void setWeapon(const QString &weapon);

    QMap<QString, Player*> getPlayers(); // get all players on the game scene
    QMap<int, Bullet*> getBullets(); // get all bullets on the game scene

    QMap<QString, qreal> getResolution(); // get resolution game
    QMap<QString, qreal> setShot(const QMap<QString, qreal> shot); // sets the mouse coordinates relative to the player

    QString getWeapon();

    bool isMine(const QString& nickname);

private:
    const int TEMP_WIDTH = 1920;
    const int TEMP_HEIGHT = 1080;
    const QPointF TEMP_HUD_POS = QPointF(0, 0);
    const qreal PADDING_TOP = 62;
    const qreal SIZE_ITEM_HUD = 30;

    QString _nickname;
    QString _weapon = "blaster";

    QMap<QString, Player*> _players;
    QMap<int, Bullet*> _bullets;
    QMap<QString, Scene*> _gameScene;

    QMap<QString, qreal> _sizePlayer;
    QMap<QString, qreal> _resolution;

    QGraphicsScene* _scene = nullptr;

    Camera _camera;
    HUD* _hud;

    bool isStop(const QPointF posNew, const QPointF posOld);

    void toScene(const QJsonObject sceneJsObj);
    void toPlayers(const QJsonArray playersJsonArr);
    void toBullets(const QJsonArray bulletsJsArr);
    void toHealth(const int health);
    void toScore(const int score);

    QGraphicsTextItem* _scoreBar = nullptr;
    QGraphicsTextItem* _healthHud = nullptr;

    QMap<QString, qreal> setPositionItems(QMap<QString, qreal> itemHUD);
    QMap<QString, qreal> itemPos(QPointF pos, const qreal size);
    QList<QMap<QString, qreal> > configureObj(const QJsonObject itemJsObj);
    bool isOutView(const QMap<QString, qreal> position);
    void createBullet(const QJsonValue bulletValue, const QMap<QString, qreal> position,
                      const QList<QMap<QString, qreal> > bulletCfg);
    void createPlayer(const QString &nickname, const QMap<QString, qreal> position, const QList<QMap<QString, qreal> > playerCfg);
    void updateMinePlayer(const QList<QMap<QString, qreal> > playerCfg, const QPair<int, int> dataHud);
};

#endif // MAGIC_H
