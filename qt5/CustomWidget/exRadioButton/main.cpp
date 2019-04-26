#include <QApplication>
#include <QWidget>
#include <QRadioButton>
#include <QButtonGroup>

void DrawRadioButton();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DrawRadioButton();

    return a.exec();
}


void DrawRadioButton()
{
    QWidget *widget = new QWidget(0);
    QButtonGroup *bg = new QButtonGroup(widget);

    // &1 : & 를 붙일 경우 ALT + 1 을 단축키로 지정 할 수 있다.
    QRadioButton *rb1 = new QRadioButton("라디오 옵션 &1", widget);
    rb1->move(10,10);
    bg->addButton(rb1);

    QRadioButton *rb2 = new QRadioButton("라디오 옵션 &2", widget);
    rb2->move(10,30);
    bg->addButton(rb2);

    QRadioButton *rb3 = new QRadioButton("라디오 옵션 &3", widget);
    rb3->move(10,50);
    bg->addButton(rb3);

    QRadioButton *rb4 = new QRadioButton("라디오 옵션 &4", widget);
    rb4->move(10,70);
    bg->addButton(rb4);

    widget->resize(150,110);
    widget->show();
}
