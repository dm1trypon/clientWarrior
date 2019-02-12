#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    createElements();
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

void Widget::connectToServer()
{

    _host = _lineEditHost->text();
    _port = _lineEditPort->text();
    _client = new Client(QUrl(_host + ":" + _port));
}
