#include "scene.h"

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
}

void Scene::setPosition(const QMap <QString, qreal> position)
{
    setPos(position["x"], position["y"]);
}
