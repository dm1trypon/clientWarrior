#ifndef WORKJSON_H
#define WORKJSON_H

#include "magic.h"

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

class WorkJson : public QObject
{
    Q_OBJECT

private:
    WorkJson(const WorkJson& root) = delete;
    WorkJson& operator = (const WorkJson&) = delete;

    QString _nickname;

    bool checkFields(const QJsonObject dataJsonObj);

    void toJsonResurrection();

    Magic *_magic = nullptr;

public:
    WorkJson(){}
    static WorkJson& Instance();

    void fromJson(const QString &data);
    void toJsonKey(const QString &key, const bool hold);
    void toJsonShot(const QMap<QString, qreal> click);
    void toJsonCursor(const QPointF cursor);

    QString toJsonVerify(const QString &method);

signals:
    void signalSend(const QString &);
    void signalConnected();

};

#endif // WORKJSON_H
