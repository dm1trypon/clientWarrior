#include "scene.h"
#include "workjson.h"

#include <QDebug>
#include <QBrush>
#include <QImage>

Scene::Scene(const QMap <QString, qreal> position, const QMap <QString, qreal> size) :
    QObject(), QGraphicsRectItem (nullptr)
{
    setRect(position["x"], position["y"], size["width"], size["height"]);

    QImage image = QImage(PATH_TO_SCENE_IMG);
    QBrush brush = QBrush(image);

    setBrush(brush);

    qDebug() << "Scene has been created (" << position["x"] << ":" << position["y"] << ")";
    qDebug() << "Scene size: " << size["width"] << ":" << size["height"];

    connect(&_mouseTimer, &QTimer::timeout, this, &Scene::onPositionCursor);
}

void Scene::setPosition(const QMap <QString, qreal> position)
{
    _position = position;
    setPos(position["x"], position["y"]);
}

QMap <QString, qreal> Scene::getPosition()
{
    return _position;
}

void Scene::onPositionCursor()
{
    WorkJson::Instance().toJsonClick(_cursor);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    _mouseTimer.start(100);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    _mouseTimer.stop();
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QMap <QString, qreal> cursor;
    cursor.insert("x", mouseEvent->scenePos().x());
    cursor.insert("y", mouseEvent->scenePos().y());

    _cursor = cursor;
}

