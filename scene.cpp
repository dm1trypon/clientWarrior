#include "scene.h"
#include "workjson.h"

#include <QDebug>
#include <QBrush>
#include <QImage>
#include <QtMath>

Scene::Scene(const QMap <QString, qreal> position, const QMap <QString, qreal> size) :
    QObject(), QGraphicsRectItem (nullptr)
{
    setRect(position["x"], position["y"], size["width"], size["height"]);
    setZValue(1);
    setAcceptHoverEvents(true);

    QImage image = QImage(PATH_TO_SCENE_IMG);
    QBrush brush = QBrush(image);

    setBrush(brush);

    qDebug() << "Scene has been created (" << position["x"] << ":" << position["y"] << ")";
    qDebug() << "Scene size: " << size["width"] << ":" << size["height"];

    connect(&_mouseTimer, &QTimer::timeout, this, &Scene::onPositionCursor);
    connect(&WorkJson::Instance(), &WorkJson::signalCursor, this, &Scene::rotate);
}

void Scene::addBorder(const QMap <QString, qreal> position, const QMap <QString, qreal> size)
{
    _border = new QGraphicsRectItem();
    _border->setRect(position["x"], position["y"], size["width"], size["height"]);
    _border->setZValue(4);

    QPen pen;
    pen.setWidth(50);
    pen.setColor(QColor(25, 25, 112));

    _border->setPen(pen);
}

QGraphicsRectItem * Scene::getBorder()
{
    return _border;
}

void Scene::setPosition(const QMap <QString, qreal> position)
{
    _position = position;

    _border->setPos(position["x"], position["y"]);
    setPos(position["x"], position["y"]);
}

QMap <QString, qreal> Scene::getPosition()
{
    return _position;
}

void Scene::onPositionCursor()
{
    WorkJson::Instance().toJsonShot(_cursor);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    setCursorPosition(mouseEvent);
    _mouseTimer.start(100);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    setCursorPosition(mouseEvent);
    _mouseTimer.stop();
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    setCursorPosition(mouseEvent);
}

void Scene::setCursorPosition(QGraphicsSceneMouseEvent *mouseEvent)
{
    QMap <QString, qreal> cursor;
    cursor.insert("x", mouseEvent->scenePos().x());
    cursor.insert("y", mouseEvent->scenePos().y());

    _cursor = cursor;
}

void Scene::rotate(const QPointF cursor)
{
    qDebug() << "Ebat'";

    _player = WorkJson::Instance().getPlayer();

    if (!_player)
    {
        qDebug() << "Какого хуя?!";
        return;
    }

    const QMap <QString, qreal> size = _player->getSize();

    _player->setTransformOriginPoint(size["width"] / 2, size["height"] / 2);

    const qreal cx = _player->x() + size["width"] / 2;
    const qreal cy = _player->y() + size["height"] / 2;
    const qreal angle = qAtan2(cursor.y() - cy, cursor.x() - cx) * HALF_G / PI;

    if (static_cast<int>(_player->rotation()) == static_cast<int>(angle))
    {
        return;
    }

    _player->setRotation(angle);
}
