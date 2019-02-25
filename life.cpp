#include "life.h"
#include "QDebug"

Life::Life(const QMap <QString, qreal> position, const QMap <QString, qreal> size) :
    QObject(), QGraphicsPixmapItem (nullptr)
{
    setPixmap(QPixmap(PATH_TO_LIFE_IMG).scaled(static_cast<int>(size["width"]), static_cast<int>(size["height"])));
    setPos(position["x"], position["y"]);
    qDebug() << "Life has been created (" << position["x"] << ":" << position["y"] << ";" << size["width"] << "X" << size["height"] << ")";
}
