#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMessageBox>
#include<QtDebug>
#include<QFile>
#include<QFileDialog>
#include<QDir>
#include<QTextStream>
#include<cv.h>
#include<highgui.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
     void deal(IplImage* pImg);
     IplImage* MainWindow::strengthImage_Histogram(IplImage* img);
     IplImage* MainWindow::myhistf(IplImage* img,double khist,double lowgray);
      IplImage* MainWindow::myline(IplImage* img);
     int MainWindow::max(int* s,int len);


private:
    Ui::MainWindow *ui;
private slots:
    void newFileSlot();
    void openFileSlot();
};

#endif // MAINWINDOW_H
