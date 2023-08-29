#ifndef HQCCWEBSOCKETCLIENT_H
#define HQCCWEBSOCKETCLIENT_H

#include <QObject>
#include <QWebSocket>
#include <QTimer>
#include <QDebug>

class HQCCWebSocketClient : public QObject
{
    Q_OBJECT
public:
    explicit HQCCWebSocketClient(const QString& name=QString(),QObject *parent = nullptr);
    ~HQCCWebSocketClient();
signals:
    void signalTextMessageReceived(const QString& message);
    void signalStateChanged(QAbstractSocket::SocketState state);
public slots:
    void slotConnectToWebSocketServer(const QUrl& url);
    void slotSendTextMessage(const QString& message);
private:
    QUrl m_WebUrl;
    QWebSocket* p_WebSocket = nullptr;
    QTimer* p_HeartbeatTimer = nullptr;
    int m_Heartbeat;
    QTimer* p_TimeoutTimer = nullptr;
    int m_Timeout;
    QString m_ClientName;
};

#endif // HQCCWEBSOCKETCLIENT_H
