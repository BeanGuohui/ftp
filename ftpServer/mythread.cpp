#include "mythread.h"
#include <QThread>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include<string.h>
myThread::myThread(QObject *parent) : QObject(parent)
{
    threadIsRun = true;

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
    cdPath = shareFilePath;
    while(threadIsRun)
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
                    getFileInfo(shareFilePath);

                }
                else{
                    //客户端发来请求
                   dateDir tempDateDir;
                   DateHeader dateHeader;
                   int rec = recv(tempReadfds.fd_array[i],(char*)&dateHeader,dateHeader.dateLength,0);//...,0下载，>出消息，《0，报错
                   if(rec == 0)
                   {
                       qDebug() << "client exit!" << endl;
                       SOCKET sockTemp = tempReadfds.fd_array[i];
                       FD_CLR(tempReadfds.fd_array[i],&readfds);
                       closesocket(sockTemp);
                   }
                   else if(rec > 0)
                   {
                       switch (dateHeader.cmd) {
                       case CMD_CDNEXT:
                           {
                           cdNext tempCdNext;
                           int rec = recv(tempReadfds.fd_array[i],(char*)&tempCdNext + sizeof(dateHeader),sizeof(tempCdNext) - sizeof(dateHeader),0);
                           qDebug() << "tempCdfilename" << tempCdNext.fileName << endl;
                           qDebug() << "tempCdfilename" << tempCdNext.filePath << endl;
                           qDebug() << "tempCdfilename" << cdPath << endl;
                           cdPath = cdPath + "/" +QString(tempCdNext.filePath);


                           getFileInfo(cdPath);
                           qDebug() << "client send path will in: " << cdPath << endl;
                           break;
                       }
                       default://客户端连接上之后默认发送的文件列表
                       {
//                           int rec = recv(tempReadfds.fd_array[i],(char*)&tempDateDir + dateHeader.dateLength,tempDateDir.dateLength - dateHeader.dateLength,0);
//                           getFileInfo(tempDateDir.filePath);
//                           qDebug() << "接受的消息："  << endl;
                           break;
                       }
                       }

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
    }
}
void myThread::getFileInfo(QString filePath)
{
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
        dateDir tempDateDir;
        if(fileInfo.isDir())
            tempDateDir.fileType = 1;
        else
            tempDateDir.fileType = 0;
        const char *temp = fileInfo.fileName().toStdString().c_str();
        //sprintf(tempDateDir.fileName,"%s",temp);
        //memcpy(tempDateDir.fileName,"我",sizeof("我'"));
        //不知道为什么用filename就是传不过去，换成path就可以传过去了
        //tempDateDir.fileName = temp.c_str();
        //strcpy(tempDateDir.fileName, temp.c_str());
        //strcpy(tempDateDir.fileName, "123");
        //sprintf(tempDateDir.fileSize,"%d",fileInfo.size());
        memcpy(tempDateDir.filePath,temp,strlen(temp)+1);
        tempDateDir.fileSize = fileInfo.size();
        send(sockClient,(char *)&tempDateDir,sizeof(tempDateDir),0);
        //发送文件列表
        qDebug() << "send file : " << tempDateDir.fileName << tempDateDir.filePath <<  tempDateDir.fileSize << endl;
        i++;
    }
    while(i<list.size());
}
