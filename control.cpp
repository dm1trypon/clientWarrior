#include "control.h"
#include "workjson.h"

#include <QDebug>
#include <QApplication>

Control::Control()
{
    qDebug() << "Control has been created...";
}

void Control::onHold(QKeyEvent *event, const bool hold)
{
    QString key;

    switch (event->key()) {
    case Qt::Key_A:
        key = "left";
        break;
    case Qt::Key_D:
        key = "right";
        break;
    case Qt::Key_W:
        key = "up";
        break;
    case Qt::Key_S:
        key = "down";
        break;
    case Qt::Key_Escape:
        qApp->exit();
        break;
    default:
        break;
    }

    if (hold)
    {
        _keyList.append(key);
    }

    if (_keyList == UP)
    {
        WorkJson::Instance().toJsonKey("up", hold);
    }

    if (_keyList == DOWN)
    {
        WorkJson::Instance().toJsonKey("down", hold);
    }

    if (_keyList == LEFT)
    {
        WorkJson::Instance().toJsonKey("left", hold);
    }

    if (_keyList == RIGHT)
    {
        WorkJson::Instance().toJsonKey("right", hold);
    }

    if (_keyList == UP_LEFT || _keyList == LEFT_UP)
    {
        WorkJson::Instance().toJsonKey("left_up", hold);
    }

    if (_keyList == UP_RIGHT || _keyList == RIGHT_UP)
    {
        WorkJson::Instance().toJsonKey("right_up", hold);
    }

    if (_keyList == DOWN_LEFT || _keyList == LEFT_DOWN)
    {
        WorkJson::Instance().toJsonKey("left_down", hold);
    }

    if (_keyList == DOWN_RIGHT || _keyList == RIGHT_DOWN)
    {
        WorkJson::Instance().toJsonKey("right_down", hold);
    }

    if (!hold)
    {
        _keyList.removeOne(key);

        if (!_keyList.isEmpty())
        {
            WorkJson::Instance().toJsonKey(_keyList.first(), true);
        }
    }
}
