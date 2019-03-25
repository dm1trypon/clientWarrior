#include "workjson.h"

#include <QJsonArray>
#include <QDebug>

WorkJson& WorkJson::Instance()
{
    static WorkJson theSingleInstance;
    return theSingleInstance;
}

void WorkJson::fromJson(const QString& data)
{
    const QJsonObject dataJsonObj = QJsonDocument::fromJson(data.toUtf8()).object();
    const QString method = dataJsonObj.value(METHOD).toString();
    const QString myName = _magic->getNickname();

    QString nickname;

    if (dataJsonObj.contains(NICKNAME)) {
        nickname = dataJsonObj.value(NICKNAME).toString();
    }

    if (method == VERIFY) {
        emit signalSend(toJsonVerify());
    }

    if (method == CONNECTION) {
        if (nickname != myName) {
            return;
        }

        emit signalConnected();
    }

    if (method == DISCONNECTION) {
        if (!_magic->getPlayers().contains(nickname)) {
            qWarning() << "Warning! Player is not exist for delete from game scene!";

            return;
        }

        _magic->delPlayer(nickname);
    }

    if (method == REMOVE) {
        const int id = dataJsonObj.value("id_bullet").toInt();

        if (!_magic->getBullets().contains(id)) {
            qWarning() << "Warning! Bullet is not exist for delete from game scene!";
            return;
        }

        _magic->delBullets(id);
    }

    if (method == OBJECTS) {
        if (!checkFields(dataJsonObj)) {
            return;
        }

        _magic->draw(dataJsonObj);
    }

    if (method == DIE) {
        if (!_magic->getPlayers().contains(nickname)) {
            qWarning() << "Warning! Player is not exist for delete from game scene!";
            return;
        }

        _magic->delPlayer(nickname);

        if (myName != nickname) {
            return;
        }

        toJsonResurrection();

        return;
    }
}

bool WorkJson::checkFields(const QJsonObject dataJsonObj)
{
    if (!dataJsonObj.contains("players")) {
        qWarning() << "Warning! Field 'players' is not exist!";
        return false;
    }

    if (!dataJsonObj.contains("scene")) {
        qWarning() << "Warning! Field 'scene' is not exist!";
        return false;
    }

    if (!dataJsonObj.value("players").isArray()) {
        qWarning() << "Warning! Field 'players' is not array!";
        return false;
    }

    if (!dataJsonObj.value("scene").isObject()) {
        qWarning() << "Warning! Field 'scene' is not object!";
        return false;
    }

    return true;
}

void WorkJson::toJsonCursor(const QPointF cursor)
{
    QJsonObject dataJsonObj;
    dataJsonObj.insert("method", "cursor");
    dataJsonObj.insert("pos_x", cursor.x());
    dataJsonObj.insert("pos_y", cursor.y());
    dataJsonObj.insert("nickname", _magic->getNickname());

    const QJsonDocument dataJsonDoc(dataJsonObj);
    const QString data(dataJsonDoc.toJson(QJsonDocument::Compact));

    emit signalSend(data);
}

QString WorkJson::toJsonVerify()
{
    const QMap<QString, qreal> resolution = _magic->getResolution();

    QJsonObject dataJsonObj;
    dataJsonObj.insert("method", VERIFY);
    dataJsonObj.insert("width", resolution["width"] / 2);
    dataJsonObj.insert("height", resolution["height"] / 2);
    dataJsonObj.insert("nickname", _magic->getNickname());

    const QJsonDocument dataJsonDoc(dataJsonObj);
    const QString data(dataJsonDoc.toJson(QJsonDocument::Compact));

    return data;
}

void WorkJson::toJsonKey(const QString& key, const bool hold)
{
    QJsonObject dataJsonObj;
    dataJsonObj.insert("method", "control");
    dataJsonObj.insert("nickname", _magic->getNickname());
    dataJsonObj.insert("key", key);
    dataJsonObj.insert("hold", hold);

    const QJsonDocument dataJsonDoc(dataJsonObj);
    const QString data = dataJsonDoc.toJson(QJsonDocument::Compact);

    emit signalSend(data);
}

void WorkJson::toJsonShot(const QMap<QString, qreal> shot)
{
    if (!_magic->getPlayers().contains(_magic->getNickname())) {
        return;
    }

    const QMap<QString, qreal> newShot = _magic->setShot(shot);

    QJsonObject dataJsonObj;
    dataJsonObj.insert("method", "shot");
    dataJsonObj.insert("nickname", _magic->getNickname());
    dataJsonObj.insert("weapon", _magic->getWeapon());
    dataJsonObj.insert("x", newShot["x"]);
    dataJsonObj.insert("y", newShot["y"]);

    const QJsonDocument dataJsonDoc(dataJsonObj);
    const QString data = dataJsonDoc.toJson(QJsonDocument::Compact);

    emit signalSend(data);
}

void WorkJson::toJsonResurrection()
{
    QJsonObject dataJsonObj;
    dataJsonObj.insert("method", "resurrection");
    dataJsonObj.insert("nickname", _magic->getNickname());

    const QJsonDocument dataJsonDoc(dataJsonObj);
    const QString data = dataJsonDoc.toJson(QJsonDocument::Compact);

    emit signalSend(data);
}

Magic* WorkJson::setMagic()
{
    if (_magic) {
        _magic->deleteLater();
    }

    if (!_magic) {
        _magic = new Magic(this);
    }

    return _magic;
}

Magic* WorkJson::getMagic()
{
    return _magic;
}
