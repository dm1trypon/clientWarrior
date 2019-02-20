#include "client.h"
#include "workjson.h"
#include <QtCore/QDebug>

QT_USE_NAMESPACE

Client::Client(const QUrl &url, QObject *parent) :
    QObject(parent),
    m_url(url)
{
    qDebug().noquote() << "Attempt connect to server:" << m_url.url();
    connect(&m_webSocket, &QWebSocket::connected, this, &Client::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &Client::closed);

    connect(&m_webSocket, static_cast<void(QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),
         [=](QAbstractSocket::SocketError)
    {
        qWarning().noquote() << "Host:" << m_url.toDisplayString() << ". Error:" << m_webSocket.errorString();
        deleteLater();
        return;
    });

    connect(&WorkJson::Instance(), &WorkJson::signalSend, this, &Client::onSend);
    m_webSocket.open(QUrl(url));
}

void Client::onConnected()
{
    qDebug().noquote() << "Сonnected to " + m_url.toDisplayString();
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &Client::onTextMessageReceived);
}

void Client::onSend(const QString &data)
{
    m_webSocket.sendTextMessage(data);
}

void Client::onTextMessageReceived(const QString &data)
{
    WorkJson::Instance().fromJson(data);
}

