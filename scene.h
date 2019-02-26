#ifndef SCENE_H
#define SCENE_H

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

private:
    const QString PATH_TO_SCENE_IMG = ":/img/scene.jpg";

    QMap <QString, qreal> _position;
    QMap <QString, qreal> _cursor;

    void setCursorPosition(QGraphicsSceneMouseEvent *mouseEvent);
    void addScoreBar();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
private slots:
    void onPositionCursor();
};


#endif // SCENE_H
