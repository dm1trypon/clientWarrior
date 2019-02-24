#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QMap>

class Camera : public QObject
{
    Q_OBJECT
public:
    Camera();

    void setOffsetFactor(const QMap <QString, qreal> position, const QMap <QString, qreal> viewCenter);
    void setSizePlayer(const QMap <QString, qreal> sizePlayer);
    QMap <QString, qreal> setPositionObjects(const QMap <QString, qreal> position);
    QMap <QString, qreal> setClick(const QMap <QString, qreal> click, const QMap <QString, qreal> positionScene);
private:
    QMap <QString, qreal> _factor;
    QMap <QString, qreal> _sizePlayer;
};

#endif // CAMERA_H
