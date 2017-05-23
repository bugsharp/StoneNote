#include "webbrowser.h"
#include <QDir>
#include <QDebug>
#include <QSizePolicy>

WebBrowser::WebBrowser()
{
    setControl(QString::fromUtf8("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
    setObjectName(QString::fromUtf8("webWidget"));
    QSizePolicy treeSizePolicy = this->sizePolicy();
    treeSizePolicy.setHorizontalPolicy(QSizePolicy::Maximum);

}

void WebBrowser::LoadHtml(QString fileName)
{
    qDebug()<<QString("file:///%1/%2").arg(QDir::currentPath()).arg(fileName);
    dynamicCall("Navigate(const QString&)",QString("file:///%1/%2").arg(QDir::currentPath()).arg(fileName));
}

