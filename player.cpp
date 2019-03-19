#include "player.h"
#include "workjson.h"

#include <QDebug>

Player::Player(const QString &nickname, const QMap <QString, qreal> position, const QMap <QString, qreal> size) :
    QObject(), QGraphicsPixmapItem (nullptr),
    _size(size)
{
    setPixmap(QPixmap(PATH_TO_PLAYER_IMG).scaled(static_cast<int>(size["width"]), static_cast<int>(size["height"])));
    setPos(position["x"], position["y"]);
    setZValue(3);
    setTransformOriginPoint(size["width"] / 2, size["height"] / 2);

    qDebug() << "Player has been created (" << position["x"] << ":" << position["y"] << ")";

    if (WorkJson::Instance().getMagic()->getNickname() == nickname)
    {
        QMap <QString, qreal> size;
        size.insert("width", pixmap().width());
        size.insert("height", pixmap().height());
        setFlag(QGraphicsItem::ItemIsFocusable);
        setFocus();

        WorkJson::Instance().getMagic()->setSizePlayer(size);
    }
}

void Player::setPosition(const QMap <QString, qreal> position)
{
    setPos(position["x"], position["y"]);
}

void Player::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
    {
        return;
    }

    _control.onHold(event, true);
}

void Player::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
    {
        return;
    }

    _control.onHold(event, false);
}

QMap <QString, qreal> Player::getSize()
{
    return _size;
}
