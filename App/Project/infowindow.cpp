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
    QVector<double> x1, y1, x2, y2;
    QVector<double> ticks;
    QVector<QString> labels;
    if(type == "Product"){
        QString date1, date2, date3;
        QSqlQuery query("SELECT cat.\"name\", "
                        "prod.* "
                        "From \"Product\" as prod "
                        "Join \"Product_Category\" as cat on prod.\"PK_product_category\"=cat.\"PK_product_category\""
                        "Where prod.\"name\"=\'" + name + "\'");
        query.next();
        ui->NameLabel->setText("Название: " + query.value(2).toString());
        ui->TypeLabel->setText("Категория: " + query.value(0).toString());
        ui->PriceLabel->setText("Цена: " + query.value(3).toString() + " руб.");
        ui->firstGraphLabel->setText("Продажи");
        ui->SecondGraphLabel->setText("Прибыль");
        for(int i = 0; i < 12; i++){
            date1 = QDateTime::currentDateTime().addMonths(-12 + i).toString();
            date2 = QDateTime::currentDateTime().addMonths(-12 + i + 1).toString();
            ticks.push_back(i + 1);
            labels.push_back(date1.split(" ").value(1) + "\n" + date1.split(" ").value(4));
            query.clear();
            query.exec("Select "
                  "count_sale_product(prod.\"name\", \'" + date1 + "\', \'" + date2 + "\') as cspNow, "
                  "income_with_product(prod.\"name\", \'" + date1 + "\', \'" + date2 + "\') as iwpNow, "
                  "prod.* "
                  "From \"Product\" as prod "
                  "Where prod.\"name\"=\'" + name + "\'");
            query.next();
            x1.push_back(i + 1);
            y1.push_back(query.value(0).toDouble());
            x2.push_back(i + 1);
            y2.push_back(query.value(1).toDouble());
        }
    }
    else {
        QString date1, date2, date3;
        QSqlQuery query("SELECT sho.* "
                        "From \"Shop\" as sho "
                        "Where sho.\"address\"=\'" + name + "\'");
        query.next();
        ui->NameLabel->setText(query.value(2).toString());
        ui->TypeLabel->setText("");
        ui->PriceLabel->setText("");
        ui->firstGraphLabel->setText("Покупатели");
        ui->SecondGraphLabel->setText("Прибыль");
        for(int i = 0; i < 12; i++){
            date1 = QDateTime::currentDateTime().addMonths(-12 + i).toString();
            date2 = QDateTime::currentDateTime().addMonths(-12 + i + 1).toString();
            ticks.push_back(i + 1);
            labels.push_back(date1.split(" ").value(1) + "\n" + date1.split(" ").value(4));
            query.clear();
            query.exec("Select "
                  "count_receipt_with_shop(sh.\"address\", \'" + date1 + "\', \'" + date2 + "\') as crwsNow, "
                  "store_income(sh.\"address\", \'" + date1 + "\', \'" + date2 + "\') as siNow "
                  "From \"Shop\" as sh "
                  "Where sh.\"address\"=\'" + name + "\'");
            query.next();
            x1.push_back(i + 1);
            y1.push_back(query.value(0).toDouble());
            x2.push_back(i + 1);
            y2.push_back(query.value(1).toDouble());
        }
    }
    QSharedPointer<QCPAxisTickerText> tex(new QCPAxisTickerText);
    tex->addTicks(ticks, labels);
    ui->FirstGraph->xAxis->setTicker(tex);
    ui->FirstGraph->addGraph();
    ui->FirstGraph->graph(0)->addData(x1, y1);
    ui->FirstGraph->yAxis->rescale();
    ui->FirstGraph->graph(0)->rescaleAxes();
    ui->FirstGraph->replot();
    ui->SecondGraph->xAxis->setTicker(tex);
    ui->SecondGraph->addGraph();
    ui->SecondGraph->graph(0)->addData(x2, y2);
    ui->SecondGraph->graph(0)->rescaleAxes();
    ui->SecondGraph->replot();
}
