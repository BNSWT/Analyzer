#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../Lexical/Lexical.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    openAction = ui->action_O;
    openAction->setShortcuts(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::on_actOpen_IODevice_triggered);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actOpen_IODevice_triggered()
{//打开文件
    QString curPath=QDir::currentPath();//获取系统当前目录
    QString dlgTitle="打开一个文件"; //对话框标题
    QString filter="程序文件(*.h *.cpp);;文本文件(*.txt);;所有文件(*.*)"; //文件过滤器
    QString aFileName=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);
    if (aFileName.isEmpty())
        return;
    openTextByIODevice(aFileName);
}
bool MainWindow::openTextByIODevice(const QString &aFileName)
{//用IODevice方式打开文本文件
    QFile aFile(aFileName);
    if (!aFile.exists()) //文件不存在
        return false;
    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    ui->textEdit->setPlainText(aFile.readAll());
    aFile.close();

    return  true;
}

void MainWindow::on_pushButton_Lexical_clicked()
{
    QMessageBox::information(this, "My Tittle", "Hello World!");
    QString input = ui->textEdit->toPlainText();
    vector<token> res = lexicalProcess(input.toStdString());

    ui->listView->setFlow(QListView::TopToBottom);
    QStandardItemModel* ListModel = new QStandardItemModel();
    for (vector<token>::iterator it = res.begin(); it < res.end(); it++) {
        string newItemStr = names[it->type]+"\t"+it->value;
        QString newItem(newItemStr.c_str());
        QStandardItem* Items = new QStandardItem(newItem);
        ListModel->appendRow(Items);
    }
    ui->listView->setModel(ListModel);
}

