#include "gettableinfodialog.h"
#include "ui_gettableinfodialog.h"
#include <QDebug>
GetTableInfoDialog::GetTableInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GetTableInfoDialog)
{
    ui->setupUi(this);
    m_model = new QStandardItemModel();
    ui->tableView->setModel(m_model);
    m_model->setRowCount(2);
    m_model->setColumnCount(2);
    //ui->tableView->verticalHeader()->set
    m_model->setHeaderData(0,Qt::Vertical,"header");
}

GetTableInfoDialog::~GetTableInfoDialog()
{
    delete ui;
}

void GetTableInfoDialog::on_spinBox_valueChanged(int arg1)
{
    m_model->setRowCount(arg1);
    m_model->setColumnCount(ui->spinBox_2->value());

}

void GetTableInfoDialog::on_spinBox_2_valueChanged(int arg1)
{
    m_model->setRowCount(ui->spinBox->value());
    m_model->setColumnCount(arg1);
}

QString GetTableInfoDialog::getData()
{
    QString data;
    for(int r = 0;r<m_model->rowCount();r++){
        data.append("|");
        for(int c = 0;c<m_model->rowCount();c++){
            data.append(m_model->index(r,c).data().toString());
            data.append("|");
        }
        if(r==0)
        {
            data.append("\n|");
            for(int c = 0;c<m_model->rowCount();c++){
                data.append("-");
                data.append("|");
            }
        }
        data.append("\n");
    }
    return data;
}


