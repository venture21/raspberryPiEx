#include <QApplication>
#include <QPushButton>

#include "CustomWidget.h"

CustomWidget::CustomWidget(QWidget* parent) : QWidget(parent)
{
    QPushButton *button = new QPushButton("Quit", this);
    button->resize(150,50);

    // Widget 사이즈 설정
    this->resize(150,50);
    move (300,300);

    //connect(button, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(button, SIGNAL(clicked()), SLOT(processClick()));
}

void CustomWidget::processClick()
{
    emit widgetClicked();
}
