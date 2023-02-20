#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Xgboost");
    qApp->setStyleSheet("file:///:/qss/mainwindow.qss");  // 加载qss文件
    pythonInit(); // python环境初始化

    ui->lEParam1->setReadOnly(true);
}

MainWindow::~MainWindow()
{
    Py_Finalize();
    delete ui;
}

int MainWindow::pythonInit()
{
    Py_SetPythonHome(reinterpret_cast<const wchar_t*>(L"G:\\RJAZ\\Miniconnda\\data\\envs\\_xg")); // 设置 python 环境目录

    Py_Initialize(); // 初始化
    if(!Py_IsInitialized())
    {
        qDebug() << "Py_Initialize Error";
        return -1;
    }


    PyRun_SimpleString("import sys");                             // 加载 sys 模块
    PyRun_SimpleString("import os");
    PyRun_SimpleString("import numpy as np");
    PyRun_SimpleString("sys.path.append('./')");                   // 设置 python 文件搜索路径
    PyRun_SimpleString("sys.path.append('G:\\RJAZ\\Miniconnda\\data\\envs\\_xg\\Lib\\site-packages')");                   // 设置 python 文件搜索路径


    PyObject* pModule = PyImport_ImportModule("xg"); // 加载python模块
    if (!pModule) {
        PyErr_Print();
        Py_Finalize();
        qDebug("Cant open python file!\n");
        return -2;
    }


    PyObject *pDict = PyModule_GetDict(pModule);                  // 加载文件中的函数名、类名

    if(!pDict)
    {
        Py_Finalize();
        qDebug() << "PyModule_GetDict Error";
        return -3;
    }

    PyObject *pClass = PyDict_GetItemString(pDict, "MyXgboost");   // 获取类名

    if(!pClass)
    {
        Py_Finalize();
        qDebug() << "PyDict_GetItemString Error";
        return -4;
    }

    pObject = PyObject_CallObject(pClass, nullptr);               // 实例化对象，相当于调用'__init__(self)',参数为null

    if(!pObject)
    {
        Py_Finalize();
        qDebug() << "PyObject_CallObject Error";
        return -5;
    }

    _Py_XDECREF(pModule);
    _Py_XDECREF(pDict);
    _Py_XDECREF(pClass);

    return 0;
}

// 预测按钮槽函数
void MainWindow::on_btnPredict_clicked()
{
    if(!m_Flag) // 未训练则返回
    {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("请训练模型"));
        return;
    }

    int number = ui->lENumber->text().toInt();
    QString sex = ui->cBSex->currentText();
    int age = ui->lEAge->text().toInt();
    int param1 = ui->lEParam1->text().toInt();
    int param2 = ui->lEParam2->text().toInt();
    int param3 = ui->lEParam3->text().toInt();
    QString invest_time = ui->lEInvestTime->text();
    QString param4 = ui->cBParam4->currentText();
    double stock = ui->lEStock->text().toDouble();
    int param5 = ui->lEParam5->text().toInt();
    int param6 = ui->lEParam6->text().toInt();

    PyObject * _number = PyTuple_New(1); /* 创建1个元素的元组 */
    PyObject * _sex = PyTuple_New(1); /* 创建1个元素的元组 */
    PyObject * _age = PyTuple_New(1); /* 创建1个元素的元组 */
    PyObject * _param1 = PyTuple_New(1); /* 创建1个元素的元组 */
    PyObject * _param2 = PyTuple_New(1); /* 创建1个元素的元组 */
    PyObject * _param3 = PyTuple_New(1); /* 创建1个元素的元组 */
    PyObject * _invest_time = PyTuple_New(1); /* 创建1个元素的元组 */
    PyObject * _param4 = PyTuple_New(1); /* 创建1个元素的元组 */
    PyObject * _stock = PyTuple_New(1); /* 创建1个元素的元组 */
    PyObject * _param5 = PyTuple_New(1); /* 创建1个元素的元组 */
    PyObject * _param6 = PyTuple_New(1); /* 创建1个元素的元组 */

    PyTuple_SetItem(_number, 0, Py_BuildValue("i", number)); // 在元组中插入值传递给python函数
    PyTuple_SetItem(_sex, 0, Py_BuildValue("s", sex.toLatin1().data()));
    PyTuple_SetItem(_age, 0, Py_BuildValue("i", age));
    PyTuple_SetItem(_param1, 0, Py_BuildValue("i", param1));
    PyTuple_SetItem(_param2, 0, Py_BuildValue("i", param2));
    PyTuple_SetItem(_param3, 0, Py_BuildValue("i", param3));
    PyTuple_SetItem(_invest_time, 0, Py_BuildValue("s", invest_time.toLatin1().data()));
    PyTuple_SetItem(_param4, 0, Py_BuildValue("s", param4.toLatin1().data()));
    PyTuple_SetItem(_stock, 0, Py_BuildValue("f", stock));
    PyTuple_SetItem(_param5, 0, Py_BuildValue("i", param5));
    PyTuple_SetItem(_param6, 0, Py_BuildValue("i", param6));

    // number
    PyObject* pFun1 = PyObject_GetAttrString(pObject,"get_number"); // 获取 xg模块中的 get_number函数
    if(!pFun1){
        PyErr_Print();
        qDebug() <<"Get function get_number failed";
    }
    PyObject *pRet1 = PyObject_CallObject(pFun1, _number); // 调用 xg模块中的 get_number函数，传递 编号 值

    if(pRet1 == NULL)
    {
        qDebug() << __LINE__;
        PyErr_Print();
        return;
    }

    // sex
    PyObject* pFun2 = PyObject_GetAttrString(pObject,"get_sex");
    if(!pFun2){
        PyErr_Print();
        qDebug() <<"Get function get_sex failed";
    }
    PyObject *pRet2 = PyObject_CallObject(pFun2, _sex);

    if(pRet2 == NULL)
    {
        qDebug() << __LINE__;
        PyErr_Print();
        return;
    }

    // age
    PyObject* pFun3 = PyObject_GetAttrString(pObject,"get_age");
    if(!pFun3){
        PyErr_Print();
        qDebug() <<"Get function get_age failed";
    }
    PyObject *pRet3 = PyObject_CallObject(pFun3, _age);

    if(pRet3 == NULL)
    {
        qDebug() << __LINE__;
        PyErr_Print();
        return;
    }

    // param1
    PyObject* pFun4 = PyObject_GetAttrString(pObject,"get_param1");
    if(!pFun4){
        PyErr_Print();
        qDebug() <<"Get function get_param1 failed";
    }
    PyObject *pRet4 = PyObject_CallObject(pFun4, _param1);

    if(pRet4 == NULL)
    {
        qDebug() << __LINE__;
        PyErr_Print();
        return;
    }

    // param2
    PyObject* pFun5 = PyObject_GetAttrString(pObject,"get_param2");
    if(!pFun5){
        PyErr_Print();
        qDebug() <<"Get function get_param2 failed";
    }
    PyObject *pRet5 = PyObject_CallObject(pFun5, _param2);

    if(pRet5 == NULL)
    {
        qDebug() << __LINE__;
        PyErr_Print();
        return;
    }

    // param3
    PyObject* pFun6 = PyObject_GetAttrString(pObject,"get_param3");
    if(!pFun6){
        PyErr_Print();
        qDebug() <<"Get function get_param3 failed";
    }
    PyObject *pRet6 = PyObject_CallObject(pFun6, _param3);

    if(pRet6 == NULL)
    {
        qDebug() << __LINE__;
        PyErr_Print();
        return;
    }

    // investtime
    PyObject* pFun7 = PyObject_GetAttrString(pObject,"get_investtime");
    if(!pFun7){
        PyErr_Print();
        qDebug() <<"Get function get_investtime failed";
    }
    PyObject *pRet7 = PyObject_CallObject(pFun7, _invest_time);

    if(pRet7 == NULL)
    {
        qDebug() << __LINE__;
        PyErr_Print();
        return;
    }


    // param4
    PyObject* pFun8 = PyObject_GetAttrString(pObject,"get_param4");
    if(!pFun8){
        PyErr_Print();
        qDebug() <<"Get function get_param4 failed";
    }
    PyObject *pRet8 = PyObject_CallObject(pFun8, _param4);

    if(pRet8 == NULL)
    {
        qDebug() << __LINE__;
        PyErr_Print();
        return;
    }


    // stock
    PyObject* pFun9 = PyObject_GetAttrString(pObject,"get_stock");
    if(!pFun9){
        PyErr_Print();
        qDebug() <<"Get function get_stock failed";
    }
    PyObject *pRet9 = PyObject_CallObject(pFun9, _stock);

    if(pRet9 == NULL)
    {
        qDebug() << __LINE__;
        PyErr_Print();
        return;
    }

    // param5
    PyObject* pFun10 = PyObject_GetAttrString(pObject,"get_param5");
    if(!pFun10){
        PyErr_Print();
        qDebug() <<"Get function get_param5 failed";
    }
    PyObject *pRet10 = PyObject_CallObject(pFun10, _param5);

    if(pRet10 == NULL)
    {
        qDebug() << __LINE__;
        PyErr_Print();
        return;
    }

    // param6
    PyObject* pFun11 = PyObject_GetAttrString(pObject,"get_param6");
    if(!pFun11){
        PyErr_Print();
        qDebug() <<"Get function get_param6 failed";
    }
    PyObject *pRet11 = PyObject_CallObject(pFun11, _param6);

    if(pRet11 == NULL)
    {
        qDebug() << __LINE__;
        PyErr_Print();
        return;
    }


    PyObject* pFun12 = PyObject_GetAttrString(pObject,"predict1"); // 调用预测函数
    if(!pFun12){
        PyErr_Print();
        qDebug() <<"Get function predict1 failed";
        return;
    }
    PyObject *pRet12 = PyObject_CallObject(pFun12, NULL);
    if(pRet12 == NULL)
    {
        qDebug() << __LINE__;
        PyErr_Print();
        return;
    }
    double result = PyFloat_AsDouble(pRet12); // 解析返回值
    
    ui->lEResult->setText(QString::number(result)); // 将预测结果显示在界面上

}

// 训练按钮槽函数
void MainWindow::on_btnTrain_clicked()
{
    if(m_TrainFile == "")
    {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("请选择训练数据集"));
        return;
    }

    PyObject * csv_file = PyTuple_New(1); /* 创建1个元素的元组 */
    PyTuple_SetItem(csv_file, 0, Py_BuildValue("s", m_TrainFile.toLatin1().data()));

    PyObject* pFun = PyObject_GetAttrString(pObject,"train_model");
    if(!pFun){
        PyErr_Print();
        qDebug() <<"Get function train_model failed";
        return;
    }

    ui->labelStatus->setText(QStringLiteral("训练中，请勿操作..."));
    QApplication::processEvents(); // 先处理还没有处理的事件，主要是为了显示上一句代码
    PyObject *pRet = PyObject_CallObject(pFun, csv_file);
    if(pRet == NULL)
    {

        qDebug() << __LINE__;
        PyErr_Print();
        return;
    }
    int result = PyLong_AsLong(pRet); // 训练完成，解析返回值，如果等于55则训练完成
    if(result == 55)
    {
        m_Flag = true;
        ui->labelStatus->setText(QStringLiteral("训练完成"));
    }
}

// 选择文件槽函数
void MainWindow::on_btnSelect_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, QStringLiteral("选择训练数据集"), "./");
    if(file.isNull()) return;
    QString filename = file.split('/').back();
    ui->lETrainFile->setText(filename);
    m_TrainFile = file;
}
