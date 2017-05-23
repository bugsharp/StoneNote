#ifndef GETIMAGEINFODIALOG_H
#define GETIMAGEINFODIALOG_H

#include <QDialog>

namespace Ui {
class GetImageInfoDialog;
}

class GetImageInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GetImageInfoDialog(QWidget *parent = 0);
    ~GetImageInfoDialog();
    QString getBase64();
    QString getText();
    QString getImageData();
    void setImage(QImage img);
private slots:
    void on_pushButton_clicked();

private:
    Ui::GetImageInfoDialog *ui;
    QString picToData(QImage img);
};

#endif // GETIMAGEINFODIALOG_H
