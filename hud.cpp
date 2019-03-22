#include "hud.h"

#include <QBrush>
#include <QPen>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QFont>
#include <QRectF>
#include <QPointF>

HUD::HUD(QPointF position, const QRectF sceneProp)
    : QObject()
    , QGraphicsPixmapItem(nullptr)
{
    setPixmap(QPixmap(PATH_TO_HUD_IMG).scaled(static_cast<int>(sceneProp.width()), static_cast<int>(sceneProp.height())));
    setPos(position.rx(), position.ry());
    setZValue(5);
}

void HUD::addTextElement(const int health, const QString &id, const QMap<QString, qreal> prop, QColor color)
{
    if (_hudTexts.contains(id)) {
        qWarning() << "Creation element failed! Item with this name is exist!";
        return;
    }

    QFont font;
    font.setPointSize(static_cast<int>(prop["size"]));
    QGraphicsTextItem *tElement = new QGraphicsTextItem(QString::number(health), this);
    tElement->setPos(prop["x"], prop["y"]);
    tElement->setFont(font);
    tElement->setZValue(5);
    tElement->setDefaultTextColor(color);
    _hudTexts.insert(id, tElement);
}

QGraphicsTextItem *HUD::getTextElement(const QString &id)
{
    if (!_hudTexts[id]) {
        return nullptr;
    }

    return _hudTexts[id];
}
