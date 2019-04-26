#include <QApplication>
#include <QTextBrowser>
#include <QUrl>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QUrl url("index.html");   // 파일이 별도로 있어야 한다.
    QTextBrowser *textbrowser = new QTextBrowser();
    textbrowser->setSource(url);
    textbrowser->show();

    return a.exec();
}
