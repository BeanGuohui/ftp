#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <unistd.h>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QDir>

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
    //connect(ui->pushButton,&QPushButton::clicked,[=](){});//开始sock的运行

    connect(this,&MainWindow::destroyed,this,&MainWindow::closeEvent);

}

MainWindow::~MainWindow()
{
    delete ui;
    thread->exit(0);
    delete myT;
    delete thread;
}

void MainWindow::closeEvent()
{

    myT->threadIsRun = false;
    thread->quit();
    thread->wait();
    this->destroy();
}

void MainWindow::on_choseShareDir_clicked()
{


    QString filePath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                 "/home", QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    myT->shareFilePath = filePath;
    qDebug() << filePath << endl;
    QDir dir(filePath);
    if(!dir.exists())
    {
        qDebug() << "filepath not exists" << endl;
        return ;
    }
    dir.setFilter(QDir::Dirs|QDir::Files);//除了目录或文件，其他的过滤掉
    dir.setSorting(QDir::DirsFirst);//优先显示目录
    QFileInfoList list = dir.entryInfoList();//获取文件信息列表
    int i = 0;
    bool bIsDir;
    do{
        QFileInfo fileInfo = list.at(i);
        if(fileInfo.fileName()=="."|fileInfo.fileName()=="..")//跳过. , ..
        {
            i++;
            continue;
        }
        
        
        qDebug() << fileInfo.fileName() << fileInfo.size() ;//<< tempTime << endl;
        
        
        i++;
    }
    while(i<list.size());
}

void MainWindow::on_pushButton_3_clicked()
{

}
