#ifndef INFOWINDOW_H
#define INFOWINDOW_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>

namespace Ui {
class InfoWindow;
}

class InfoWindow : public QDialog
{
    Q_OBJECT

public:
    InfoWindow(QWidget *parent = nullptr);
    ~InfoWindow();
    void UpdateInfo(QString name, QString type);

private:
    Ui::InfoWindow *ui;
};

#endif // INFOWINDOW_H
