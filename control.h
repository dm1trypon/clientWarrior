#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <QKeyEvent>

class Control : public QObject {
    Q_OBJECT
public:
    Control();
    void onHold(QKeyEvent* event, const bool hold);

private:
    const QStringList LEFT = { "left" };
    const QStringList RIGHT = { "right" };
    const QStringList DOWN = { "down" };
    const QStringList UP = { "up" };

    const QStringList UP_LEFT = { "up", "left" };
    const QStringList UP_RIGHT = { "up", "right" };
    const QStringList DOWN_LEFT = { "down", "left" };
    const QStringList DOWN_RIGHT = { "down", "right" };

    const QStringList LEFT_UP = { "left", "up" };
    const QStringList RIGHT_UP = { "right", "up" };
    const QStringList LEFT_DOWN = { "left", "down" };
    const QStringList RIGHT_DOWN = { "right", "down" };

    QStringList _keyList;
    QMap<QStringList, QString> _keys;

    void keys();
};

#endif // CONTROL_H
