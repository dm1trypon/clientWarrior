#include "camera.h"

#include <QDebug>

Camera::Camera()
{
    qDebug() << "Camera has been created!";
}

void Camera::setOffsetFactor(const QMap <QString, qreal> position, const QMap <QString, qreal> viewCenter)
{
    if (!_factor.isEmpty())
    {
        _factor.clear();
    }

    _factor.insert("xFactor", position["x"] - viewCenter["x"]);
    _factor.insert("yFactor", position["y"] - viewCenter["y"]);
}

QMap <QString, qreal> Camera::setPositionObjects(const QMap <QString, qreal> position, const QMap <QString, qreal> sizePlayer)
{
    QMap <QString, qreal> newPosition;
    newPosition.insert("x", position["x"] - _factor["xFactor"] - sizePlayer["width"] / 2);
    newPosition.insert("y", position["y"] - _factor["yFactor"] - sizePlayer["width"] / 2);
    return newPosition;
}
