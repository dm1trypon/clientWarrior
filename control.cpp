#include "control.h"
#include "workjson.h"

#include <QDebug>
#include <QApplication>

Control::Control()
{
    qDebug() << "Control has been created...";
    keys();
}

void Control::onHold(QKeyEvent* event, const bool hold)
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

    case Qt::Key_1:
        WorkJson::Instance().getMagic()->setWeapon("blaster");
        break;

    case Qt::Key_2:
        WorkJson::Instance().getMagic()->setWeapon("plazma");
        break;

    case Qt::Key_3:
        WorkJson::Instance().getMagic()->setWeapon("machine_gun");
        break;

    case Qt::Key_Escape:
        qApp->exit();
        break;

    default:
        break;
    }

    if (hold) {
        if (_keyList.count() == 2) {
            _keyList.first() = _keyList.last();
            _keyList.removeLast();
        }

        _keyList.append(key);
    }

    WorkJson::Instance().toJsonKey(_keys[_keyList], hold);

    if (!hold) {
        _keyList.removeOne(key);

        if (!_keyList.isEmpty()) {
            WorkJson::Instance().toJsonKey(_keyList.first(), true);
        }
    }
}

void Control::keys()
{
    _keys.insert(UP, "up");
    _keys.insert(DOWN, "down");
    _keys.insert(LEFT, "left");
    _keys.insert(RIGHT, "right");
    _keys.insert(LEFT_DOWN, "left_down");
    _keys.insert(LEFT_UP, "left_up");
    _keys.insert(RIGHT_DOWN, "right_down");
    _keys.insert(RIGHT_UP, "right_up");
    _keys.insert(DOWN_LEFT, "left_down");
    _keys.insert(DOWN_RIGHT, "right_down");
    _keys.insert(UP_LEFT, "left_up");
    _keys.insert(UP_RIGHT, "right_up");
}
