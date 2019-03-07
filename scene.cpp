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
    setPen(QPen(Qt::yellow));

    qDebug() << "Scene has been created (" << position["x"] << ":" << position["y"] << ")";
    qDebug() << "Scene size: " << size["width"] << ":" << size["height"];

    connect(&_mouseTimer, &QTimer::timeout, this, &Scene::onPositionCursor);
}

QGraphicsRectItem * Scene::addScoreBar(const QMap <QString, qreal> position)
{
    qDebug() << position;
    _scoreBar = new QGraphicsRectItem(nullptr);
    _scoreBar->setRect(position["x"], position["y"], 200, 100);
    _scoreBar->setBrush(QBrush(Qt::black));
    _scoreBar->setPen(QPen(Qt::yellow));
    return _scoreBar;
}

void Scene::setPositionScoreBar(const QMap <QString, qreal> position)
{
    _scoreBar->setPos(position["x"], position["y"]);
}

QGraphicsRectItem * Scene::getScoreBar()
{
    return _scoreBar;
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

