#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>

class Player : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Player(QMap <QString, qreal> position);
    void setPosition(QMap<QString, qreal> position);

private:
    const QString PATH_TO_PLAYER_IMG = ":/img/player.png";
    const QString LEFT = "left";
    const QString RIGHT = "right";
    const QString UP = "up";
    const QString DOWN = "down";

    void onHold(QKeyEvent *event, bool hold);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

};

#endif // PLAYER_H
