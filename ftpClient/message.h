
#ifndef MESSAGE_H
#define MESSAGE_H
enum CMD{
    CMD_DIR,
    CMD_DELE,
    CMD_DATA,//用来指定传送数据或者发送数据的头
    CMD_CDNEXT//进入下一层的消息
};
struct DateHeader
{    
    short cmd;//命令
};
//数据包
struct dateDir:public DateHeader
{
    dateDir(){
        cmd = CMD_DIR;
    }
    char fileName[255];
    char filePath[255];
    short fileType;//文件类型，文件1，其他0；
    long long int fileSize;
};
struct cdNext:public DateHeader//客户端发来的进入下一层文件夹的请求
{
    cdNext(){
        cmd = CMD_CDNEXT;
    }
    char fileName[100];
    char filePath[100];
};
#endif // MESSAGE_H


