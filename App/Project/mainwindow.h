#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDateTime>
#include <QListWidgetItem>
#include <infowindow.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_GoodsButton_clicked();

    void on_CheckButton_clicked();

    void on_SortButton_clicked();

    void on_FiltButton_clicked();

    void on_ShopButton_clicked();

    void GoodsShow(QString filt, QString sort);

    void CheckShow(QString filt, QString sort);

    void ShopShow(QString sort);

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QString OpenedTabel;
    QString Filt;
};
#endif // MAINWINDOW_H
