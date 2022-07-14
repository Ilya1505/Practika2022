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
