#ifndef WORKJSON_H
#define WORKJSON_H

#include "player.h"
#include "scene.h"
#include "camera.h"

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QGraphicsScene>
#include <QMap>

class WorkJson : public QObject
{
    Q_OBJECT

private:
    WorkJson(const WorkJson& root) = delete;
    WorkJson& operator = (const WorkJson&) = delete;

    QString _nickname;
    QMap <QString, Player *> _players;
    QMap <QString, Scene *> _gameScene;
    QMap <QString, qreal> _viewCenter;
    QMap <QString, qreal> _sizePlayer;
    QGraphicsScene *_scene;
    Camera _camera;

    bool checkFields(const QJsonObject dataJsonObj);
    void toScene(const QJsonObject dataJsonObj);
    void toPlayers(const QJsonObject dataJsonObj);
public:
    WorkJson(){}
    static WorkJson& Instance();

    void fromJson(const QString &data);
    void setNickname(const QString &nickname);
    QString getNickname();
    QString toJsonVerify(const QString &method);
    void toJsonKey(const QString &key, const bool hold);
    void setScene(QGraphicsScene *scene);
    void setViewCenter(const QMap<QString, qreal> viewCenter);
    void setSizePlayer(const QMap<QString, qreal> sizePlayer);
signals:
    void signalSend(QString);
    void signalConnected();
};

#endif // WORKJSON_H
