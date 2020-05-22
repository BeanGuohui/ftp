#include "mythread.h"
#include <QThread>
#include <QDebug>
myThread::myThread(QObject *parent) : QObject(parent)
{

}
void myThread::initSocket()
{
    if(WSAStartup(MAKEWORD(2,2),&wsaData) !=0)
    {
        WSACleanup();
        qDebug() << "in wsastart error" << endl;
        return ;
    }
    sockServer = socket(AF_INET,SOCK_STREAM,0);

    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(6666);

    addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    if ( 0 != bind(sockServer,(SOCKADDR*)&addrServer,sizeof(addrServer)))
        qDebug() << "bind error" << endl;
    else
        qDebug() << "bind sucess" << endl;
    listen(sockServer,10);
    int len = sizeof(addrClient);
    fd_set readfds;/*检查是否有可读的socket*/
    fd_set writefds; /*检查是否有可写的socket*/
    fd_set exceptfds; /*检查socket上的异常错误*/
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);
    FD_SET(sockServer,&readfds);
    while(1)
    {
        fd_set tempReadfds = readfds;
        
        int res = select(0,&tempReadfds,&writefds,&exceptfds,0);
        if(res == 0)
        {
            continue;
        }
        else if(res > 0)
        {
            //有相应
            for(int i=0; i < tempReadfds.fd_count;i++)
            {
                if(tempReadfds.fd_array[i] == sockServer)
                {
                    //有客户端链接
                    int len = sizeof(addrClient);
                    sockClient = accept(sockServer,(SOCKADDR*)&addrClient,&len);
                    if(SOCKET_ERROR == sockClient)
                    {
                        //链接出错，等一下写进日志里面
                        continue;
                    }

                    FD_SET(sockClient,&readfds);
                    qDebug() << "IP: " << ntohl(addrClient.sin_addr.S_un.S_addr) << endl;

                }
                else{
                    //客户端发来请求
                    char recvBuf[100];
                   int rec = recv(tempReadfds.fd_array[i],recvBuf,sizeof(recvBuf),0);//...,0下载，>出消息，《0，报错
                   if(rec == 0)
                   {
                       qDebug() << "client exit!" << endl;
                       SOCKET sockTemp = tempReadfds.fd_array[i];
                       FD_CLR(tempReadfds.fd_array[i],&readfds);
                       closesocket(sockTemp);
                   }
                   else if(rec > 0)
                   {
                       qDebug() << "接受的消息：" << recvBuf << endl;
                   }
                   else{
                       //报错,出现错误，wsagetlasterror();
                   }
                }
                }
            }

        else{
            //发生错误
        }
            
    
        
        
        
        
        sockClient = accept(sockServer,(SOCKADDR*)&addrClient,&len);
        qDebug() << QString::fromLocal8Bit("客户端链接：IP: ")
        //<< inet_ntop(AF_INET,&addrClient.sin_addr.s_addr,CLIENTIP,sizeof(CLIENTIP))
                << "PORT:" << ntohs(addrClient.sin_port);
    }

}
