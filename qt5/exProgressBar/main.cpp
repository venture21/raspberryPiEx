#include <unistd.h>
#include <QApplication>
#include <QProgressBar>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QProgressBar *progressBar = new QProgressBar();
    progressBar->setRange(0,100);
    progressBar->setValue(0);
    progressBar->show();

    return a.exec();
}
