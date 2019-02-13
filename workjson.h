#ifndef WORKJSON_H
#define WORKJSON_H

#include "player.h"

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>

class WorkJson : public QObject
{
    Q_OBJECT

private:
    WorkJson(const WorkJson& root) = delete;
    WorkJson& operator = (const WorkJson&) = delete;
    QString _nickname;
    QMap <QString, Player *> _players;

public:
    WorkJson(){}
    static WorkJson& Instance();
    void fromJson(const QString &data);
    void setNickname(QString nickname);
    QString getNickname();
    QString toJsonVerify(QString method);
    QString toJsonKey(const QString &key, bool hold);
signals:
    void signalSend(QString);
    void signalConnected();
};

#endif // WORKJSON_H
