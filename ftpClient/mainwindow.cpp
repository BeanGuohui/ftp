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
    connect(ui->linkServer,&QPushButton::clicked,myT,&myThread::initSocket);



    ui->showFile->setRowCount(0);
    ui->showFile->setColumnCount(3); //设置列数为5

    QStringList header;
    header << QString::fromLocal8Bit("文件名") << QString::fromLocal8Bit("文件大小") << QString::fromLocal8Bit("文件类型");
    ui->showFile->setHorizontalHeaderLabels(header);
    ui->showFile->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(myT,&myThread::showFileInfo,this,&MainWindow::showFileTable);
    connect(ui->cdNextDir,&QPushButton::clicked,this,&MainWindow::cdNextDir);//获取要进入的目录位置
    //connect(this,&MainWindow::sendNextDir,myT,&myThread::sendNextDir);//向线程发送要进入的目录位置

}

MainWindow::~MainWindow()
{
    delete ui;
}


//void MainWindow::on_linkServer_clicked()
//{
//    thread->start();
//    myT->initSocket();
//}

void MainWindow::showFileTable(char *name, short type, long long int size)
{
    qDebug() << "charu==============" << endl;
    int row = ui->showFile->rowCount();
    ui->showFile->insertRow(row);
    ui->showFile->setItem(row,0,new QTableWidgetItem(QString(name)));
    ui->showFile->setItem(row,1,new QTableWidgetItem(QString::number(size)));
    ui->showFile->setItem(row,2,new QTableWidgetItem(QString::number(type)));
}

void MainWindow::cdNextDir()
{
    QString selectFile = ui->showFile->selectedItems().at(0)->text();//获取选中行的文件名字
    QString type = ui->showFile->selectedItems().at(2)->text();
    if(type.toInt() != 1)
    {
        QMessageBox::information(this,"error","that is not a dirction!",QMessageBox::Ok);
    }
    memcpy(myT->tempCdNext.fileName,selectFile.toStdString().c_str(),strlen(selectFile.toStdString().c_str())+1);
    memcpy(myT->tempCdNext.filePath,selectFile.toStdString().c_str(),strlen(selectFile.toStdString().c_str())+1);
    qDebug() << "myTcdNext" << myT->tempCdNext.fileName << "=============" << myT->tempCdNext.filePath << endl;
    QThread::sleep(10);
    myT->issend = true;//开放发送消息

}
