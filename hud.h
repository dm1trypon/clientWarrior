#ifndef BAR_H
#define BAR_H

#include <QObject>
#include <QGraphicsPixmapItem>

class HUD : public QObject, public QGraphicsPixmapItem
{
public:
    HUD(const QMap <QString, qreal> position, const QMap <QString, qreal> size);

    void addScore(const int score, const QMap<QString, qreal> position);
    void addHealth(const int health, const QMap <QString, qreal> position);

    QGraphicsTextItem *getHealth();
    QGraphicsTextItem *getScore();

private:
    const QString PATH_TO_HUD_IMG = ":/img/hud.png";

    QGraphicsTextItem *_scoreHud;
    QGraphicsTextItem *_healthHud;
    QMap <QString, qreal> _size;
};

#endif // BAR_H
