/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLineEdit *lineEdit;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QLabel *label_3;
    QLineEdit *lineEdit_3;
    QLabel *label_4;
    QLineEdit *lineEdit_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *labelOutput1;
    QLabel *labelOutput2;
    QLabel *labelOutput3;
    QLabel *labelOutput4;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(243, 365);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(100, 10, 113, 33));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 91, 31));
        label->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 50, 91, 31));
        label_2->setAlignment(Qt::AlignCenter);
        lineEdit_2 = new QLineEdit(centralWidget);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(100, 50, 113, 33));
        lineEdit_2->setEchoMode(QLineEdit::NoEcho);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 130, 91, 31));
        label_3->setAlignment(Qt::AlignCenter);
        lineEdit_3 = new QLineEdit(centralWidget);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(100, 90, 113, 33));
        lineEdit_3->setEchoMode(QLineEdit::PasswordEchoOnEdit);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 90, 91, 31));
        label_4->setAlignment(Qt::AlignCenter);
        lineEdit_4 = new QLineEdit(centralWidget);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(100, 130, 113, 33));
        lineEdit_4->setEchoMode(QLineEdit::Password);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 180, 81, 21));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 210, 81, 21));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(20, 240, 81, 21));
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(20, 270, 81, 21));
        labelOutput1 = new QLabel(centralWidget);
        labelOutput1->setObjectName(QStringLiteral("labelOutput1"));
        labelOutput1->setGeometry(QRect(100, 180, 111, 21));
        labelOutput2 = new QLabel(centralWidget);
        labelOutput2->setObjectName(QStringLiteral("labelOutput2"));
        labelOutput2->setGeometry(QRect(100, 210, 111, 21));
        labelOutput3 = new QLabel(centralWidget);
        labelOutput3->setObjectName(QStringLiteral("labelOutput3"));
        labelOutput3->setGeometry(QRect(100, 240, 111, 21));
        labelOutput4 = new QLabel(centralWidget);
        labelOutput4->setObjectName(QStringLiteral("labelOutput4"));
        labelOutput4->setGeometry(QRect(100, 270, 111, 21));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 243, 27));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "TextBrowser", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "LineEdit1", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "LineEdit2", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "LineEdit4", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "LineEdit3", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "Output1", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "Output2", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "Output3", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "Output4", Q_NULLPTR));
        labelOutput1->setText(QString());
        labelOutput2->setText(QString());
        labelOutput3->setText(QString());
        labelOutput4->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
