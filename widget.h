#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLineEdit>
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

private:
    Ui::Widget *ui;
    QGraphicsScene *_scene;
    QGraphicsView *_view;
    QVBoxLayout *_mainLayout;
    QPushButton *_buttonConnect;
    QLineEdit *_lineEditNickname;
    QLineEdit *_lineEditHost;
    QLineEdit *_lineEditPort;
    QLabel *_labelNickname;
    QLabel *_labelHost;
    QLabel *_labelPort;
    QString _host;
    QString _port;
    QString _nickname;

    void createElements();

    void setViewCenter();
protected:
    void resizeEvent(QResizeEvent *);
};

#endif // WIDGET_H
