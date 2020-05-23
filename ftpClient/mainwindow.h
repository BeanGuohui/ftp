#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QThread>
#include "mythread.h"

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

private:
    Ui::MainWindow *ui;
    QThread *thread;
    myThread *myT;

};

#endif // MAINWINDOW_H
