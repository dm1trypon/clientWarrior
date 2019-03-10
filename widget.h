#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void connectToServer();
    void onConnected();
    void typeWindow();

    void setResolution(const int id);
private:
    Ui::Widget *ui;
    QGraphicsScene *_scene;
    QGraphicsView *_view;
    QVBoxLayout *_mainLayout;
    QPushButton *_buttonConnect;
    QPushButton *_buttonTypeWindow;
    QLineEdit *_lineEditNickname;
    QLineEdit *_lineEditHost;
    QLineEdit *_lineEditPort;
    QComboBox *_listFall;
    QLabel *_labelNickname;
    QLabel *_labelHost;
    QLabel *_labelPort;
    QLabel *_labelResolution;
    QString _host;
    QString _port;
    QString _nickname;
    QMap <QString, qreal> _resolution;
    bool _fullscreen = false;

    void createElements();

    void createScene();
};

#endif // WIDGET_H
