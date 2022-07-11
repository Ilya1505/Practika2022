/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *CheckButton;
    QListWidget *listWidget;
    QComboBox *SortBox;
    QPushButton *SortButton;
    QPushButton *GoodsButton;
    QPushButton *ShopButton;
    QPushButton *FiltButton;
    QLineEdit *FiltBox;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        CheckButton = new QPushButton(centralwidget);
        CheckButton->setObjectName(QString::fromUtf8("CheckButton"));
        CheckButton->setGeometry(QRect(11, 11, 111, 29));
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(10, 100, 771, 441));
        SortBox = new QComboBox(centralwidget);
        SortBox->setObjectName(QString::fromUtf8("SortBox"));
        SortBox->setGeometry(QRect(10, 50, 121, 26));
        SortButton = new QPushButton(centralwidget);
        SortButton->setObjectName(QString::fromUtf8("SortButton"));
        SortButton->setGeometry(QRect(190, 50, 99, 29));
        GoodsButton = new QPushButton(centralwidget);
        GoodsButton->setObjectName(QString::fromUtf8("GoodsButton"));
        GoodsButton->setGeometry(QRect(340, 10, 93, 29));
        ShopButton = new QPushButton(centralwidget);
        ShopButton->setObjectName(QString::fromUtf8("ShopButton"));
        ShopButton->setGeometry(QRect(690, 11, 93, 29));
        FiltButton = new QPushButton(centralwidget);
        FiltButton->setObjectName(QString::fromUtf8("FiltButton"));
        FiltButton->setGeometry(QRect(690, 47, 99, 29));
        FiltBox = new QLineEdit(centralwidget);
        FiltBox->setObjectName(QString::fromUtf8("FiltBox"));
        FiltBox->setGeometry(QRect(410, 50, 161, 26));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        CheckButton->setText(QCoreApplication::translate("MainWindow", "\320\247\320\265\320\272\320\270", nullptr));
        SortButton->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\200\321\202\320\270\321\200\320\276\320\262\320\260\321\202\321\214", nullptr));
        GoodsButton->setText(QCoreApplication::translate("MainWindow", "\320\242\320\276\320\262\320\260\321\200\321\213", nullptr));
        ShopButton->setText(QCoreApplication::translate("MainWindow", "\320\234\320\260\320\263\320\260\320\267\320\270\320\275\321\213", nullptr));
        FiltButton->setText(QCoreApplication::translate("MainWindow", "\320\244\320\270\320\273\321\214\321\202\321\200\320\276\320\262\320\260\321\202\321\214", nullptr));
        FiltBox->setText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\274\320\260\320\263\320\260\320\267\320\270\320\275", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
