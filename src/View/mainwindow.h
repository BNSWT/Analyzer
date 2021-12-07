#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../src/Parser/Parser.h"
#include <QMainWindow>
#include <QAction>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QToolBar>
#include <QMessageBox>
#include <QStandardItemModel>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void on_actOpen_IODevice_triggered();
    bool openTextByIODevice(const QString &aFileName);

private slots:

    void on_pushButton_Start_clicked();
    void on_pushButton_Lexical_clicked();
    void on_pushButton_Syntax_clicked();
    void on_pushButton_Action_clicked();
    void on_pushButton_Goto_clicked();

private:
    vector<token> lexicalRes;
    vector<vector<int>> gotoTable;
    vector<vector<actionElem>> actionTable;
    vector<analyzeProcess> parseProcess;
    string errlog;
    Ui::MainWindow *ui;
    QAction *openAction;
    QAction *triggerDialog;
};
#endif // MAINWINDOW_H
