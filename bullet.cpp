#include "bullet.h"

Bullet::Bullet(const QString &weapon, const QMap<QString, qreal> position, const QMap<QString, qreal> size)
    : QObject()
    , QGraphicsPixmapItem(nullptr)
{
    setPixmap(QPixmap(PATH_TO_BULLET_IMG + weapon + ".png").scaled(static_cast<int>(size["width"]), static_cast<int>(size["height"])));
    setPos(position["x"], position["y"]);
    setZValue(2);
    setTransformOriginPoint(size["width"] / 2, size["height"] / 2);
}

void Bullet::setPosition(const QMap<QString, qreal> position)
{
    setPos(position["x"], position["y"]);
}
