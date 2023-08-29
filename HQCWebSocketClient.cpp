#include "HQCWebSocketClient.h"

HQCCWebSocketClient::HQCCWebSocketClient(const QString& name,QObject *parent) : QObject(parent){
    if(name.isEmpty()){
        qDebug()<<__FUNCTION__<<"请为当前WebSocketClient命名";
    }
    m_Heartbeat = 1000;
    m_Timeout = 5000;
    m_ClientName = name;
    p_WebSocket = new QWebSocket(m_ClientName,QWebSocketProtocol::VersionLatest,this);
    connect(p_WebSocket,&QWebSocket::connected,[&](){
        qDebug()<<"已连接Web服务端";
        connect(p_WebSocket,&QWebSocket::pong,[&](){
            p_TimeoutTimer->start(m_Timeout);
        });
        connect(p_WebSocket,&QWebSocket::textMessageReceived,this,&HQCCWebSocketClient::signalTextMessageReceived);
        connect(p_WebSocket,&QWebSocket::stateChanged,this,&HQCCWebSocketClient::signalStateChanged);
        p_WebSocket->sendTextMessage(m_ClientName);
        p_HeartbeatTimer->start(m_Heartbeat);
    });
    p_HeartbeatTimer = new QTimer(this);
    connect(p_HeartbeatTimer,&QTimer::timeout,[&](){
        p_WebSocket->ping(m_ClientName.toUtf8());
    });
    p_TimeoutTimer = new QTimer(this);
    connect(p_TimeoutTimer,&QTimer::timeout,[&](){
        qDebug()<<"ping服务端超时，重新连接Web服务端";
        p_HeartbeatTimer->stop();
        if(p_WebSocket->state() == QAbstractSocket::ConnectedState){
            p_WebSocket->close();
        }
        p_WebSocket->deleteLater();
        p_WebSocket = new QWebSocket(m_ClientName,QWebSocketProtocol::VersionLatest,this);
        connect(p_WebSocket,&QWebSocket::connected,[&](){
            qDebug()<<"已连接Web服务端";
            connect(p_WebSocket,&QWebSocket::pong,[&](){
                p_TimeoutTimer->start(m_Timeout);
            });
            connect(p_WebSocket,&QWebSocket::textMessageReceived,this,&HQCCWebSocketClient::signalTextMessageReceived);
            connect(p_WebSocket,&QWebSocket::stateChanged,this,&HQCCWebSocketClient::signalStateChanged);
            p_WebSocket->sendTextMessage(m_ClientName);
            p_HeartbeatTimer->start(m_Heartbeat);
        });
        p_WebSocket->open(m_WebUrl);
    });
}
HQCCWebSocketClient::~HQCCWebSocketClient(){
    if(p_WebSocket){
        p_WebSocket->deleteLater();
    }
    if(p_HeartbeatTimer){
        p_HeartbeatTimer->deleteLater();
    }
    if(p_TimeoutTimer){
        p_TimeoutTimer->deleteLater();
    }
}
void HQCCWebSocketClient::slotConnectToWebSocketServer(const QUrl& url){
    m_WebUrl = url;
    p_WebSocket->open(m_WebUrl);
    p_TimeoutTimer->start(m_Timeout);
}
void HQCCWebSocketClient::slotSendTextMessage(const QString& message){
    p_WebSocket->sendTextMessage(message);
}
