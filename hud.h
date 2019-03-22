#ifndef BAR_H
#define BAR_H

#include <QObject>
#include <QGraphicsPixmapItem>

class HUD : public QObject, public QGraphicsPixmapItem {
public:
    HUD(QPointF position, const QRectF sceneProp);

    void addTextElement(const int health, const QString &id, const QMap<QString, qreal> prop, QColor color);

    QGraphicsTextItem* getTextElement(const QString &id);
private:
    const QString PATH_TO_HUD_IMG = ":/img/hud.png";

    QMap<QString, QGraphicsTextItem*> _hudTexts;
};

#endif // BAR_H
