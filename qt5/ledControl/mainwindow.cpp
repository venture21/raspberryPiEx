#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <wiringPi.h>
#include "gpioled.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnLEDON_clicked()
{
    GpioLED gpioLED1;
    gpioLED1.ledCTRL(1, HIGH);
    QString strOperation, strLedStatus;
    strOperation.sprintf("LED Control");
    ui->labelOp->setText(strOperation);
    strLedStatus.sprintf("LED : ON");
    ui->labelOpStatus->setText(strLedStatus);
}

void MainWindow::on_btnLEDOFF_clicked()
{
    GpioLED gpioLED1;
    gpioLED1.ledCTRL(1, LOW);
    QString strOperation, strLedStatus;
    strOperation.sprintf("LED Control");
    ui->labelOp->setText(strOperation);
    strLedStatus.sprintf("LED : OFF");
    ui->labelOpStatus->setText(strLedStatus);
}



