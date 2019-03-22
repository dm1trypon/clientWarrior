#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>

class Scene : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    Scene(const QMap<QString, qreal> position, const QMap<QString, qreal> size);

    QGraphicsRectItem* getBorder();
    QMap<QString, qreal> getPosition();

    void addBorder(const QMap<QString, qreal> position, const QMap<QString, qreal> size);
    void setPosition(const QMap<QString, qreal> position);

private:
    const QString PATH_TO_SCENE_IMG = ":/img/scene.jpg";
    const int CLICK_SENSITIVITY = 10;

    QTimer _mouseTimer;

    QMap<QString, qreal> _position;
    QMap<QString, qreal> _cursor;
    QGraphicsRectItem* _border;

    void setCursorPosition(QGraphicsSceneMouseEvent* mouseEvent);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent*);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent*);

private slots:
    void onPositionCursor();
};

#endif // SCENE_H
