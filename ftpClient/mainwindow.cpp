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
    connect(ui->linkServer,&QPushButton::clicked,myT,&myThread::initSocket);//链接服务器
    connect(ui->cdPreDir,&QPushButton::clicked,this,&MainWindow::cdPreDir);//返回上级目录



    ui->showFile->setRowCount(0);
    ui->showFile->setColumnCount(4); //设置列数为5

    QStringList header;
    header << QString::fromLocal8Bit("文件名") << QString::fromLocal8Bit("文件大小") << QString::fromLocal8Bit("文件类型")<< QString::fromLocal8Bit("文件路径");
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

void MainWindow::showFileTable(char *name, short type, long long int size,char *path)
{

    if(strcmp(name,"///") == 0)
    {
        allPath.push_back(QString(path));
        allPath.push_back(QString(path));

        return;
    }
    qDebug() << "charu==============" << endl;
    int row = ui->showFile->rowCount();
    ui->showFile->insertRow(row);
    ui->showFile->setItem(row,0,new QTableWidgetItem(QString(name)));
    ui->showFile->setItem(row,1,new QTableWidgetItem(QString::number(size)));
    ui->showFile->setItem(row,2,new QTableWidgetItem(QString::number(type)));
    ui->showFile->setItem(row,3,new QTableWidgetItem(QString(path)));
}

void MainWindow::cdNextDir()
{
    QString selectFile = ui->showFile->selectedItems().at(0)->text();//获取选中行的文件名字
    QString selectFilePath = ui->showFile->selectedItems().at(3)->text();//获取选中文件的路径
    QString type = ui->showFile->selectedItems().at(2)->text();
    if(type.toInt() != 1)
    {
        QMessageBox::information(this,"error","that is not a dirction!",QMessageBox::Ok);
        return;
    }
    memcpy(myT->tempCdNext.fileName,selectFile.toStdString().c_str(),strlen(selectFile.toStdString().c_str())+1);
    memcpy(myT->tempCdNext.filePath,selectFilePath.toStdString().c_str(),strlen(selectFilePath.toStdString().c_str())+1);
    allPath.push_back(selectFilePath);
    qDebug() << "myTcdNext" << myT->tempCdNext.fileName << "=============" << myT->tempCdNext.filePath << endl;
    //QThread::sleep(2);

    ui->showFile->clearContents();//清空之前的表
    ui->showFile->setRowCount(0);
    myT->issend = true;//开放发送消息

}

void MainWindow::cdPreDir()
{//加以判断是否已经是主目录了。
    if(allPath.size() == 1)
    {
        QMessageBox::information(this,"information",QString::fromLocal8Bit("这是主目录了"),QMessageBox::Ok);
        return ;
    }
    QString path;
    allPath.pop_back();
    path = allPath.back();
    if(allPath.size() <= 2)
    {
        ;
    }
    else{

        allPath.pop_back();
    }

    memcpy(myT->tempCdNext.filePath,path.toStdString().c_str(),strlen(path.toStdString().c_str())+1);
    qDebug() << "myTcdNext" << "=============" << myT->tempCdNext.filePath << endl;
    //QThread::sleep(2);
    ui->showFile->clearContents();//清空之前的表
    ui->showFile->setRowCount(0);
    myT->issend = true;//开放发送消息
}
