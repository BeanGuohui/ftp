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
     void showFileInfo(char*, short,long long int);
public slots:


private:

    SOCKET sockClient;
    SOCKADDR_IN addrClient;
    SOCKADDR_IN addrServer;
    WSADATA wsaData;
};

#endif // MYTHREAD_H
