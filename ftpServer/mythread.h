#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include "message.h"

#include <winsock2.h>

class myThread : public QObject
{
    Q_OBJECT
public:
    explicit myThread(QObject *parent = nullptr);

signals:


public slots:

       void initSocket();
public:
       bool threadIsRun;//判断进程是否关闭
       void getFileInfo(char *);//获取文件夹下的所有内容
private:
       SOCKET sockClient;
       SOCKET sockServer;
       SOCKADDR_IN addrClient;
       SOCKADDR_IN addrServer;
       WSADATA wsaData;


};

#endif // MYTHREAD_H
