#ifndef SCENE_H
#define SCENE_H

#include "player.h"

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>

class Scene : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Scene(const QMap <QString, qreal> position, const QMap <QString, qreal> size);
    void setPosition(const QMap <QString, qreal> position);
    QMap <QString, qreal> getPosition();
    QTimer _mouseTimer;

    QGraphicsRectItem *getBorder();
    void addBorder(const QMap<QString, qreal> position, const QMap<QString, qreal> size);

private:
    const QString PATH_TO_SCENE_IMG = ":/img/scene.jpg";
    const int HALF_G = 180;
    const qreal PI = 3.14;

    QMap <QString, qreal> _position;
    QMap <QString, qreal> _cursor;
    QGraphicsRectItem *_border;
    Player *_player = nullptr;

    void setCursorPosition(QGraphicsSceneMouseEvent *mouseEvent);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *);

private slots:
    void onPositionCursor();
    void rotate(const QPointF cursor);
};


#endif // SCENE_H
