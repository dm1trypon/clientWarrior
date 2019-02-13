#include "widget.h"
#include "workjson.h"
#include "player.h"
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
    _mainLayout = new QVBoxLayout;
    _view = new QGraphicsView;
    _view->hide();
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
    setLayout(_mainLayout);
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

    Player *player = new Player(0, 0);
    _scene->addItem(player);
//    player->setPos(posX, posY);
}

void Widget::connectToServer()
{
    _host = _lineEditHost->text();
    _port = _lineEditPort->text();
    _nickname = _lineEditNickname->text();
    WorkJson::Instance().setNickname(_nickname);
    _client = new Client(QUrl("ws://" + _host + ":" + _port));
}
