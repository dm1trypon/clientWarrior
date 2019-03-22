#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QMap>

class Camera : public QObject {
    Q_OBJECT
public:
    Camera();

    void setOffsetFactor(const QMap<QString, qreal> position, const QRectF sceneProp);
    void setSizePlayer(const QMap<QString, qreal> sizePlayer);

    QMap<QString, qreal> setPositionObjects(const QMap<QString, qreal> position);
    QMap<QString, qreal> setShot(const QMap<QString, qreal> click);

private:
    QMap<QString, qreal> _factor;
    QMap<QString, qreal> _sizePlayer;
};

#endif // CAMERA_H
