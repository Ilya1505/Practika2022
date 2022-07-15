#include "checkform.h"
#include "ui_checkform.h"

CheckForm::CheckForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CheckForm)
{
    ui->setupUi(this);
}

CheckForm::~CheckForm()
{
    delete ui;
}

void CheckForm::UpdateInfo(QString date, QString shop)
{
    QSqlQuery query("SELECT price_receipt_with_discount(rep.\"PK_receipt\") as prwd, "
                    "price_receipt(rep.\"PK_receipt\") as pr, "
                    "sh.\"address\" , rep.* "
                    "From \"Receipt\" as rep "
                    "Join \"Shop\" as sh on rep.\"PK_shop\"=sh.\"PK_shop\" "
                    "Where rep.\"date_receipt\"='" + date.split(" ").value(0) + "' and "
                    "rep.\"time_receipt\"='" + date.split(" ").value(1) + "' and sh.\"address\"='" + shop + "'");
    query.next();
    int rep;
    rep = query.value(3).toInt();
    ui->dateLabel->setText("Дата и время: " + query.value(4).toString() + " " + query.value(5).toString());
    ui->shopLabel->setText("Магазин: " + shop);
    ui->discountLabel->setText("Итоговая сумма: " + query.value(0).toString() + " руб.");
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setColumnCount(3);
    QStringList header;
    header << "Товар" << "Количество" << "Сумма";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    query.exec("Select prod.\"name\", str.* "
               "From \"String_Receipt\" as str "
               "Join \"Product\" as prod on prod.\"PK_product\"=str.\"PK_product\" "
               "Where str.\"PK_receipt\"=" + QString::number(rep));
    int i;
    i = 0;
    while(query.next()){
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(query.value(2).toString()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(query.value(7).toString() + " руб."));
        i++;
    }
}
