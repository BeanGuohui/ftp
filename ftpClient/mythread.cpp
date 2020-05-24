#include "mythread.h"
#include <QTextCodec>
#include <QDebug>
#include <iostream>
#include <thread>
using namespace std;
void runsend(myThread *myThis)
{
    while(1)
    {
        if(myThis->issend == true)
        {
            send(myThis->sockClient,(char *)&(myThis->tempCdNext),sizeof(myThis->tempCdNext),0);//给服务器发送指令
            qDebug() << "1111111111111111111111111111===============" << endl;
            qDebug() << "myThis" << myThis->tempCdNext.fileName << "==" << myThis->tempCdNext.filePath << endl;
            myThis->issend = false;
        }

    }
}

myThread::myThread(QObject *parent) : QObject(parent)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));//设置中文不乱码
    memset(&addrClient,0,sizeof(addrClient));
    memset(&addrClient,0,sizeof(addrServer));
    issend = false;

}
void myThread::initSocket()
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
        qDebug() << "connect error" << endl;
        //QMessageBox::information(this,QString::fromLocal8Bit("温馨提示"),QString::fromLocal8Bit("操作失败"),QMessageBox::Ok);
        GetLastError();
        return ;
    }
    else if(rect == 0)
    {
        qDebug() << "sucess" << endl;
        //QMessageBox::information(this,QString::fromLocal8Bit("温馨提示"),QString::fromLocal8Bit("链接成功，请继续操作！"),QMessageBox::Ok);
    }
    std::thread sendThread(runsend,this);
    sendThread.detach();
   while(1)
   {
       dealReadfd();
   }


}


void myThread::dealReadfd()
{

    //服务器发来数据
    dateDir tempDateDir;
    DateHeader dateHeader;
    int rec = recv(sockClient,(char*)&dateHeader,sizeof(dateHeader),0);//...,0下载，>出消息，《0，报错
    qDebug() << QString::fromLocal8Bit("接受数据头") << endl;
    qDebug() << dateHeader.cmd << endl;
    if(rec == 0)
    {
       //服务器推出
    }
    else if(rec > 0)
    {
       int rec = recv(sockClient,(char*)&tempDateDir + sizeof(dateHeader),sizeof(tempDateDir) - sizeof(dateHeader),0);
       emit showFileInfo(tempDateDir.fileName,tempDateDir.fileType,tempDateDir.fileSize,tempDateDir.filePath);

       qDebug() << QString::fromLocal8Bit("接受数据身子")  << endl;
       qDebug() <<  "filename: " << tempDateDir.fileName
                 << "fileType:" << tempDateDir.fileType
                 << "filepath:" << tempDateDir.filePath
                 << "filesize:" << tempDateDir.fileSize << endl;
       //下面进行消息分类
    }
    else{
       //报错,出现错误，wsagetlasterror();
    }

}
