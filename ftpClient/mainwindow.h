#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <winsock2.h>
#pragma comment( lib, "ws2_32.lib" )
#include <windows.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initSocket();

private:
    Ui::MainWindow *ui;
    SOCKET sockClient;
    SOCKADDR_IN addrClient;
    SOCKADDR_IN addrServer;
    WSADATA wsaData;
};

#endif // MAINWINDOW_H
