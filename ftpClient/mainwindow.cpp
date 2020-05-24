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
    //connect(ui->downFile,&QPushButton::clicked,this,&MainWindow::sendDownFile);//发送给进程具体的下载信息

    connect(ui->pbdownFile,&QPushButton::clicked,this,&MainWindow::sendDownFile);

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
    int i = 0;
    if(strcmp(myT->alltempDateDir.at(0).fileName,"///") == 0)
    {
        allPath.push_back(QString(myT->alltempDateDir.at(0).filePath));
        allPath.push_back(QString(myT->alltempDateDir.at(0).filePath));
        i = 1;
    }
    for(; i < myT->alltempDateDir.size()-1;i++)//最后一个是文件读取结尾信号的标志
    {
        int row = ui->showFile->rowCount();
        ui->showFile->insertRow(row);
        ui->showFile->setItem(row,0,new QTableWidgetItem(myT->alltempDateDir.at(i).fileName));
        ui->showFile->setItem(row,1,new QTableWidgetItem(QString::number(myT->alltempDateDir.at(i).fileSize)));
        ui->showFile->setItem(row,2,new QTableWidgetItem(QString::number(myT->alltempDateDir.at(i).fileType)));
        ui->showFile->setItem(row,3,new QTableWidgetItem(myT->alltempDateDir.at(i).filePath));
        qDebug() << "filename --------" << name <<endl;
    }
    myT->alltempDateDir.clear();

//    if(strcmp(name,"///") == 0)
//    {
//        allPath.push_back(QString(path));
//        allPath.push_back(QString(path));

//        return;
//    }
//    qDebug() << "=================this is showtable file=========================="<< endl;
//    int row = ui->showFile->rowCount();
//    ui->showFile->insertRow(row);
//    ui->showFile->setItem(row,0,new QTableWidgetItem(name));
//    ui->showFile->setItem(row,1,new QTableWidgetItem(QString::number(size)));
//    ui->showFile->setItem(row,2,new QTableWidgetItem(QString::number(type)));
//    ui->showFile->setItem(row,3,new QTableWidgetItem(path));
//    qDebug() << "filename --------" << name <<endl;

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

void MainWindow::sendDownFile()
{
    //默认下载的路径是在家目录下
    qDebug() << "***********************************" <<endl;
    qDebug() << "this is send" << endl;

    QString type = ui->showFile->selectedItems().at(2)->text();
    if(type.toInt() == 1)
    {
        QMessageBox::information(this,"error",QString::fromLocal8Bit("暂不支持下载文件夹!"),QMessageBox::Ok);
        return;
    }
    QString selectFile = ui->showFile->selectedItems().at(0)->text();//获取选中行的文件名字
    QString selectFilePath = ui->showFile->selectedItems().at(3)->text();//获取选中文件的路径
    memcpy(myT->tempCdNext.fileName,selectFile.toStdString().c_str(),strlen(selectFile.toStdString().c_str())+1);
    memcpy(myT->tempCdNext.filePath,selectFilePath.toStdString().c_str(),strlen(selectFilePath.toStdString().c_str())+1);
    myT->tempCdNext.cmd = CMD_DOWNLOAD;
    qDebug() << "=================this is download file=========================="<< endl;
    qDebug() << "myTcdNext" << myT->tempCdNext.fileName << "=============" << myT->tempCdNext.filePath << endl;
    qDebug() << "=================================================================" << endl;
    //QThread::sleep(2);
    myT->issend = true;//开放发送消息
}
