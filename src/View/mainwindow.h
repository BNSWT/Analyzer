#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QToolBar>
#include <QMessageBox>
#include <QStandardItemModel>

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
    void on_pushButton_Lexical_clicked();

private:
    Ui::MainWindow *ui;
    QAction *openAction;
};
#endif // MAINWINDOW_H
