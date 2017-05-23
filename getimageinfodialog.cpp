#include "getimageinfodialog.h"
#include "ui_getimageinfodialog.h"
#include <QFileDialog>
#include <QBuffer>
#include <QFileInfo>
#include <QDebug>
GetImageInfoDialog::GetImageInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GetImageInfoDialog)
{
    ui->setupUi(this);
}

GetImageInfoDialog::~GetImageInfoDialog()
{
    delete ui;
}

void GetImageInfoDialog::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home",
                                                    tr("Images (*.png *.bmp *.jpg)"));
    if(!fileName.isEmpty())
    {
        QPixmap pixmap(fileName);
        pixmap = pixmap.scaled(ui->label_2->width(),ui->label_2->height(),Qt::KeepAspectRatio);
        ui->label_2->setPixmap(pixmap);
        ui->textBrowser->setPlainText(picToData(QImage(fileName)));
        ui->lineEdit->setText(QFileInfo(fileName).baseName());
    }
}

QString GetImageInfoDialog::picToData(QImage img)
{
    qDebug()<<img.width();
    if(img.width()>800||img.height()>600)
    {
        img = img.scaled(800,600,Qt::KeepAspectRatio);
    }
    QByteArray ba;
    QBuffer buf(&ba);
    img.save(&buf,"JPG");//按照JPG解码保存数据
    //QByteArray cc=qCompress(ba,1);
    return QString(ba.toBase64());//base64数据
}

QString GetImageInfoDialog::getBase64()
{
    return ui->textBrowser->toPlainText();
}

QString GetImageInfoDialog::getText()
{
    return ui->lineEdit->text();
}

void GetImageInfoDialog::setImage(QImage img)
{
    QPixmap pixmap = QPixmap::fromImage(img);
    pixmap = pixmap.scaled(543,179,Qt::KeepAspectRatio);
    ui->label_2->setPixmap(pixmap);
    ui->textBrowser->setPlainText(picToData(img));
    ui->lineEdit->setText("");
}

QString GetImageInfoDialog::getImageData()
{
    return QString("![%1](data:image/jpeg;base64,%2)").arg(getText()).arg(getBase64());
}
