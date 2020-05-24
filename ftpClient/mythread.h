#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <winsock2.h>
#include <windows.h>
#include <QMainWindow>
#include "message.h"
#include <thread>
namespace Ui {
class MainWindow;
}
class myThread : public QObject
{
    Q_OBJECT
public:
    explicit myThread(QObject *parent = nullptr);
    void initSocket();
    void dealReadfd();

signals:
     void showFileInfo(char*, short,long long,char*);
public slots:
     void sendNextDir(QString);
     //void runsend();

public:
     bool issend;//是否发送消息
     cdNext tempCdNext;//下一层目录消息
     friend void runsend(myThread *);
private:

    SOCKET sockClient;
    SOCKADDR_IN addrClient;
    SOCKADDR_IN addrServer;
    WSADATA wsaData;


    std::thread sendThread;
};

#endif // MYTHREAD_H
