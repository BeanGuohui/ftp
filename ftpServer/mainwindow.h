#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>

#include "mythread.h"
#include <QThread>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    virtual void closeEvent();


private slots:
    void on_pushButton_3_clicked();

    void on_choseShareDir_clicked();

private:
    Ui::MainWindow *ui;

    myThread *myT;
    QThread *thread;
};

#endif // MAINWINDOW_H
