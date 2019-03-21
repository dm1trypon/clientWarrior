#ifndef BAR_H
#define BAR_H

#include <QObject>
#include <QGraphicsPixmapItem>

class HUD : public QObject, public QGraphicsPixmapItem
{
public:
    HUD(QPointF position, const QRectF sceneProp);

    void addScore(const int score, const QMap<QString, qreal> position);
    void addHealth(const int health, const QMap <QString, qreal> position);

    QGraphicsTextItem *getHealth();
    QGraphicsTextItem *getScore();

private:
    const QString PATH_TO_HUD_IMG = ":/img/hud.png";

    QGraphicsTextItem *_scoreHud;
    QGraphicsTextItem *_healthHud;
};

#endif // BAR_H
