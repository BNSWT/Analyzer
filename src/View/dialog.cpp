#include "dialog.h"
#include "./ui_dialog.h"

//构造函数
Dialog::Dialog(QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

//析构函数
Dialog::~Dialog()
{
    delete ui;
}


void Dialog::fillUp(QAbstractItemModel* model)
{
    //if (ui->tableView->model()->columnCount()<=4)
        //ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setModel(model);
}
