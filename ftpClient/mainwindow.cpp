#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTextCodec>
#include <QDebug>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));//设置中文不乱码
    ui->setupUi(this);
    myT = new myThread;
    thread = new QThread(this);
    myT->moveToThread(thread);
    thread->start();

    QStringList << "文件名" << "文件大小" << "是否是文件夹" << endl;
    ui

    connect(myT,&myThread::objectNameChanged,this,showFileTable);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_linkServer_clicked()
{
    myT->initSocket();
}

void MainWindow::showFileTable(char *name, short type, long long size)
{


}
