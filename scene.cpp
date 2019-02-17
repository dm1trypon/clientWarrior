#include "scene.h"

#include <QDebug>

Scene::Scene(const QMap<QString, qreal> position, const QMap<QString, qreal> size) :
    QObject(), QGraphicsPixmapItem (nullptr)
{
    setPixmap(QPixmap(PATH_TO_SCENE_IMG).scaled(static_cast<int>(size["width"]), static_cast<int>(size["height"])));
    setPos(position["x"], position["y"]);
    qDebug() << "Scene has been created (" << position["x"] << ":" << position["y"] << ")";
    qDebug() << "Scene size: " << size["width"] << ":" << size["height"];
}

void Scene::setPosition(const QMap<QString, qreal> position)
{
    setPos(position["x"], position["y"]);
}
