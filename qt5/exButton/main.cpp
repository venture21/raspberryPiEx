#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <QPushButton>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // <i></i> 는 이태릭체
    QLabel *label = new QLabel("<font color=blue>Hello <i>World!</i></font>",0);
    label->resize(150,50);
    label->move(50,300);
    label->show();

    QPushButton *button1 = new QPushButton("Button1", 0);
    button1->resize(150,50);
    button1->move(200,300);
    // sender   : 시그널 을발생시 킬객체 의포인터를 지정
    // signal   : sender가 발생시키 는시그널 을지정
    // receiver : 시그널 을 받을 객체의 포인터를 지정한다.
    // method   : receiver에 정의된 슬롯을 지정한다.
    // button1이 clicked되었을 때 Application a 가 수신하고
    // 이에 대응하는 quit()함수가 실행된다.
    QObject::connect(button1, SIGNAL(clicked()),&a, SLOT(quit()));
    button1->show();

    //MainWindow w;
    //w.show();

    return a.exec();
}
