#include "hud.h"

#include <QBrush>
#include <QPen>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QFont>
#include <QRectF>
#include <QPointF>

HUD::HUD(QPointF position, const QRectF sceneProp) :
    QObject(), QGraphicsPixmapItem (nullptr)
{
    setPixmap(QPixmap(PATH_TO_HUD_IMG).scaled(static_cast<int>(sceneProp.width()), static_cast<int>(sceneProp.height())));
    setPos(position.rx(), position.ry());
    setZValue(5);
}

void HUD::addHealth(const int health, const QMap <QString, qreal> position)
{
    QFont _font;
    _font.setPointSize(position["size"]);
    _healthHud = new QGraphicsTextItem(QString::number(health), this);
    _healthHud->setPos(position["x"], position["y"]);
    _healthHud->setFont(_font);
    _healthHud->setZValue(5);
    _healthHud->setDefaultTextColor(Qt::yellow);
}

QGraphicsTextItem * HUD::getHealth()
{
    return _healthHud;
}

void HUD::addScore(const int score, const QMap <QString, qreal> position)
{
    QFont _font;
    _font.setPointSize(position["size"]);
    _scoreHud = new QGraphicsTextItem(QString::number(score), this);
    _scoreHud->setPos(position["x"], position["y"]);
    _scoreHud->setFont(_font);
    _scoreHud->setZValue(5);
    _scoreHud->setDefaultTextColor(Qt::green);
}

QGraphicsTextItem * HUD::getScore()
{
    return _scoreHud;
}
