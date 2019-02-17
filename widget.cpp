#include "widget.h"
#include "workjson.h"
#include "client.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    createElements();
    connect(&WorkJson::Instance(), &WorkJson::signalConnected, this, &Widget::onConnected);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::createElements()
{
    _scene = new QGraphicsScene(0, 0, 1920, 1080, this);
    _mainLayout = new QVBoxLayout;

    _view = new QGraphicsView(this);
    _view->hide();
    _view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    _labelNickname = new QLabel("Nickname:");
    _labelHost = new QLabel("Host:");
    _labelPort = new QLabel("Port:");

    _lineEditNickname = new QLineEdit();
    _lineEditHost = new QLineEdit();
    _lineEditPort = new QLineEdit();

    _buttonConnect = new QPushButton("Connect");
    connect(_buttonConnect, &QPushButton::clicked, this, &Widget::connectToServer);

    _mainLayout->addWidget(_labelNickname);
    _mainLayout->addWidget(_lineEditNickname);
    _mainLayout->addWidget(_labelHost);
    _mainLayout->addWidget(_lineEditHost);
    _mainLayout->addWidget(_labelPort);
    _mainLayout->addWidget(_lineEditPort);
    _mainLayout->addWidget(_buttonConnect);
    _mainLayout->addWidget(_view);

    _view->setScene(_scene);
    setLayout(_mainLayout);
    WorkJson::Instance().setScene(_scene);
}

void Widget::resizeEvent(QResizeEvent *)
{
    qDebug() << "Widht:" << _view->width() << "Height:" << _view->height();
    setViewCenter();
}

void Widget::setViewCenter()
{
    QMap <QString, qreal> viewCenter;
    viewCenter.insert("x", _view->width() / 2);
    viewCenter.insert("y", _view->height() / 2);

    WorkJson::Instance().setViewCenter(viewCenter);
}

void Widget::onConnected()
{
    _view->show();
    _labelNickname->hide();
    _labelHost->hide();
    _labelPort->hide();
    _lineEditHost->hide();
    _lineEditNickname->hide();
    _lineEditPort->hide();
    _buttonConnect->hide();
    showFullScreen();
    setViewCenter();
    _mainLayout->setMargin(0);
}

void Widget::connectToServer()
{
    _host = _lineEditHost->text();
    _port = _lineEditPort->text();
    _nickname = _lineEditNickname->text();
    WorkJson::Instance().setNickname(_nickname);
    new Client(QUrl("ws://" + _host + ":" + _port));
}
