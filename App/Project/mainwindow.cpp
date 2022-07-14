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
        ui->tableWidget->setShowGrid(true);
        ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
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
    ui->FiltBox->setEnabled(1);
    ui->FiltButton->setEnabled(1);
    ui->FiltBox->clear();
    QSqlQuery query("Select prod.* from \"Product_Category\" as prod");
    while(query.next()){
        ui->FiltBox->addItem(query.value(1).toString());
    }
    Filt = Sort = "";
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
    ui->FiltBox->clear();
    QSqlQuery query("Select sh.* from \"Shop\" as sh");
    while(query.next()){
        ui->FiltBox->addItem(query.value(2).toString());
    }
    Filt = Sort = "";
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
    Filt = Sort = "";
}

void MainWindow::on_SortButton_clicked()
{
    QString sort("");
    QString filt("");
    if(OpenedTabel == "Receipt"){
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
    Sort = sort;
}


void MainWindow::on_FiltButton_clicked()
{
    if(OpenedTabel == "Product"){
        GoodsShow("Where cat.\"name\"=\'" + ui->FiltBox->currentText() + "\'", Sort);
        Filt = ui->FiltBox->currentText();
    }
    else if(OpenedTabel == "Receipt"){
        CheckShow("Where sh.\"address\"=\'" + ui->FiltBox->currentText() + "\'", Sort);
        Filt = ui->FiltBox->currentText();
    }
}

void MainWindow::GoodsShow(QString filt, QString sort)
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(7);
    QStringList header;
    header << "Название" << "Категория" << "Продано за месяц" << "Сравнение с прошлым месяцем" << "Цена" <<
              "Заработано за месяц" << "Сравнение с прошлым месяцем";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    QString date1, date2, date3;
    //date1 = QDateTime::currentDateTime().addMonths(-1).toString();
    //date2 = QDateTime::currentDateTime().toString();
    //date3 = QDateTime::currentDateTime().addMonths(-2).toString();
    date1 = "2021-12-01";
    date2 = "2022-01-01";
    date3 = "2021-11-01";
    double tent1, tent2;
    QSqlQuery query("SELECT cat.\"name\", "
                    "count_sale_product(prod.\"name\", \'" + date1 + "\', \'" + date2 + "\') as cspNow, "
                    "income_with_product(prod.\"name\", \'" + date1 + "\', \'" + date2 + "\') as iwpNow, "
                    "count_sale_product(prod.\"name\", \'" + date3 + "\', \'" + date1 + "\') as cspOld, "
                    "income_with_product(prod.\"name\", \'" + date3 + "\', \'" + date1 + "\') as iwpOld, "
                    "prod.* "
                    "From \"Product\" as prod "
                    "Join \"Product_Category\" as cat on prod.\"PK_product_category\"=cat.\"PK_product_category\"" + filt + sort);
    int i;
    i = 0;
    while(query.next()){
        tent1 = query.value(3).toDouble();
        if(tent1 == 0) tent1++;
        tent1 = ((query.value(1).toDouble() - query.value(3).toDouble())/tent1) * 100;
        tent2 = query.value(4).toDouble();
        if(tent2 == 0) tent2++;
        tent2 =  ((query.value(2).toDouble() - query.value(4).toDouble())/tent2) * 100;
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(query.value(6).toString()));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(query.value(0).toString()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(query.value(1).toString() + " " + query.value(8).toString()));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(tent1) + + "\%"));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(query.value(7).toString() + " руб."));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(query.value(2).toString() + " руб."));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(tent2) + "\%"));
        i++;
    }
    ui->tableWidget->resizeColumnsToContents();
}

void MainWindow::CheckShow(QString filt, QString sort)
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(3);
    QStringList header;
    header << "Дата и время" << "Итоговая Сумма" << "Магазин";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    QSqlQuery query("SELECT price_receipt_with_discount(rep.\"PK_receipt\") as prwd, "
                    "sh.\"address\" , rep.* "
                    "From \"Receipt\" as rep "
                    "Join \"Shop\" as sh on rep.\"PK_shop\"=sh.\"PK_shop\" " + filt + sort);
    int i;
    i = 0;
    while(query.next()){
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(query.value(3).toString() + " " + query.value(4).toString()));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(query.value(0).toString() + " руб."));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(query.value(1).toString()));
        i++;
    }
    ui->tableWidget->resizeColumnsToContents();
}

void MainWindow::ShopShow(QString sort)
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(5);
    QStringList header;
    header << "Адрес" << "Покупателей за месяц" << "Сравнение с прошлым месяцем" << "Прибыль за месяц" << "Сравнение с прошлым месяцем";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    QString date1, date2, date3;
    //date1 = QDateTime::currentDateTime().addMonths(-1).toString();
    //date2 = QDateTime::currentDateTime().toString();
    //date3 = QDateTime::currentDateTime().addMonths(-2).toString();
    date1 = "2021-12-01";
    date2 = "2022-01-01";
    date3 = "2021-11-01";
    double tent1, tent2;
    QSqlQuery query("Select count_receipt_with_shop(sh.\"address\", \'" + date1 + "\', \'" + date2 + "\') as crwsNow, "
                    "store_income(sh.\"address\", \'" + date1 + "\', \'" + date2 + "\') as siNow, "
                    "count_receipt_with_shop(sh.\"address\", \'" + date3 + "\', \'" + date1 + "\') as crwsOld, "
                    "store_income(sh.\"address\", \'" + date3 + "\', \'" + date1 + "\') as siOld, sh.* "
                    "From \"Shop\" as sh " + sort);
    int i;
    i = 0;
    while(query.next()){
        tent1 = query.value(2).toDouble();
        if(tent1 == 0) tent1++;
        tent1 = ((query.value(0).toDouble() - query.value(2).toDouble())/tent1) * 100;
        tent2 = query.value(3).toDouble();
        if(tent2 == 0) tent2++;
        tent2 =  ((query.value(1).toDouble() - query.value(3).toDouble())/tent2) * 100;
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(query.value(6).toString()));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(query.value(0).toString()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(tent1) + "\%\n"));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(query.value(1).toString() + " руб."));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(tent2) + + "\%"));
        i++;
    }
    ui->tableWidget->resizeColumnsToContents();
}

void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column)
{
    if(OpenedTabel == "Product"){
            InfoWindow *Form = new InfoWindow;
            Form->UpdateInfo(ui->tableWidget->item(row, 0)->text(), "Product");
            Form->exec();
    }
    else if(OpenedTabel == "Shop"){
            InfoWindow *Form = new InfoWindow;
            Form->UpdateInfo(ui->tableWidget->item(row, 0)->text(), "Shop");
            Form->exec();
    }
    else if(OpenedTabel == "Receipt"){
            CheckForm *Form = new CheckForm;;
            Form->UpdateInfo(ui->tableWidget->item(row, 0)->text(), ui->tableWidget->item(row, 2)->text());
            Form->exec();
    }
}

