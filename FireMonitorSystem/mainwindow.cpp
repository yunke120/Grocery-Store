#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>
#include <QDateTime>
#include <QPen>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    chartInit();
    serialInit();


    //    ui->btnWindow->setStyleSheet("QPushButton{border-image:url(:/imgs/off.png);}");

    pTimer = new QTimer(this);
    connect(pTimer, &QTimer::timeout, this, [=](){

        QDateTime curDateTime = QDateTime::currentDateTime();
        qint64 startTimeT = curDateTime.toSecsSinceEpoch() -55;
        qint64 endTimeT = curDateTime.toSecsSinceEpoch() + 5;
        QDateTime startDateTime = QDateTime::fromSecsSinceEpoch(startTimeT);
        QDateTime endDateTime = QDateTime::fromSecsSinceEpoch(endTimeT);
        m_AxisX->setRange(startDateTime, endDateTime);

        double temp = QRandomGenerator::global()->bounded(20,45) + QRandomGenerator::global()->bounded(0.5);
        pSplineSeriesTemp->append(curDateTime.toMSecsSinceEpoch(), temp);
        temp = QRandomGenerator::global()->bounded(10,30) + QRandomGenerator::global()->bounded(0.5);
        pSplineSeriesHumi->append(curDateTime.toMSecsSinceEpoch(), temp);

    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::chartInit()
{
    pChartTemp = new QChart;
    pSplineSeriesTemp = new QSplineSeries(this);
    pSplineSeriesHumi = new QSplineSeries(this);
    pSplineSeriesTemp->setName("Temperature");
    pSplineSeriesHumi->setName("Humidity");

    QDateTime curDateTime = QDateTime::currentDateTime();
    qint64 startTimeT = curDateTime.toSecsSinceEpoch() -55;
    qint64 endTimeT = curDateTime.toSecsSinceEpoch() + 5;
    QDateTime startDateTime = QDateTime::fromSecsSinceEpoch(startTimeT);
    QDateTime endDateTime = QDateTime::fromSecsSinceEpoch(endTimeT);


    m_AxisX = new QDateTimeAxis(this);
    m_AxisX->setFormat("HH:mm:ss");
    m_AxisX->setTickCount(X_MAXSLICES);
    m_AxisX->setTitleText("t/s");
    m_AxisX->setRange(startDateTime, endDateTime);

    m_TempY = new QValueAxis(this);
    m_TempY->setRange(-0,50);
    m_TempY->setTickCount(15);
    m_TempY->setTitleText("温度℃");
    m_TempY->setLabelFormat("%4.2f");
    m_TempY->setLinePenColor(Qt::red);
    m_TempY->setTitleBrush(Qt::red);
    QPen pen(Qt::red, 1, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
    pSplineSeriesTemp->setPen(pen);

    m_HumiY = new QValueAxis(this);
    m_HumiY->setRange(0,100);
    m_HumiY->setTickCount(15);
    m_HumiY->setTitleText("湿度RH");
    m_HumiY->setLabelFormat("%4.2f");
    m_HumiY->setLinePenColor(Qt::blue);
    m_HumiY->setTitleBrush(Qt::blue);
    QPen pen2(Qt::blue, 1, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
    pSplineSeriesHumi->setPen(pen2);

//    pChartTemp->setAnimationOptions(QChart::SeriesAnimations);
    pChartTemp->setTitle("Temperature & Humidity");
    pChartTemp->setTheme(QChart::ChartTheme::ChartThemeDark);

    pChartTemp->addAxis(m_AxisX, Qt::AlignBottom);
    pChartTemp->addAxis(m_TempY, Qt::AlignLeft);
    pChartTemp->addAxis(m_HumiY, Qt::AlignRight);
//    pChartTemp->legend()->hide();

    pChartTemp->addSeries(pSplineSeriesTemp);
    pChartTemp->addSeries(pSplineSeriesHumi);

    pSplineSeriesTemp->attachAxis(m_AxisX);
    pSplineSeriesTemp->attachAxis(m_TempY);

    pSplineSeriesHumi->attachAxis(m_AxisX);
    pSplineSeriesHumi->attachAxis(m_HumiY);

    ui->chartTemp->setRenderHint(QPainter::Antialiasing);
    ui->chartTemp->setChart(pChartTemp);



}

void MainWindow::serialInit()
{
    pSerial = new QSerialPort(this);
    pSerial->setBaudRate(QSerialPort::Baud115200);
    pSerial->setDataBits(QSerialPort::Data8);
    pSerial->setStopBits(QSerialPort::OneStop);
    pSerial->setParity(QSerialPort::NoParity);
    pSerial->setFlowControl(QSerialPort::NoFlowControl);
    connect(pSerial, &QSerialPort::readyRead, this, &MainWindow::slotSerialReadyRead);

    ui->cBPort->clear();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->cBPort->addItem(info.portName());
    }
}
#include <QtCharts/QValueAxis>
void MainWindow::appendData(double temp, double humi)
{
    QDateTime curDateTime = QDateTime::currentDateTime();
    qint64 startTimeT = curDateTime.toSecsSinceEpoch() -55;
    qint64 endTimeT = curDateTime.toSecsSinceEpoch() + 5;
    QDateTime startDateTime = QDateTime::fromSecsSinceEpoch(startTimeT);
    QDateTime endDateTime = QDateTime::fromSecsSinceEpoch(endTimeT);
    m_AxisX->setRange(startDateTime, endDateTime);

    pSplineSeriesTemp->append(curDateTime.toMSecsSinceEpoch(), temp);
    pSplineSeriesHumi->append(curDateTime.toMSecsSinceEpoch(), humi);


#if 1
    // 获取 pSplineSeriesTemp 中的点数据
    QVector<QPointF> points = pSplineSeriesTemp->pointsVector();

    // 初始化最大和最小值
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::min();

    // 迭代点数据以更新最大和最小值
    for (const QPointF& point : points)
    {
        // 获取当前点的 Y 值
        double yValue = point.y();
        // 更新最大和最小值
        if (yValue < minY) { minY = yValue; }
        if (yValue > maxY) { maxY = yValue; }
    }

    double range = maxY - minY;
    m_TempY->setRange(minY - margin * range, maxY + margin * range); // 手动调整坐标轴的范围

    // 获取 pSplineSeriesTemp 中的点数据
    QVector<QPointF> points2 = pSplineSeriesHumi->pointsVector();

    // 初始化最大和最小值
    double minY2 = std::numeric_limits<double>::max();
    double maxY2 = std::numeric_limits<double>::min();

    // 迭代点数据以更新最大和最小值
    for (const QPointF& point : points2)
    {
        // 获取当前点的 Y 值
        double yValue = point.y();
        // 更新最大和最小值
        if (yValue < minY2) { minY2 = yValue; }
        if (yValue > maxY2) { maxY2 = yValue; }
    }

    double range2 = maxY2 - minY2;
    m_HumiY->setRange(minY2 - margin * range2, maxY2 + margin * range2); // 手动调整坐标轴的范围

#endif

//    if (temp < minTempY) { minTempY = temp; }
//    if (temp > maxTempY) { maxTempY = temp; }
//    double range = maxTempY - minTempY;
//    m_TempY->setRange(minTempY - margin * range, maxTempY + margin * range); // 手动调整坐标轴的范围


}

void MainWindow::slotSerialReadyRead()
{
    QByteArray array = pSerial->readAll();
#if (0)
    QString str;
    for (int i = 0; i < 12; i++) {
        str += QString("%1 ").arg((unsigned char)array.at(i), 2, 16, QChar('0'));
    }
    ui->textEdit->append("RX --> " + str);
#endif
    if(array.size() != 12) return;
    if(array.at(0) == 0x3A && array.at(11) == 0x0A)
    {
        unsigned short temp = ((unsigned char)array.at(2)) << 8 | (unsigned char)array.at(1);
        unsigned short humi = ((unsigned char)array.at(4)) << 8 | (unsigned char)array.at(3);
//        qDebug() << temp << humi;
        double _temp =  static_cast<double>(temp / 10.0) ;
        double _humi =  static_cast<double>(humi / 10.0) ;

        ui->labelTemp->setText(QString::number(_temp, 'f', 2) + "℃");
        ui->labelHumi->setText(QString::number(_humi, 'f', 2) + "RH");
        appendData(_temp, _humi);

//        array.at(5) > 0 ? ui->labelWindow->setState(eState::Open) : ui->labelWindow->setState(eState::Close);
//        array.at(6) > 0 ? ui->labelWater->setState(eState::Open) : ui->labelWater->setState(eState::Close);
//        array.at(7) > 0 ? ui->labelLed->setState(eState::Open) : ui->labelLed->setState(eState::Close);
//        array.at(8) > 0 ? ui->labelBeep->setState(eState::Open) : ui->labelBeep->setState(eState::Close);

        if(array.at(5) > 0)
        {
            ui->labelWindow->setState(eState::Open);
            ui->btnWindow->setText("关闭");
        }
        else
        {
            ui->labelWindow->setState(eState::Close);
            ui->btnWindow->setText("打开");
        }

        if(array.at(6) > 0)
        {
            ui->labelWater->setState(eState::Open);
            ui->btnWater->setText("关闭");
        }
        else
        {
            ui->labelWater->setState(eState::Close);
            ui->btnWater->setText("打开");
        }

        if(array.at(7) > 0)
        {
            ui->labelLed->setState(eState::Open);
            ui->btnLed->setText("关闭");
        }
        else
        {
            ui->labelLed->setState(eState::Close);
            ui->btnLed->setText("打开");
        }

        if(array.at(8) > 0)
        {
            ui->labelBeep->setState(eState::Open);
            ui->btnBeep->setText("关闭");
        }
        else
        {
            ui->labelBeep->setState(eState::Close);
            ui->btnBeep->setText("打开");
        }
    }
}

#include <QMessageBox>
void MainWindow::on_btnConnect_clicked()
{
    if(ui->btnConnect->text() == "连接")
    {
        if(ui->cBPort->currentText() == nullptr)
        {
            ui->cBPort->clear();
            foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
            {
                ui->cBPort->addItem(info.portName());
            }
            return;
        }

        pSerial->setPortName(ui->cBPort->currentText());
        if(!pSerial->open(QIODevice::ReadWrite))
        {
            QMessageBox::warning(this, "警告", pSerial->errorString());
            return;
        }
        ui->btnConnect->setText("断开");
    }
    else
    {
        if(!pSerial->isOpen()) return;
        pSerial->close();
        ui->btnConnect->setText("连接");
    }
}

void MainWindow::on_btnWindow_clicked()
{
    unsigned char send_msg[12] = {0};
    send_msg[0] = 0x3a;
    send_msg[1] = 0;
    send_msg[2] = 0;
    send_msg[3] = 0;
    send_msg[4] = 0;
    send_msg[5] = ui->labelWindow->state();
    send_msg[6] = ui->labelWater->state();
    send_msg[7] = ui->labelLed->state();
    send_msg[8] = ui->labelBeep->state();
    send_msg[9] = 0;
    send_msg[10] = 0x0D;
    send_msg[11] = 0x0A;

    if(ui->btnWindow->text() == "打开")
    {
//        ui->btnWindow->setText("关闭");
        send_msg[5] = 1;
    }
    else {
//        ui->btnWindow->setText("打开");
        send_msg[5] = 0;
    }
#if (0)
    QString str;
    for (int i = 0; i < 12; i++) {
        str += QString("%1 ").arg(send_msg[i], 2, 16, QChar('0'));
    }
    ui->textEdit->append("TX --> " + str);
#endif
    if(pSerial->isOpen())
        pSerial->write(reinterpret_cast<char *>(send_msg), 12);
}

void MainWindow::on_btnWater_clicked()
{
    unsigned char send_msg[12] = {0};
    send_msg[0] = 0x3a;
    send_msg[1] = 0;
    send_msg[2] = 0;
    send_msg[3] = 0;
    send_msg[4] = 0;
    send_msg[5] = ui->labelWindow->state();
    send_msg[6] = ui->labelWater->state();
    send_msg[7] = ui->labelLed->state();
    send_msg[8] = ui->labelBeep->state();
    send_msg[9] = 0;
    send_msg[10] = 0x0D;
    send_msg[11] = 0x0A;

    if(ui->btnWater->text() == "打开")
    {
//        ui->btnWater->setText("关闭");
        send_msg[6] = 1;
    }
    else {
//        ui->btnWater->setText("打开");
        send_msg[6] = 0;
    }
#if (0)
    QString str;
    for (int i = 0; i < 12; i++) {
        str += QString("%1 ").arg(send_msg[i], 2, 16, QChar('0'));
    }
    ui->textEdit->append("TX --> " + str);
#endif
    if(pSerial->isOpen())
        pSerial->write(reinterpret_cast<char *>(send_msg), 12);
}

void MainWindow::on_btnLed_clicked()
{
    unsigned char send_msg[12] = {0};
    send_msg[0] = 0x3a;
    send_msg[1] = 0;
    send_msg[2] = 0;
    send_msg[3] = 0;
    send_msg[4] = 0;
    send_msg[5] = ui->labelWindow->state();
    send_msg[6] = ui->labelWater->state();
    send_msg[7] = ui->labelLed->state();
    send_msg[8] = ui->labelBeep->state();
    send_msg[9] = 0;
    send_msg[10] = 0x0D;
    send_msg[11] = 0x0A;

    if(ui->btnLed->text() == "打开")
    {
//        ui->btnLed->setText("关闭");
        send_msg[7] = 1;
    }
    else {
//        ui->btnLed->setText("打开");
        send_msg[7] = 0;
    }
#if (0)
    QString str;
    for (int i = 0; i < 12; i++) {
        str += QString("%1 ").arg(send_msg[i], 2, 16, QChar('0'));
    }
    ui->textEdit->append("TX --> " + str);
#endif
    if(pSerial->isOpen())
        pSerial->write(reinterpret_cast<char *>(send_msg), 12);
}

void MainWindow::on_btnBeep_clicked()
{
    unsigned char send_msg[12] = {0};
    send_msg[0] = 0x3a;
    send_msg[1] = 0;
    send_msg[2] = 0;
    send_msg[3] = 0;
    send_msg[4] = 0;
    send_msg[5] = ui->labelWindow->state();
    send_msg[6] = ui->labelWater->state();
    send_msg[7] = ui->labelLed->state();
    send_msg[8] = ui->labelBeep->state();
    send_msg[9] = 0;
    send_msg[10] = 0x0D;
    send_msg[11] = 0x0A;

    if(ui->btnBeep->text() == "打开")
    {
//        ui->btnBeep->setText("关闭");
        send_msg[8] = 1;
    }
    else {
//        ui->btnBeep->setText("打开");
        send_msg[8] = 0;
    }
#if (0)
    QString str;
    for (int i = 0; i < 12; i++) {
        str += QString("%1 ").arg(send_msg[i], 2, 16, QChar('0'));
    }
    ui->textEdit->append("TX --> " + str);
#endif
    if(pSerial->isOpen())
        pSerial->write(reinterpret_cast<char *>(send_msg), 12);
}
