#ifndef HQCCMAINWINDOW_H
#define HQCCMAINWINDOW_H

#include <QMainWindow>

#include <QThread>
#include "HQCWebSocketClient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class HQCCMainWindow; }
QT_END_NAMESPACE

class HQCCMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    HQCCMainWindow(QWidget *parent = nullptr);
    ~HQCCMainWindow();

private:
    Ui::HQCCMainWindow *ui;
signals:
    void signalConnectToWebSocketServer(const QUrl& url);
    void signalSendTextMessage(const QString& message);
private slots:
    void slotTextMessageReceived(const QString& message);
    void on_btnConnectServer_clicked();
    void on_btnSendMsg_clicked();
private:
    HQCCWebSocketClient *p_WebClient = nullptr;
    QThread *p_WebClientThread = nullptr;
};
#endif // HQCCMAINWINDOW_H
