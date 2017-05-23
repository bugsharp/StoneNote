#ifndef GETLINKINFODIALOG_H
#define GETLINKINFODIALOG_H

#include <QDialog>

namespace Ui {
class GetLinkInfoDialog;
}

class GetLinkInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GetLinkInfoDialog(QWidget *parent = 0);
    ~GetLinkInfoDialog();
    void setLink(QString link);

    QString getLinkData();
private:
    Ui::GetLinkInfoDialog *ui;
};

#endif // GETLINKINFODIALOG_H
