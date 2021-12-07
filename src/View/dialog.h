#ifndef QWDIALOGHEADERS_H
#define QWDIALOGHEADERS_H
#include <QWidget>
#include <QDialog>
#include <QAbstractItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog:public QDialog
{
    Q_OBJECT
public:
    explicit Dialog(QWidget *parent = 0);
    void fillUp(QAbstractItemModel* model);
    ~Dialog();
private:
    Ui::Dialog *ui;
};

#endif // QWDIALOGHEADERS_H
