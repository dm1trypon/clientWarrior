#ifndef BULLET_H
#define BULLET_H

#include <QObject>
#include <QGraphicsPixmapItem>

class Bullet : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Bullet(const QString &weapon, const QMap<QString, qreal> position, const QMap<QString, qreal> size);

    void setPosition(const QMap<QString, qreal> position);

private:
    const QString PATH_TO_BULLET_IMG = ":/img/";

    QMap<QString, qreal> _speed;
    QMap<QString, qreal> _size;
};

#endif // BULLET_H
