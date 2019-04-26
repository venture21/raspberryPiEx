#include "comboboxdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ComboBoxDialog w;
    w.show();

    return a.exec();
}
