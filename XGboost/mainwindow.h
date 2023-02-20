#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QTimer>
#include <QApplication>
#include <python/Python.h>
#include <numpy/ndarrayobject.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int pythonInit();

private slots:
    void on_btnPredict_clicked();

    void on_btnTrain_clicked();

    void on_btnSelect_clicked();

private:
    Ui::MainWindow *ui;


    PyObject* pObject;
    QString m_TrainFile;
    bool m_Flag = false;

};

#endif // MAINWINDOW_H
