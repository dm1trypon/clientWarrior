#include "widget.h"
#include "workjson.h"
#include "client.h"
#include "ui_widget.h"

#include <QDesktopWidget>
#include <QGLWidget>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    createElements();
    connect(&WorkJson::Instance(), &WorkJson::signalConnected, this, &Widget::onConnected);
    setAttribute(Qt::WA_KeyCompression);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setResolution(const int id)
{
    if (!_resolution.isEmpty())
    {
        _resolution.clear();
    }

    QString value = _resolutionBox->itemData(id).toString();
    QStringList resolution = value.split(":");
    _resolution.insert("width", resolution[0].toDouble());
    _resolution.insert("height", resolution[1].toDouble());

    WorkJson::Instance().setResolution(_resolution);
}

void Widget::createElements()
{
    _view = new QGraphicsView(this);
    _view->viewport()->installEventFilter(this);
    _view->setViewport(new QGLWidget);
    _view->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
    _view->hide();

    WorkJson::Instance().setView(_view);

    _mainLayout = new QVBoxLayout;

    _labelNickname = new QLabel("Nickname:");
    _labelHost = new QLabel("Host:");
    _labelPort = new QLabel("Port:");
    _labelResolution = new QLabel("Resolution:");

    _resolutionBox = new QComboBox();

    resolutionInit();
    setResolutionDefault();

    connect(_resolutionBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::setResolution);

    _lineEditNickname = new QLineEdit();
    _lineEditHost = new QLineEdit();
    _lineEditHost->setText("127.0.0.1");
    _lineEditPort = new QLineEdit();
    _lineEditPort->setText("44444");

    _buttonConnect = new QPushButton("Connect");
    connect(_buttonConnect, &QPushButton::clicked, this, &Widget::connectToServer);

    _buttonTypeWindow = new QPushButton("Fullscreen");
    connect(_buttonTypeWindow, &QPushButton::clicked, this, &Widget::typeWindow);

    _mainLayout->addWidget(_labelNickname);
    _mainLayout->addWidget(_lineEditNickname);
    _mainLayout->addWidget(_labelHost);
    _mainLayout->addWidget(_lineEditHost);
    _mainLayout->addWidget(_labelPort);
    _mainLayout->addWidget(_lineEditPort);
    _mainLayout->addWidget(_labelResolution);
    _mainLayout->addWidget(_resolutionBox);
    _mainLayout->addWidget(_buttonTypeWindow);
    _mainLayout->addWidget(_buttonConnect);
    _mainLayout->addWidget(_view);

    setLayout(_mainLayout);
}

void Widget::setResolutionDefault()
{
    _resolution.insert("width", 640);
    _resolution.insert("height", 480);

    WorkJson::Instance().setResolution(_resolution);
}

void Widget::resolutionInit()
{
    _resolutionBox->addItem("640 x 480", QVariant("640:480"));
    _resolutionBox->addItem("800 x 600", QVariant("800:600"));
    _resolutionBox->addItem("1024 x 600", QVariant("1024:600"));
    _resolutionBox->addItem("1024 x 768", QVariant("1024:768"));
    _resolutionBox->addItem("1280 x 720", QVariant("1280:720"));
    _resolutionBox->addItem("1280 x 1024", QVariant("1280:1024"));
    _resolutionBox->addItem("1600 x 900", QVariant("1600:900"));
    _resolutionBox->addItem("1920 x 1080", QVariant("1920:1080"));
}

bool Widget::eventFilter(QObject *target, QEvent *event)
{
    if (!_scene)
    {
        return false;
    }

    if (target == _scene)
    {
        if (event->type() == QEvent::GraphicsSceneMouseMove)
        {
            const QGraphicsSceneMouseEvent* const cursor = static_cast<const QGraphicsSceneMouseEvent*>(event);

            WorkJson::Instance().toJsonCursor(cursor->scenePos());
        }
    }

    return QWidget::eventFilter(target, event);
}

void Widget::onConnected()
{
    setFixedSize(static_cast<int>(_resolution["width"]),
            static_cast<int>(_resolution["height"]));

    createScene();
    _scene->installEventFilter(this);

    QMap <QString, qreal> sceneCenter;
    sceneCenter.insert("x", _scene->width() / 2);
    sceneCenter.insert("y", _scene->height() / 2);

    WorkJson::Instance().setViewCenter(sceneCenter);

    _scene->setStickyFocus(true);
    _view->setScene(_scene);

    WorkJson::Instance().setScene(_scene);

    _view->show();
    _view->setSceneRect(_scene->sceneRect());
    _labelNickname->hide();
    _labelHost->hide();
    _labelPort->hide();
    _lineEditHost->hide();
    _lineEditNickname->hide();
    _lineEditPort->hide();
    _labelResolution->hide();
    _resolutionBox->hide();
    _buttonConnect->hide();
    _buttonTypeWindow->hide();
    _mainLayout->setMargin(0);
}

void Widget::createScene()
{
    if (_fullscreen)
    {
        _scene = new QGraphicsScene(0, 0,
                                    QApplication::desktop()->screenGeometry().width() - 2,
                                    QApplication::desktop()->screenGeometry().height() - 2,
                                    this);

        showFullScreen();
        return;
    }

    _scene = new QGraphicsScene(0, 0,
                                _resolution["width"] - 2,
                                _resolution["height"] - 2,
                                this);
}

void Widget::typeWindow()
{
    if (_fullscreen)
    {
        _fullscreen = false;
        _buttonTypeWindow->setText("Fullscreen");
        return;
    }

    _fullscreen = true;
    _buttonTypeWindow->setText("Window");
}

void Widget::connectToServer()
{
    _host = _lineEditHost->text();
    _port = _lineEditPort->text();
    _nickname = _lineEditNickname->text();
    WorkJson::Instance().setNickname(_nickname);
    new Client(QUrl("ws://" + _host + ":" + _port));
}
