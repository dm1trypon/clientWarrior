#include "client.h"
#include "workjson.h"
#include <QtCore/QDebug>

QT_USE_NAMESPACE

Client::Client(const QUrl &url, QObject *parent) :
    QObject(parent),
    m_url(url)
{
    qDebug() << "Attempt connect to server:" << m_url.url();
    connect(&m_webSocket, &QWebSocket::connected, this, &Client::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &Client::closed);
    connect(&WorkJson::Instance(), &WorkJson::signalSend, this, &Client::onSend);
    m_webSocket.open(QUrl(url));
}

void Client::onConnected()
{
    qDebug() << "WebSocket connected.";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &Client::onTextMessageReceived);
}

void Client::onSend(QString data)
{
    m_webSocket.sendTextMessage(data);
}

void Client::onTextMessageReceived(QString data)
{
//    qDebug().noquote() << "Message received:" << data;
    WorkJson::Instance().fromJson(data);
}

