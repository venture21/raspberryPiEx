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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *labelOp;
    QLabel *labelOpStatus;
    QPushButton *btnLEDON;
    QPushButton *btnLEDOFF;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(260, 167);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        labelOp = new QLabel(centralWidget);
        labelOp->setObjectName(QStringLiteral("labelOp"));
        labelOp->setGeometry(QRect(30, 20, 201, 21));
        labelOp->setAlignment(Qt::AlignCenter);
        labelOpStatus = new QLabel(centralWidget);
        labelOpStatus->setObjectName(QStringLiteral("labelOpStatus"));
        labelOpStatus->setGeometry(QRect(30, 40, 201, 21));
        labelOpStatus->setAlignment(Qt::AlignCenter);
        btnLEDON = new QPushButton(centralWidget);
        btnLEDON->setObjectName(QStringLiteral("btnLEDON"));
        btnLEDON->setGeometry(QRect(30, 70, 91, 31));
        btnLEDOFF = new QPushButton(centralWidget);
        btnLEDOFF->setObjectName(QStringLiteral("btnLEDOFF"));
        btnLEDOFF->setGeometry(QRect(130, 70, 101, 31));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 260, 27));
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
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "LED Control", Q_NULLPTR));
        labelOp->setText(QApplication::translate("MainWindow", "Operation", Q_NULLPTR));
        labelOpStatus->setText(QApplication::translate("MainWindow", "Status", Q_NULLPTR));
        btnLEDON->setText(QApplication::translate("MainWindow", "LEDON", Q_NULLPTR));
        btnLEDOFF->setText(QApplication::translate("MainWindow", "LEDOFF", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
