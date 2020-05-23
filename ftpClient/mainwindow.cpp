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
    myT = new myThread;
    thread = new QThread(this);
    myT->moveToThread(thread);
    thread->start();


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_linkServer_clicked()
{
    myT->initSocket();
}
