#include "client.h"
#include "workjson.h"
#include <QtCore/QDebug>

QT_USE_NAMESPACE

Client::Client(const QUrl& url, QObject* parent)
    : QObject(parent)
    , _url(url)
{
    qDebug().noquote() << "Attempt connect to server:" << _url.url();
    connect(&_webSocket, &QWebSocket::connected, this, &Client::onConnected);
    connect(&_webSocket, &QWebSocket::disconnected, this, &Client::closed);

    connect(&_webSocket,
        static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),
        [=](QAbstractSocket::SocketError) {
            qWarning().noquote() << "Host:" << _url.toDisplayString()
                                 << ". Error:" << _webSocket.errorString();

            deleteLater();

            return;
        });

    connect(&WorkJson::Instance(), &WorkJson::signalSend, this, &Client::onSend);
    _webSocket.open(QUrl(url));
}

void Client::onConnected()
{
    qDebug().noquote() << "Сonnected to " + _url.toDisplayString();
    connect(&_webSocket, &QWebSocket::textMessageReceived,
        this, &Client::onTextMessageReceived);
}

void Client::onSend(const QString& data)
{
    _webSocket.sendTextMessage(data);
}

void Client::onTextMessageReceived(const QString& data)
{
    WorkJson::Instance().fromJson(data);
}
