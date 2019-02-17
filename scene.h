#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>

class Scene : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Scene(const QMap <QString, qreal> position, const QMap <QString, qreal> size);
    void setPosition(const QMap <QString, qreal> position);

private:
    const QString PATH_TO_SCENE_IMG = ":/img/scene.jpg";
};

#endif // SCENE_H
