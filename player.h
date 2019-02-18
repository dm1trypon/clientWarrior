#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>

class Player : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Player(const QString &_nickname, const QMap<QString, qreal> position, const QMap <QString, qreal> size);
    void setPosition(const QMap<QString, qreal> position);

private:
    const QString PATH_TO_PLAYER_IMG = ":/img/player.png";
    const QString LEFT = "left";
    const QString RIGHT = "right";
    const QString UP = "up";
    const QString DOWN = "down";

    void onHold(QKeyEvent *event, const bool hold);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

};

#endif // PLAYER_H
