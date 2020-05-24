#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QThread>
#include "mythread.h"
#include <QVector>

#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initSocket();

private slots:
    void on_linkServer_clicked();
    void showFileTable(char*, short,long long,char*);
    void cdNextDir();//进入选中文件夹
    void cdPreDir();//返回上级目录
signals:
    void sendNextDir(QString);

private:
    Ui::MainWindow *ui;
    QThread *thread;
    myThread *myT;
    QVector<QString> allPath;
};

#endif // MAINWINDOW_H
