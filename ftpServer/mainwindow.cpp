#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <unistd.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myT = new myThread;
    thread = new QThread(this);
    myT->moveToThread(thread);
    thread->start();
    connect(ui->pushButton,&QPushButton::clicked,myT,&myThread::initSocket);//开始sock的运行
    connect(this,&MainWindow::destroyed,this,&MainWindow::closeWindow);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete myT;
    delete thread;
}

void MainWindow::closeWindow()
{
    thread->quit();
    thread->wait();
    this->destroy();
}
