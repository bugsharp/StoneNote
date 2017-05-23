#include "getlinkinfodialog.h"
#include "ui_getlinkinfodialog.h"

GetLinkInfoDialog::GetLinkInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GetLinkInfoDialog)
{
    ui->setupUi(this);
}

GetLinkInfoDialog::~GetLinkInfoDialog()
{
    delete ui;
}

QString GetLinkInfoDialog::getLinkData()
{
    return QString("[%1](%2)").arg(ui->lineEdit->text()).arg(ui->lineEdit_2->text());
}

void GetLinkInfoDialog::setLink(QString link)
{
    ui->lineEdit->setText(link);
    ui->lineEdit_2->setText(link);
}
