#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("Shop");
    db.setUserName("postgres");
    db.setPassword("1324");
    bool ok = db.open();
    if(ok){
        on_CheckButton_clicked();
    }
    else{
        QMessageBox msBox;
        msBox.setText("Не получилось открыть базу данных");
        msBox.setWindowTitle("Ошибка");
        msBox.exec();
        this->setAttribute(Qt::WA_DeleteOnClose);
        this->close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_GoodsButton_clicked()
{
    GoodsShow("", "");
    OpenedTabel = "Product";
    ui->SortBox->clear();
    ui->SortBox->addItem("Цена");
    ui->SortBox->addItem("Продажи за месяц");
    ui->SortBox->addItem("Прибыль за месяц");
    ui->SortBox->addItem("Изменение продаж");
    ui->SortBox->addItem("Изменение прибыли");
    ui->FiltBox->setEnabled(1);
    ui->FiltButton->setEnabled(1);
    ui->FiltBox->setText("Введите категорию");
    Filt = "";
}


void MainWindow::on_CheckButton_clicked()
{
    CheckShow("", "");
    OpenedTabel = "Receipt";
    ui->SortBox->clear();
    ui->SortBox->addItem("Дата");
    ui->SortBox->addItem("Сумма");
    ui->FiltBox->setEnabled(1);
    ui->FiltButton->setEnabled(1);
    ui->FiltBox->setText("Введите магазин");
    Filt = "";
}

void MainWindow::on_ShopButton_clicked()
{
    ShopShow("");
    OpenedTabel = "Shop";
    ui->SortBox->clear();
    ui->SortBox->addItem("Покупателей за месяц");
    ui->SortBox->addItem("Прибыль за месяц");
    ui->SortBox->addItem("Изменение покупателей");
    ui->SortBox->addItem("Изменение прибыли");
    ui->FiltBox->setDisabled(1);
    ui->FiltButton->setDisabled(1);
    Filt = "";
}

void MainWindow::on_SortButton_clicked()
{
    QString sort("");
    QString filt("");
    if(OpenedTabel == "Receipt"){
        QString sort("");
        if(ui->SortBox->currentIndex() == 0){
            sort = "Order by rep.\"date_receipt\", rep.\"time_receipt\"";
        }
        else if(ui->SortBox->currentIndex() == 1){
            sort = "Order by prwd";
        }
        if(Filt != ""){
            filt = "Where sh.\"address\"=\'" + Filt + "\'";
        }
        CheckShow(filt, sort);
    }
    else if(OpenedTabel == "Product"){
        if(Filt != ""){
            filt = "Where cat.\"name\"=\'" + Filt + "\'";
        }
        if(ui->SortBox->currentIndex() == 0){
            sort = "Order by prod.\"price_without_discount\"";
        }
        if(ui->SortBox->currentIndex() == 1){
            sort = "Order by cspNow";
        }
        if(ui->SortBox->currentIndex() == 2){
            sort = "Order by iwpNow";
        }
        GoodsShow(filt, sort);
    }
    else if(OpenedTabel == "Shop"){
        if(ui->SortBox->currentIndex() == 0){
            sort = "Order by crwsNow";
        }
        if(ui->SortBox->currentIndex() == 1){
            sort = "Order by siNow";
        }
        ShopShow(sort);
    }
}


void MainWindow::on_FiltButton_clicked()
{
    if(OpenedTabel == "Product"){
        GoodsShow("Where cat.\"name\"=\'" + ui->FiltBox->text() + "\'", "");
        Filt = ui->FiltBox->text();
    }
    else if(OpenedTabel == "Receipt"){
        CheckShow("Where sh.\"address\"=\'" + ui->FiltBox->text() + "\'", "");
        Filt = ui->FiltBox->text();
    }
}

void MainWindow::GoodsShow(QString filt, QString sort)
{
    ui->listWidget->clear();
    QString date1, date2, date3;
    //date1 = QDateTime::currentDateTime().addMonths(-1).toString();
    //date2 = QDateTime::currentDateTime().toString();
    //date3 = QDateTime::currentDateTime().addMonths(-2).toString();
    date1 = "2022-05-02";
    date2 = "2022-06-02";
    date3 = "2022-04-02";
    double tent1, tent2;
    QSqlQuery query("SELECT cat.\"name\", "
                    "count_sale_product(prod.\"name\", \'" + date1 + "\', \'" + date2 + "\') as cspNow, "
                    "income_with_product(prod.\"name\", \'" + date1 + "\', \'" + date2 + "\') as iwpNow, "
                    "count_sale_product(prod.\"name\", \'" + date3 + "\', \'" + date1 + "\') as cspOld, "
                    "income_with_product(prod.\"name\", \'" + date3 + "\', \'" + date1 + "\') as iwpOld, "
                    "prod.* "
                    "From \"Product\" as prod "
                    "Join \"Product_Category\" as cat on prod.\"PK_product_category\"=cat.\"PK_product_category\"" + filt + sort);
    while(query.next()){
        tent1 = query.value(3).toDouble();
        if(tent1 == 0) tent1++;
        tent1 = ((query.value(1).toDouble() - query.value(3).toDouble())/tent1) * 100;
        tent2 = query.value(4).toDouble();
        if(tent2 == 0) tent2++;
        tent2 =  ((query.value(2).toDouble() - query.value(4).toDouble())/tent2) * 100;
        ui->listWidget->addItem(query.value(6).toString() + "\n" +
                                "Категория: " + query.value(0).toString() + " | "
                                "Продано за месяц: " + query.value(1).toString() + " " + query.value(8).toString() + " | "
                                "Сравнение с прошлым месяцем: " + QString::number(tent1) + + "\%\n" +
                                "Цена: " + query.value(7).toString() + " руб." + " | "
                                "Заработано за месяц: " + query.value(2).toString() + " | "
                                "Сравнение с прошлым месяцем: " + QString::number(tent2) + "\%");
    }
}

void MainWindow::CheckShow(QString filt, QString sort)
{
    ui->listWidget->clear();
    QSqlQuery query("SELECT price_receipt_with_discount(rep.\"PK_receipt\") as prwd, "
                    "sh.\"address\" , rep.* "
                    "From \"Receipt\" as rep "
                    "Join \"Shop\" as sh on rep.\"PK_shop\"=sh.\"PK_shop\" " + filt + sort);
    while(query.next()){
        ui->listWidget->addItem("Дата и время: " + query.value(3).toString() + " " + query.value(4).toString() + "\n" +
                                "Итоговая Сумма: " + query.value(0).toString() + "\n" +
                                "Магазин: " + query.value(1).toString());
    }
}

void MainWindow::ShopShow(QString sort)
{
    ui->listWidget->clear();
    QString date1, date2, date3;
    date1 = QDateTime::currentDateTime().addMonths(-1).toString();
    date2 = QDateTime::currentDateTime().toString();
    date3 = QDateTime::currentDateTime().addMonths(-2).toString();
    double tent1, tent2;
    QSqlQuery query("Select count_receipt_with_shop(sh.\"address\", \'" + date1 + "\', \'" + date2 + "\') as crwsNow, "
                    "store_income(sh.\"address\", \'" + date1 + "\', \'" + date2 + "\') as siNow, "
                    "count_receipt_with_shop(sh.\"address\", \'" + date3 + "\', \'" + date1 + "\') as crwsOld, "
                    "store_income(sh.\"address\", \'" + date3 + "\', \'" + date1 + "\') as siOld, sh.* "
                    "From \"Shop\" as sh " + sort);
    while(query.next()){
        tent1 = query.value(2).toDouble();
        if(tent1 == 0) tent1++;
        tent1 = ((query.value(0).toDouble() - query.value(2).toDouble())/tent1) * 100;
        tent2 = query.value(3).toDouble();
        if(tent2 == 0) tent2++;
        tent2 =  ((query.value(1).toDouble() - query.value(3).toDouble())/tent2) * 100;
        ui->listWidget->addItem("Адрес: " + query.value(6).toString() + "\n" +
                                "Покупателей за месяц: " + query.value(0).toString() + " | "
                                "Сравнение с прошлым месяцем: " + QString::number(tent1) + + "\%\n" +
                                "Прибыль за месяц: " + query.value(1).toString() + " | "
                                "Сравнение с прошлым месяцем: " + QString::number(tent2) + + "\%");
    }
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if(OpenedTabel == "Product"){
        QStringList ddd = item->text().split("\n");
        ddd = ddd.value(0).split(" ");
        QString name = ddd.value(1);
        InfoWindow *Form = new InfoWindow;
        Form->exec();

    }

}

