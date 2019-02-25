#ifndef LIFE_H
#define LIFE_H

#include <QGraphicsPixmapItem>
#include <QObject>

class Life : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
private:
    const QString PATH_TO_LIFE_IMG = ":/img/player.png";

public:
    Life(const QMap <QString, qreal> position, const QMap <QString, qreal> size);
};

#endif // LIFE_H
