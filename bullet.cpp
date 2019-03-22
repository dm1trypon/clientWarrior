#include "bullet.h"

Bullet::Bullet(const QString& nickname, const int id, const QMap<QString, qreal> position, const QMap<QString, qreal> size)
    : QObject()
    , QGraphicsPixmapItem(nullptr)
    , _id(id)
    , _nickname(nickname)
{
    setPixmap(QPixmap(PATH_TO_BULLET_IMG).scaled(static_cast<int>(size["width"]), static_cast<int>(size["height"])));
    setPos(position["x"], position["y"]);
    setZValue(2);
}

void Bullet::setPosition(const QMap<QString, qreal> position)
{
    setPos(position["x"], position["y"]);
}
