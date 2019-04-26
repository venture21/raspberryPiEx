#include <QApplication>
#include <QLineEdit>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    QLineEdit *lineEdit1 = new QLineEdit(0);
//    //lineEdit1->setEchoMode(QLineEdit::Normal);
//    lineEdit1->show();

//    QLineEdit *lineEdit2 = new QLineEdit(0);
//    lineEdit2->setEchoMode(QLineEdit::NoEcho);
//    lineEdit2->show();

//    QLineEdit *lineEdit3 = new QLineEdit(0);
//    lineEdit3->setEchoMode(QLineEdit::Password);
//    lineEdit3->show();

//    QLineEdit *lineEdit4 = new QLineEdit(0);
//    lineEdit4->setEchoMode(QLineEdit::PasswordEchoOnEdit);
//    lineEdit4->show();

    return a.exec();
}
