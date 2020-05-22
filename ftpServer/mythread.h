#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>

#include <winsock2.h>
//#pragma comment( lib, "ws2_32.lib" )

class myThread : public QObject
{
    Q_OBJECT
public:
    explicit myThread(QObject *parent = nullptr);

signals:


public slots:

       void initSocket();
private:
       SOCKET sockClient;
       SOCKET sockServer;
       SOCKADDR_IN addrClient;
       SOCKADDR_IN addrServer;
       WSADATA wsaData;

};

#endif // MYTHREAD_H
