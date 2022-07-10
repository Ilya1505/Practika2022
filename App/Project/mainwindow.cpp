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
    ui->listWidget->clear();
    QString date1, date2;
    date1 = QDateTime::currentDateTime().addMonths(-1).toString();
    date2 = QDateTime::currentDateTime().toString();
    QSqlQuery query("SELECT cat.\"name\", "
                    "count_sale_product(prod.\"name\", \'" + date1 + "\', \'" + date2 + "\'), "
                    "income_with_product(prod.\"name\", \'" + date1 + "\', \'" + date2 + "\'), "
                    "prod.* "
                    "From \"Product\" as prod "
                    "Join \"Product_Category\" as cat on prod.\"PK_product_category\"=cat.\"PK_product_category\"");
    while(query.next()){
        ui->listWidget->addItem(query.value(4).toString() + "\n" +
                                "Категория: " + query.value(0).toString() +
                                " Продано за месяц: " + query.value(1).toString() + " " + query.value(6).toString() + "\n" +
                                "Цена: " + query.value(5).toString() + " руб." +
                                " Заработано за месяц: " + query.value(2).toString());
    }
    OpenedTabel = "Product";
    ui->SortBox->clear();
    ui->SortBox->addItem("Цена");
    ui->FiltBox->setEnabled(1);
    ui->FiltButton->setEnabled(1);
    ui->FiltBox->setText("Введите категорию");
    Filt = "";
}


void MainWindow::on_CheckButton_clicked()
{
    ui->listWidget->clear();
    QSqlQuery query("SELECT price_receipt_with_discount(rep.\"PK_receipt\") , sh.\"address\" , rep.* "
                    "From \"Receipt\" as rep "
                    "Join \"Shop\" as sh on rep.\"PK_shop\"=sh.\"PK_shop\"");
    while(query.next()){
        ui->listWidget->addItem("Дата и время: " + query.value(3).toString() + " " + query.value(4).toString() + "\n" +
                                "Сумма: " + query.value(0).toString() + "\n" +
                                "Магазин: " + query.value(1).toString());
    }
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
    ui->listWidget->clear();
    QString date1, date2;
    date1 = QDateTime::currentDateTime().addMonths(-1).toString();
    date2 = QDateTime::currentDateTime().toString();
    QSqlQuery query("Select count_receipt_with_shop(sh.\"address\", \'" + date1 + "\', \'" + date2 + "\'), "
                    "store_income(sh.\"address\", \'" + date1 + "\', \'" + date2 + "\'), sh.* "
                    "From \"Shop\" as sh");
    while(query.next()){
        ui->listWidget->addItem("Адрес: " + query.value(4).toString() + "\n" +
                                "Покупателей за месяц: " + query.value(0).toString() + "\n" +
                                "Прибыль за месяц: " + query.value(1).toString());
    }
    OpenedTabel = "Shop";
    ui->SortBox->clear();
    ui->FiltBox->setDisabled(1);
    ui->FiltButton->setDisabled(1);
    Filt = "";
}

void MainWindow::on_SortButton_clicked()
{
    if(OpenedTabel == "Receipt"){
        ui->listWidget->clear();
        QSqlQuery query;
        QString dop("");
        if(Filt != ""){
            dop = "Where sh.\"address\"=\'" + Filt + "\'";
        }
        if(ui->SortBox->currentIndex() == 0){
            query.exec("SELECT price_receipt_with_discount(rep.\"PK_receipt\") , sh.\"address\" , rep.* "
                       "From \"Receipt\" as rep "
                       "Join \"Shop\" as sh on rep.\"PK_shop\"=sh.\"PK_shop\" " + dop +
                       "Order by rep.\"date_receipt\", rep.\"time_receipt\"");
        }
        if(ui->SortBox->currentIndex() == 1){
            query.exec("SELECT price_receipt_with_discount(rep.\"PK_receipt\") as sumo , sh.\"address\" , rep.* "
                       "From \"Receipt\" as rep "
                       "Join \"Shop\" as sh on rep.\"PK_shop\"=sh.\"PK_shop\" " + dop +
                       "Order by sumo");
        }
        while(query.next()){
            ui->listWidget->addItem("Дата и время: " + query.value(3).toString() + " " + query.value(4).toString() + "\n" +
                                    "Сумма: " + query.value(0).toString() + "\n" +
                                    "Магазин: " + query.value(1).toString());
        }
    }
    else if(OpenedTabel == "Product"){
        ui->listWidget->clear();
        QString date1, date2;
        date1 = QDateTime::currentDateTime().addMonths(-1).toString();
        date2 = QDateTime::currentDateTime().toString();
        QSqlQuery query;
        QString dop("");
        if(Filt != ""){
            dop = "Where cat.\"name\"=\'" + Filt + "\'";
        }
        if(ui->SortBox->currentIndex() == 0){
            query.exec("SELECT cat.\"name\", "
                       "count_sale_product(prod.\"name\", \'" + date1 + "\', \'" + date2 + "\'), "
                       "income_with_product(prod.\"name\", \'" + date1 + "\', \'" + date2 + "\'), "
                       "prod.* "
                       "From \"Product\" as prod "
                       "Join \"Product_Category\" as cat on prod.\"PK_product_category\"=cat.\"PK_product_category\"" + dop +
                       "Order by prod.\"price_without_discount\"");
        }
        while(query.next()){
            ui->listWidget->addItem(query.value(4).toString() + "\n" +
                                    "Категория: " + query.value(0).toString() +
                                    " Продано за месяц: " + query.value(1).toString() + " " + query.value(6).toString() + "\n" +
                                    "Цена: " + query.value(5).toString() + " руб." +
                                    " Заработано за месяц: " + query.value(2).toString());
        }
    }
}


void MainWindow::on_FiltButton_clicked()
{
    if(OpenedTabel == "Product"){
        ui->listWidget->clear();
        QString date1, date2;
        date1 = QDateTime::currentDateTime().addMonths(-1).toString();
        date2 = QDateTime::currentDateTime().toString();
        QSqlQuery query("SELECT cat.\"name\", "
                        "count_sale_product(prod.\"name\", \'" + date1 + "\', \'" + date2 + "\'), "
                        "income_with_product(prod.\"name\", \'" + date1 + "\', \'" + date2 + "\'), "
                        "prod.* "
                        "From \"Product\" as prod "
                        "Join \"Product_Category\" as cat on prod.\"PK_product_category\"=cat.\"PK_product_category\""
                        "Where cat.\"name\"=\'" + ui->FiltBox->text() + "\'");
        while(query.next()){
            ui->listWidget->addItem(query.value(4).toString() + "\n" +
                                    "Категория: " + query.value(0).toString() +
                                    " Продано за месяц: " + query.value(1).toString() + " " + query.value(6).toString() + "\n" +
                                    "Цена: " + query.value(5).toString() + " руб." +
                                    " Заработано за месяц: " + query.value(2).toString());
        }
        Filt = ui->FiltBox->text();
    }
    else if(OpenedTabel == "Receipt"){
        ui->listWidget->clear();
        QSqlQuery query("SELECT price_receipt_with_discount(rep.\"PK_receipt\") , sh.\"address\" , rep.* "
                        "From \"Receipt\" as rep "
                        "Join \"Shop\" as sh on rep.\"PK_shop\"=sh.\"PK_shop\" "
                        "Where sh.\"address\"=\'" + ui->FiltBox->text() + "\'");
        while(query.next()){
            ui->listWidget->addItem("Дата и время: " + query.value(3).toString() + " " + query.value(4).toString() + "\n" +
                                    "Сумма: " + query.value(0).toString() + "\n" +
                                    "Магазин: " + query.value(1).toString());
        }
        Filt = ui->FiltBox->text();
    }
}
