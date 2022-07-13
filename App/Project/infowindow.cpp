#include "infowindow.h"
#include "ui_infowindow.h"

InfoWindow::InfoWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoWindow)
{
    ui->setupUi(this);
}

InfoWindow::~InfoWindow()
{
    delete ui;
}

void InfoWindow::UpdateInfo(QString name, QString type)
{
    if(type == "Product"){
        QString date1, date2, date3;
        date1 = QDateTime::currentDateTime().addMonths(-1).toString();
        date2 = QDateTime::currentDateTime().toString();
        date3 = QDateTime::currentDateTime().addMonths(-2).toString();
        double tent1, tent2;
        QSqlQuery query("SELECT cat.\"name\", "
                        "count_sale_product(prod.\"name\", \'" + date1 + "\', \'" + date2 + "\') as cspNow, "
                        "income_with_product(prod.\"name\", \'" + date1 + "\', \'" + date2 + "\') as iwpNow, "
                        "count_sale_product(prod.\"name\", \'" + date3 + "\', \'" + date1 + "\') as cspOld, "
                        "income_with_product(prod.\"name\", \'" + date3 + "\', \'" + date1 + "\') as iwpOld, "
                        "prod.* "
                        "From \"Product\" as prod "
                        "Join \"Product_Category\" as cat on prod.\"PK_product_category\"=cat.\"PK_product_category\""
                        "Where prod.\"name\"=\'" + name + "\'");
        query.next();
        ui->NameLabel->setText(query.value(6).toString());
        ui->TypeLabel->setText(query.value(0).toString());
        ui->PriceLabel->setText(query.value(7).toString());

    }
}
