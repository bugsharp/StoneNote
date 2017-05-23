#ifndef GETTABLEINFODIALOG_H
#define GETTABLEINFODIALOG_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class GetTableInfoDialog;
}

class GetTableInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GetTableInfoDialog(QWidget *parent = 0);
    ~GetTableInfoDialog();

    done();
    QString getData();
private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

private:
    Ui::GetTableInfoDialog *ui;
    QStandardItemModel  *m_model;
};

#endif // GETTABLEINFODIALOG_H
