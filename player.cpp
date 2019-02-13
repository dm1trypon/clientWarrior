#include "player.h"
#include "workjson.h"

#include <QDebug>

Player::Player(QMap <QString, qreal> position) : QObject(), QGraphicsPixmapItem (nullptr)
{
    setPixmap(QPixmap(PATH_TO_PLAYER_IMG));
    qDebug() << "Player has been created (" << position["x"] << ":" << position["y"] << ")";
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

void Player::setPosition(QMap <QString, qreal> position)
{
    setPos(position["x"], position["y"]);
}

void Player::keyPressEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
    {
        return;
    }

    qDebug() << "Press!";

    onHold(event, true);
}

void Player::keyReleaseEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
    {
        return;
    }

    qDebug() << "Release!";

    onHold(event, false);
}

void Player::onHold(QKeyEvent *event, bool hold)
{
    switch (event->key()) {
    case Qt::Key_Left:
        WorkJson::Instance().toJsonKey("left", hold);
        break;
    case Qt::Key_Right:
        WorkJson::Instance().toJsonKey("right", hold);
        break;
    case Qt::Key_Up:
        WorkJson::Instance().toJsonKey("up", hold);
        break;
    case Qt::Key_Down:
        WorkJson::Instance().toJsonKey("down", hold);
        break;
    default:
        break;
    }
}
