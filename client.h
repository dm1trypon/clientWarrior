#ifndef CLIENT_H
#define CLIENT_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(const QUrl &url, QObject *parent = nullptr);

signals:
    void closed();

private slots:
    void onConnected();
    void onTextMessageReceived(const QString &message);
    void onSend(const QString &data);

private:
    QWebSocket _webSocket;
    QUrl _url;
};


#endif // CLIENT_H
