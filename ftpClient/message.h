
#ifndef MESSAGE_H
#define MESSAGE_H
enum CMD{
    CMD_DIR,
    CMD_DELE,
    CMD_DATA//用来指定传送数据或者发送数据的头
};
struct DateHeader
{
    int dateLength = sizeof(DateHeader);
    short cmd;//命令
};
//数据包
struct dateDir:public DateHeader
{
    dateDir(){
        dateLength = sizeof(dateDir);
        cmd = CMD_DIR;
    }
    char fileName[255];
    char filePath[255];
    int fileType;//文件类型，文件1，其他0；
};
#endif // MESSAGE_H

