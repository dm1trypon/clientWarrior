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
    QMap <QString, qreal> setPositionObjects(const QMap <QString, qreal> position, const QMap <QString, qreal> sizePlayer);
private:
    QMap <QString, qreal> _factor;
};

#endif // CAMERA_H
