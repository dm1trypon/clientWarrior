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

void Camera::setSizePlayer(const QMap<QString, qreal> sizePlayer)
{
    _sizePlayer = sizePlayer;
}

QMap <QString, qreal> Camera::setPositionObjects(const QMap <QString, qreal> position)
{
    QMap <QString, qreal> newPosition;
    newPosition.insert("x", position["x"] - _factor["xFactor"] - _sizePlayer["width"] / 2);
    newPosition.insert("y", position["y"] - _factor["yFactor"] - _sizePlayer["height"] / 2);

    return newPosition;
}

QMap <QString, qreal> Camera::setClick(const QMap <QString, qreal> click, const QMap <QString, qreal> positionScene)
{
    QMap <QString, qreal> newClick;
    newClick.insert("x", click["x"] + _factor["xFactor"] + _sizePlayer["width"] / 2);
    newClick.insert("y", click["y"] + _factor["yFactor"] + _sizePlayer["height"] / 2);
    return newClick;
}
