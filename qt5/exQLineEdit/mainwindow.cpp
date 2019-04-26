#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::on_lineEdit_editingFinished()
{
    QString str = ui->lineEdit->text();
    ui->labelOutput1->setText(str);
}

void MainWindow::on_lineEdit_2_editingFinished()
{
    QString str = ui->lineEdit_2->text();
    ui->labelOutput2->setText(str);
}

void MainWindow::on_lineEdit_3_editingFinished()
{
    QString str = ui->lineEdit_3->text();
    ui->labelOutput3->setText(str);
}

void MainWindow::on_lineEdit_4_editingFinished()
{
    QString str = ui->lineEdit_4->text();
    ui->labelOutput4->setText(str);
}
