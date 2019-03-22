#ifndef PLAYER_H
#define PLAYER_H

#include "control.h"

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>

class Player : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Player(const QString& _nickname, const QMap<QString, qreal> position, const QMap<QString, qreal> size);

    void setPosition(const QMap<QString, qreal> position);

    QMap<QString, qreal> getSize();

private:
    const QString PATH_TO_PLAYER_IMG = ":/img/player.png";

    QMap<QString, qreal> _size;
    Control _control;

protected:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
};

#endif // PLAYER_H
