#include "hud.h"

#include <QBrush>
#include <QPen>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QFont>

HUD::HUD(const QMap<QString, qreal> position, const QMap<QString, qreal> size) :
    QObject(), QGraphicsPixmapItem (nullptr),
    _size(size)
{
    setPixmap(QPixmap(PATH_TO_HUD_IMG).scaled(static_cast<int>(size["width"]), static_cast<int>(size["height"])));
    setPos(position["x"], position["y"]);
    setZValue(9);
}

void HUD::addHealth(const int health)
{
    QFont _font;
    _font.setPointSize(30);
    _healthHud  = new QGraphicsTextItem(QString::number(health), this);
    _healthHud->setPos(170, _size["height"] - 60);
    _healthHud->setFont(_font);
    _healthHud->setZValue(10);
    _healthHud->setDefaultTextColor(Qt::yellow);
}

QGraphicsTextItem * HUD::getHealth()
{
    return _healthHud;
}

QGraphicsTextItem * HUD::addScore(const int score, const QMap <QString, qreal> position)
{
    QFont _font;
    _font.setPointSize(30);
    QGraphicsTextItem* scoreBar  = new QGraphicsTextItem(QString::number(score), this);
    scoreBar->setFont(_font);
    scoreBar->setPos(_size["width"] - 150, _size["height"] - 60);
    scoreBar->setZValue(9);
    scoreBar->setDefaultTextColor(Qt::red);
    return scoreBar;
}
