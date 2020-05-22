#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTextCodec>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));//设置中文不乱码
    ui->setupUi(this);
    memset(&addrClient,0,sizeof(addrClient));
    memset(&addrClient,0,sizeof(addrServer));
    initSocket();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initSocket()
{
    if(WSAStartup(MAKEWORD(2,2),&wsaData) !=0)
    {
        WSACleanup();
         qDebug() << "in wsastart error" << endl;
        return ;
    }

    sockClient = socket(AF_INET,SOCK_STREAM,0);

    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(6666);

    addrServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    int rect = ::connect(sockClient,(SOCKADDR*)&addrServer,sizeof(addrServer));
    if(rect == -1)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("温馨提示"),QString::fromLocal8Bit("操作失败"),QMessageBox::Ok);
        GetLastError();
        return ;
    }
    else if(rect == 0)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("温馨提示"),QString::fromLocal8Bit("链接成功，请继续操作！"),QMessageBox::Ok);
    }




}
