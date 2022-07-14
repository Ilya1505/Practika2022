#ifndef CHECKFORM_H
#define CHECKFORM_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class CheckForm;
}

class CheckForm : public QDialog
{
    Q_OBJECT

public:
    explicit CheckForm(QWidget *parent = nullptr);
    ~CheckForm();
    void UpdateInfo(QString date, QString shop);

private:
    Ui::CheckForm *ui;
};

#endif // CHECKFORM_H
