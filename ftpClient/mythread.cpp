#include "mythread.h"
#include "message.h"
#include <QTextCodec>
#include <QDebug>

myThread::myThread(QObject *parent) : QObject(parent)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));//设置中文不乱码
    memset(&addrClient,0,sizeof(addrClient));
    memset(&addrClient,0,sizeof(addrServer));
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_SET(sockClient,&readfds);


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
    
    
    while(1)
    {
        tempReadfds = readfds;

        int res = select(0,&tempReadfds,&writefds,NULL,0);
        if(res == 0)
        {
            continue;
        }
        else if(res > 0)
        {
            //有相应
            //编写读数据;
            dealReadfd();

        }

        else{
            //发生错误
        }
    }


}


void myThread::dealReadfd()
{
    for(int i=0; i < tempReadfds.fd_count;i++)
    {

            //服务器发来数据
           dateDir tempDateDir;
           DateHeader dateHeader;
           int rec = recv(tempReadfds.fd_array[i],(char*)&dateHeader,dateHeader.dateLength,0);//...,0下载，>出消息，《0，报错
           if(rec == 0)
           {
               //服务器推出
//                       qDebug() << "client exit!" << endl;
//                       SOCKET sockTemp = tempReadfds.fd_array[i];
//                       FD_CLR(tempReadfds.fd_array[i],&readfds);
//                       closesocket(sockTemp);
           }
           else if(rec > 0)
           {
               int rec = recv(tempReadfds.fd_array[i],(char*)&tempDateDir + dateHeader.dateLength,tempDateDir.dateLength - dateHeader.dateLength,0);
               qDebug() << "server's send message："  << endl;
               qDebug() << tempDateDir.fileName << tempDateDir.fileType << endl;
               //下面进行消息分类

           }
           else{
               //报错,出现错误，wsagetlasterror();
           }
    }
}
