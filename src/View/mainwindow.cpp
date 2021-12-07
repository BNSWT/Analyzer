#include "mainwindow.h"
#include "dialog.h"
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

void MainWindow::on_pushButton_Start_clicked()
{
    QString input = ui->textEdit->toPlainText();
    lexicalRes = lexicalProcess(input.toStdString());
    lexicalRes.push_back({TYPE::TAIL, "end of file"});
    auto inputString = lexicalRes;
    readTable("/home/yuyangz/Documents/courses/compilation/Analyzer/data/Actiontable.txt", "/home/yuyangz/Documents/courses/compilation/Analyzer/data/GOTOtable.txt", gotoTable, actionTable);
    parserProcess(inputString, gotoTable, actionTable, parseProcess, errlog);
    QMessageBox::information(this, "提示", "语法分析成功！");

    string res;
    if(errlog.size()==0)
        res = res + "<font color=\"#34eb3d\">语法分析通过！</font>";
    else
        res = res + "<font color=\"#eb4034\">" + errlog + "</font>";
    ui->textBrowser->setText(res.c_str());
}

void MainWindow::on_pushButton_Lexical_clicked()
{
    if (lexicalRes.size()==0) {
        QMessageBox::warning(this, "警告", "请先开始分析！");
    }
    else {
        vector<token> res = lexicalRes;

        QStandardItemModel* tableModel = new QStandardItemModel();

        tableModel->setColumnCount(2);
        tableModel->setHeaderData(0,Qt::Horizontal,QString("类别"));
        tableModel->setHeaderData(1,Qt::Horizontal,QString("值"));


        for(auto iter=res.begin(); iter<res.end(); iter++) {
            tableModel->setItem(iter-res.begin(), 0, new QStandardItem(QString(names[iter->type].c_str())));
            tableModel->setItem(iter-res.begin(), 1, new QStandardItem(QString(iter->value.c_str())));
        }
        Dialog *dialog=new Dialog(this);
        dialog->setWindowTitle("词法分析结果");
        dialog->fillUp(tableModel);
        dialog->show();
    }
}

void MainWindow::on_pushButton_Syntax_clicked()
{
    if (lexicalRes.size()==0) {
        QMessageBox::warning(this, "警告", "请先开始分析！");
    }
    else {
        QStandardItemModel* tableModel = new QStandardItemModel();

        tableModel->setColumnCount(4);
        tableModel->setHeaderData(0,Qt::Horizontal,QString("状态栈"));
        tableModel->setHeaderData(1,Qt::Horizontal,QString("符号栈"));
        tableModel->setHeaderData(2,Qt::Horizontal,QString("输入元素类"));
        tableModel->setHeaderData(3,Qt::Horizontal,QString("输入元素值"));

        auto res = parseProcess;
        for(auto iter=res.begin(); iter<res.end(); iter++) {
            stringstream stateStream;
            while(iter->stateStack.size()) {
                stateStream << iter->stateStack.top() << " ";
                iter->stateStack.pop();
            }
            string state;
            stateStream >> state;

            string symbol;
            while(iter->symbolStack.size()) {
                auto sym = iter->symbolStack.top();
                if(sym.type==RIGHT_ELEM_TYPE::STATE) {
                    symbol += stateNames[sym.index] + " ";
                }
                else {
                    symbol += names[sym.index] + " ";
                }
                iter->symbolStack.pop();
            }


            tableModel->setItem(iter-res.begin(), 0, new QStandardItem(QString(state.c_str())));
            tableModel->setItem(iter-res.begin(), 1, new QStandardItem(QString(symbol.c_str())));
            tableModel->setItem(iter-res.begin(), 2, new QStandardItem(QString(names[iter->inputString[0].type].c_str())));
            tableModel->setItem(iter-res.begin(), 3, new QStandardItem(QString(iter->inputString[0].value.c_str())));
        }
        Dialog *dialog=new Dialog(this);
        dialog->setWindowTitle("语法分析结果");
        dialog->fillUp(tableModel);
        dialog->show();
    }
}

void MainWindow::on_pushButton_Action_clicked()
{
    if (actionTable.size()==0)
        readTable("/home/yuyangz/Documents/courses/compilation/Analyzer/data/Actiontable.txt", "/home/yuyangz/Documents/courses/compilation/Analyzer/data/GOTOtable.txt", gotoTable, actionTable);
    auto res = actionTable;

    QStandardItemModel* tableModel = new QStandardItemModel();

    tableModel->setColumnCount(TYPE::TAIL+1);
    int i =0;
    for (enum TYPE type=TYPE::HEAD; type <= TYPE::TAIL; type = (enum TYPE)(type+1)) {
        tableModel->setHeaderData(i,Qt::Horizontal,QString(names[type].c_str()));
        i++;
    }

    for (int i =0;i < actionTable.size(); i++) {
        for (int j =0;j < actionTable[i].size(); j++) {
            stringstream outfile;
            if (actionTable[i][j].gotoElemType==GOTO_ELEM_TYPE::MOVE){
                outfile << "S" << actionTable[i][j].des;
            }
            else if (actionTable[i][j].gotoElemType==GOTO_ELEM_TYPE::INDUCE){
                outfile << "R" << actionTable[i][j].des;
            }
            else
                outfile << "E";
            tableModel->setItem(i,j, new QStandardItem(QString(outfile.str().c_str())));
        }
    }
    Dialog *dialog=new Dialog(this);
    dialog->setWindowTitle("Action表");
    dialog->fillUp(tableModel);
    dialog->show();
}

void MainWindow::on_pushButton_Goto_clicked()
{
    if (gotoTable.size()==0)
        readTable("/home/yuyangz/Documents/courses/compilation/Analyzer/data/Actiontable.txt", "/home/yuyangz/Documents/courses/compilation/Analyzer/data/GOTOtable.txt", gotoTable, actionTable);
    auto res = gotoTable;

    QStandardItemModel* tableModel = new QStandardItemModel();

    int colNum = 20+TYPE::TAIL+1;
    tableModel->setColumnCount(colNum);
    int i =0;
    for (SYNTAX_STATE synState = SYNTAX_STATE::SBEG; synState <= SYNTAX_STATE::V; synState=(SYNTAX_STATE)(synState+1)) {
        tableModel->setHeaderData(i,Qt::Horizontal,QString(stateNames[synState].c_str()));
        i++;
    }
    for(; i<20; i++) {
        tableModel->setHeaderData(i,Qt::Horizontal,QString("保留"));
    }
    for (enum TYPE type=TYPE::HEAD; type <= TYPE::TAIL; type = (enum TYPE)(type+1)) {
        tableModel->setHeaderData(i,Qt::Horizontal,QString(names[type].c_str()));
        i++;
    }
    for (int i =0;i < gotoTable.size(); i++) {
        for (int j =0;j < colNum; j++) {
            stringstream outfile;
            outfile << gotoTable[i][j];
            tableModel->setItem(i,j, new QStandardItem(QString(outfile.str().c_str())));
        }
    }
    Dialog *dialog=new Dialog(this);
    dialog->setWindowTitle("Goto表");
    dialog->fillUp(tableModel);
    dialog->show();
}
