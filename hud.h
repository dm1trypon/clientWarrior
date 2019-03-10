#ifndef BAR_H
#define BAR_H

#include <QObject>
#include <QGraphicsPixmapItem>

class HUD : public QObject, public QGraphicsPixmapItem
{
public:
    HUD(const QMap <QString, qreal> position, const QMap <QString, qreal> size);

    QGraphicsTextItem *addScore(const int score, const QMap<QString, qreal> position);
    void addHealth(const int health);
    QGraphicsTextItem *getHealth();
private:
    const QString PATH_TO_HUD_IMG = ":/img/hud.png";
    QGraphicsRectItem *_scoreBar;
    QGraphicsTextItem *_healthHud;
    QMap<QString, qreal> _size;
};

#endif // BAR_H
