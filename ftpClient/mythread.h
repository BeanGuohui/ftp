#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <winsock2.h>
#include <windows.h>

class myThread : public QObject
{
    Q_OBJECT
public:
    explicit myThread(QObject *parent = nullptr);
    void initSocket();
    void dealReadfd();

signals:

public slots:
private:
    fd_set readfds;/*检查是否有可读的socket*/
    fd_set writefds; /*检查是否有可写的socket*/

    fd_set tempReadfds;
    fd_set tempWritefds;
    SOCKET sockClient;
    SOCKADDR_IN addrClient;
    SOCKADDR_IN addrServer;
    WSADATA wsaData;
};

#endif // MYTHREAD_H
