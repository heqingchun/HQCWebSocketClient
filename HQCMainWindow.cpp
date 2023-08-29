#include "HQCMainWindow.h"
#include "ui_HQCMainWindow.h"

HQCCMainWindow::HQCCMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HQCCMainWindow)
{
    ui->setupUi(this);
}

HQCCMainWindow::~HQCCMainWindow(){
    if(p_WebClientThread){
        p_WebClientThread->quit();
        p_WebClientThread->wait();
    }
    if(p_WebClient){
        p_WebClient->deleteLater();
    }
    delete ui;
}
void HQCCMainWindow::slotTextMessageReceived(const QString& message){
    ui->textEditMsg->append(message);
}

void HQCCMainWindow::on_btnConnectServer_clicked(){
    p_WebClient = new HQCCWebSocketClient(ui->lineEditName->text());
    p_WebClientThread = new QThread();
    p_WebClient->moveToThread(p_WebClientThread);
    connect(this,&HQCCMainWindow::signalConnectToWebSocketServer,p_WebClient,&HQCCWebSocketClient::slotConnectToWebSocketServer);
    connect(this,&HQCCMainWindow::signalSendTextMessage,p_WebClient,&HQCCWebSocketClient::slotSendTextMessage);
    connect(p_WebClient,&HQCCWebSocketClient::signalTextMessageReceived,this,&HQCCMainWindow::slotTextMessageReceived);
    connect(p_WebClientThread,&QThread::finished,p_WebClientThread,&QThread::deleteLater);
    p_WebClientThread->start();
    emit signalConnectToWebSocketServer(QUrl(QString("ws://%1:%2").arg(ui->lineEditIp->text()).arg(ui->lineEditPort->text())));
}

void HQCCMainWindow::on_btnSendMsg_clicked(){
    emit signalSendTextMessage(ui->lineEditMsg->text());
}
