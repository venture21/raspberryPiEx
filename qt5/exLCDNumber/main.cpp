//==========================================================
// 오늘 날짜를 읽어와서 QLCDNumber에 출력하는 예제
//==========================================================

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <QApplication>
#include <QLCDNumber>
#include <QPalette>
#include <QLabel>

#define BUFSIZE 64

//============================
// 날짜와 시간을 저장하는 구조체
//============================
//struct tm {
//   int tm_sec;         /* seconds,  range 0 to 59          */
//   int tm_min;         /* minutes, range 0 to 59           */
//   int tm_hour;        /* hours, range 0 to 23             */
//   int tm_mday;        /* day of the month, range 1 to 31  */
//   int tm_mon;         /* month, range 0 to 11             */
//   int tm_year;        /* The number of years since 1900   */
//   int tm_wday;        /* day of the week, range 0 to 6    */
//   int tm_yday;        /* day in the year, range 0 to 365  */
//   int tm_isdst;       /* daylight saving time             */
//};


int main(int argc, char *argv[])
{
    time_t UTCtime;
    struct tm *tm;
    char sDate[BUFSIZE];
    char sTime[BUFSIZE];
    time(&UTCtime);        // UTC 현재 시간 구하기
    tm = gmtime(&UTCtime);

    sprintf(sDate, "  %4d.%2d.%2d", 1900+(tm->tm_year), 1+(tm->tm_mon), tm->tm_mday);
    sprintf(sTime, "  %2d:%02d", (tm->tm_hour+9)%24,tm->tm_min);
    printf("%s\n", sDate);
    //printf(" ctime : %s\n",ctime(&UTCtime));

    QApplication a(argc, argv);
    QWidget* widget = new QWidget(0);

//    QLabel *label = new QLabel("test label",0);
//    label->resize(300, 50);
//    label->move(100,100);
//    label->setText(sTime);
//    label->show();

    QPalette *palette = new QPalette();
    palette->setColor(QPalette::WindowText,Qt::blue);


    // Digit  : 년
    QLCDNumber *lcd1 = new QLCDNumber(widget);
    lcd1->setSegmentStyle(QLCDNumber::Flat);
    lcd1->display(1900+(tm->tm_year));
    lcd1->setDigitCount(4);        // 4자리
    lcd1->setPalette(*palette);    // 색상 변경
    lcd1->setFixedSize(150,50);    // QLCDNumber 크기 변경
    lcd1->move(10,10);


    // Digit  : 월
    QLCDNumber *lcd2 = new QLCDNumber(widget);
    lcd2->setSegmentStyle(QLCDNumber::Flat);
    lcd2->display(1+(tm->tm_mon));
    lcd2->setDecMode();        // 10진수
    lcd2->setDigitCount(2);    // 2자리
    lcd2->setPalette(*palette);    // 색상 변경
    lcd2->setFixedSize(80,50);
    lcd2->move(170,10);


    // Digit : 일
    QLCDNumber *lcd3 = new QLCDNumber(widget);
    lcd3->setSegmentStyle(QLCDNumber::Flat);
    lcd3->display(tm->tm_mday);
    lcd3->setDecMode();        // 10진수
    lcd3->setDigitCount(2);    // 2자리
    lcd3->setPalette(*palette);    // 색상 변경
    lcd3->setFixedSize(80,50);
    lcd3->move(260,10);
    widget->resize(350,150);
    widget->show();

    return a.exec();
}
