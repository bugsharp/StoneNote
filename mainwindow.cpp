#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QTextStream>
#include  <QFile>
#include <QSettings>
#include <QFileDialog>
#include <QDebug>
#include <QTimer>
#include <QTextCodec>
#include <QSettings>
#include <QInputDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include "gettableinfodialog.h"
#include "getimageinfodialog.h"
#include "getlinkinfodialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化treeview
    m_filesystem_model = new QFileSystemModel;
    m_cur_path = QDir::currentPath();
   // m_filesystem_model->setRootPath(m_cur_path);
    ui->treeView->setModel(m_filesystem_model);
    ui->treeView->setColumnHidden(1,true);
    ui->treeView->setColumnHidden(2,true);
    ui->treeView->setColumnHidden(3,true);
    ui->treeView->header()->hide();
    //初始化webBrowser
    m_web_browser = new WebBrowser();

    //初始化Splitter
    m_splitter = new QSplitter(this);
    setCentralWidget(m_splitter);
    m_splitter->addWidget(ui->treeView);
    m_splitter->addWidget(ui->textEdit);
    m_splitter->addWidget(m_web_browser);

    //初始化比例
    m_splitter->setStretchFactor(0,1);
    m_splitter->setStretchFactor(1,3);
    m_splitter->setStretchFactor(2,3);
    QTimer::singleShot(20,this,SLOT(on_init_ui()));

    //初始化变量
    m_codec = "utf-8";
    m_cur_file ="";
    is_test_changed = false;

    //读取配置文件
    QSettings config("./config.ini", QSettings::IniFormat);
    open_dir(config.value("curpath").toString());

    //初始化菜单栏关系
    m_codec_action_group =  new QActionGroup(this);
    m_codec_action_group->addAction(ui->action_GBK);
    m_codec_action_group->addAction(ui->action_utf8);
    m_codec_action_group->setExclusive(true);

    //初始化右键菜单
    m_dir_context_menu = new QMenu(this);
    m_file_context_menu = new QMenu(this);
    m_dir_add_file_action =new QAction("添加文件",this);
    m_dir_rename_action   =new QAction("重命名",this);
    m_dir_add_dir_action  =new QAction("添加文件夹",this);
    m_dir_delete_action   =new QAction("删除文件夹",this);
    m_dir_search_action   =new QAction("在文件夹中搜索",this);
    m_file_rename_action           =new QAction("重命名",this);
    m_file_delete_action           =new QAction("删除文件",this);
    m_file_open_in_explorer_action =new QAction("在资源管理器中打开",this);
    m_dir_context_menu->addAction(m_dir_add_file_action);
    m_dir_context_menu->addAction(m_dir_rename_action  );
    m_dir_context_menu->addAction(m_dir_add_dir_action );
    m_dir_context_menu->addAction(m_dir_delete_action  );
    m_dir_context_menu->addAction(m_dir_search_action  );
    m_file_context_menu->addAction(m_file_rename_action);
    m_file_context_menu->addAction(m_file_delete_action);
    m_file_context_menu->addAction(m_file_open_in_explorer_action);
    connect(m_dir_add_file_action,SIGNAL(triggered(bool)),this,SLOT(on_action_addfile_dir()));
    connect(m_dir_rename_action,SIGNAL(triggered(bool)),this,SLOT(on_action_rename_dir()));
    connect(m_dir_add_dir_action,SIGNAL(triggered(bool)),this,SLOT(on_action_mkdir_dir()));
    connect(m_dir_delete_action,SIGNAL(triggered(bool)),this,SLOT(on_action_delete_dir()));

    connect(m_file_rename_action,SIGNAL(triggered(bool)),this,SLOT(on_action_rename_file()));
    connect(m_file_delete_action,SIGNAL(triggered(bool)),this,SLOT(on_action_delete_file()));
    connect(m_file_open_in_explorer_action,SIGNAL(triggered(bool)),this,SLOT(on_action_open_in_explorer_file()));

    // 初始化剪切板
    m_clipBoard = QGuiApplication::clipboard();;
    ui->textEdit->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_preview_triggered()
{
    QFile data("output.md");
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);
        out.setCodec(QTextCodec::codecForName("utf-8"));
        QString data = ui->textEdit->toPlainText();
        data = data.replace("\n","\n\r");
        out << data<< endl;
    }
    data.close();
    QDir::current().remove("temp.html");
    QProcess myProcess;
    myProcess.start("pandoc --from=markdown --to=html --standalone --smart --section-divs --css=github.css --mathjax -s output.md -o temp.html");
    myProcess.waitForStarted();
    myProcess.waitForReadyRead();
    myProcess.waitForFinished();
    m_web_browser->LoadHtml("temp.html");
}

void MainWindow::on_action_quit_triggered()
{
    this->close();
}

void MainWindow::on_action_open_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    open_dir(dir);
    //将路径写入配置文件，方便下次使用
    QSettings config("./config.ini", QSettings::IniFormat);
    config.setValue("curpath",dir);
}

void MainWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
    save_cur_file();
    if (m_filesystem_model->fileInfo(index).completeSuffix().toLower() == "md") {
        m_cur_file = m_filesystem_model->filePath(index);
        open_cur_file();
    }
}

void MainWindow::on_action_utf8_triggered()
{
        save_cur_file();
        m_codec = "utf-8";
        open_cur_file();
}


void MainWindow::on_action_GBK_triggered()
{
        save_cur_file();
        m_codec = "GBK";
        open_cur_file();
}


void MainWindow::save_cur_file()
{
    if(QFile::exists(m_cur_file))
    {
        if(is_test_changed)
        {
            QFile data(m_cur_file);
            if (data.open(QFile::WriteOnly | QFile::Truncate)) {
                QTextStream out(&data);
                out.setCodec(QTextCodec::codecForName(m_codec.toLatin1()));
                QString str = ui->textEdit->toPlainText();
                out << str<< endl;
                ui->statusBar->showMessage("保存文件成功:"+m_cur_file);
                data.close();
                is_test_changed = false;
            }else
            {
                ui->statusBar->showMessage("保存文件失败:"+m_cur_file);
            }
        }
    }
}
void MainWindow::open_cur_file()
{
    QFile data(m_cur_file);
    if (data.open(QFile::ReadOnly)) {
        QTextStream out(&data);
        out.setCodec(QTextCodec::codecForName(m_codec.toLatin1()));
        ui->textEdit->clear();
        ui->textEdit->setPlainText(out.readAll());
        is_test_changed =false;
        data.close();
        on_action_preview_triggered();
        ui->statusBar->showMessage("打开文件成功:"+m_cur_file);
    }else
    {
        ui->statusBar->showMessage("打开文件失败:"+m_cur_file);
    }
}


void MainWindow::open_dir(QString dir)
{
    if(!dir.isEmpty())
    {
        m_cur_path = dir;
    }
    ui->treeView->setRootIndex(m_filesystem_model->setRootPath(m_cur_path));
}

void MainWindow::on_action_save_triggered()
{
    save_cur_file();
}

void MainWindow::on_textEdit_textChanged()
{
    is_test_changed = true;
}

void MainWindow::on_action_treeview_triggered()
{

}

void MainWindow::on_action_treeview_triggered(bool checked)
{
    if(!checked)
    {
        ui->treeView->hide();
    }else
    {
        ui->treeView->show();
    }
}

void MainWindow::on_action_edit_triggered()
{

}

void MainWindow::on_action_edit_triggered(bool checked)
{
    if(!checked)
    {
        ui->textEdit->hide();
    }else
    {
        ui->textEdit->show();
    }
}

void MainWindow::on_action_browser_triggered(bool checked)
{
    if(!checked)
    {
        m_web_browser->hide();
    }else
    {
        m_web_browser->show();
    }
}

void MainWindow::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->treeView->indexAt(pos);

    //在文件夹上调出右键菜单
    if(m_filesystem_model->fileInfo(index).isDir())
    {
        m_dir_context_menu->exec(QCursor::pos());
        return;
    }

    //在文件上调出右键菜单
    if(m_filesystem_model->fileInfo(index).isFile())
    {
        m_file_context_menu->exec(QCursor::pos());
        return;
    }

    //在空白处调出右键菜单
    ui->treeView->clearSelection();
    ui->treeView->setCurrentIndex(index);
    m_dir_context_menu->exec(QCursor::pos());
}


void MainWindow::on_action_addfile_dir()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("添加文件"),
                                         tr(""), QLineEdit::Normal,
                                         "readme.md", &ok);
    if (ok && !text.isEmpty())
    {
        QString rootPath;
        if(m_filesystem_model->fileInfo(ui->treeView->currentIndex()).isDir())
        {
            rootPath = m_filesystem_model->fileInfo(ui->treeView->currentIndex()).absoluteFilePath();
        }else
        {
            rootPath = m_filesystem_model->rootPath();
        }
        QString fileName = rootPath+QDir::separator()+text;
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            file.close();
            m_cur_file = fileName;
            open_cur_file();
        }
        ui->treeView->setCurrentIndex(m_filesystem_model->index(fileName));
    }
}

void MainWindow::on_action_rename_dir()
{
    if(m_filesystem_model->fileInfo(ui->treeView->currentIndex()).isDir())
    {
        bool ok;
        QString text = QInputDialog::getText(this, tr("重命名"),
                                             tr(""), QLineEdit::Normal,
                                             m_filesystem_model->fileInfo(ui->treeView->currentIndex()).baseName(), &ok);
        if (ok && !text.isEmpty())
        {
            QString oldName = m_filesystem_model->fileInfo(ui->treeView->currentIndex()).filePath();
            QString newName = m_filesystem_model->fileInfo(ui->treeView->currentIndex()).canonicalPath()+QDir::separator()+text;
            QDir dir(oldName);
            dir.rename(oldName,newName);
        }
    }
}

void MainWindow::on_action_mkdir_dir()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("添加文件夹"),
                                         tr(""), QLineEdit::Normal,
                                         m_filesystem_model->fileInfo(ui->treeView->currentIndex()).baseName(), &ok);
    if (ok && !text.isEmpty())
    {
        QString rootPath;
        if(m_filesystem_model->fileInfo(ui->treeView->currentIndex()).isDir())
        {
            rootPath = m_filesystem_model->fileInfo(ui->treeView->currentIndex()).absoluteFilePath();
        }else
        {
            rootPath = m_filesystem_model->rootPath();
        }
        QString oldName = rootPath;
        QString newName = rootPath+QDir::separator()+text;
        QDir dir(oldName);
        dir.mkdir(newName);
        ui->treeView->setCurrentIndex(m_filesystem_model->index(newName));
    }
}

void MainWindow::on_action_delete_dir()
{
    if(m_filesystem_model->fileInfo(ui->treeView->currentIndex()).isDir())
    {
        QMessageBox msgBox;
        msgBox.setText("删除后将无法恢复，确认删除文件夹？");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();

        if (QMessageBox::Ok == ret)
        {
            QString oldName = m_filesystem_model->fileInfo(ui->treeView->currentIndex()).absoluteFilePath();
            QDir dir(oldName);
            dir.rmpath(oldName);
        }
    }
}

void MainWindow::on_action_rename_file()
{
    if(m_filesystem_model->fileInfo(ui->treeView->currentIndex()).isFile()) {
        bool ok;
        QString text = QInputDialog::getText(this, tr("重命名"),
                                             tr(""), QLineEdit::Normal,
                                             m_filesystem_model->fileInfo(ui->treeView->currentIndex()).fileName(), &ok);
        if (ok && !text.isEmpty())
        {
            QString oldName = m_filesystem_model->fileInfo(ui->treeView->currentIndex()).filePath();
            QString newName = m_filesystem_model->fileInfo(ui->treeView->currentIndex()).canonicalPath()+QDir::separator()+text;
            QDir dir(oldName);
            dir.rename(oldName,newName);
        }
    }
}

void MainWindow::on_action_delete_file()
{
    if(m_filesystem_model->fileInfo(ui->treeView->currentIndex()).isFile()) {

        QMessageBox msgBox;
        msgBox.setText("确认删除文件？");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();

        if (QMessageBox::Ok == ret)
        {
            QString oldName = m_filesystem_model->fileInfo(ui->treeView->currentIndex()).filePath();
            QDir dir(oldName);
            dir.remove(m_filesystem_model->fileInfo(ui->treeView->currentIndex()).absoluteFilePath());
        }
    }
}

void MainWindow::on_action_open_in_explorer_file()
{
    if(m_filesystem_model->fileInfo(ui->treeView->currentIndex()).isFile()) {
        QProcess myProcess;
        QString filePath =m_filesystem_model->fileInfo(ui->treeView->currentIndex()).absoluteFilePath().replace("/","\\");
        myProcess.start(QString("explorer.exe /select, %1").arg(filePath));
        myProcess.waitForStarted();
        myProcess.waitForReadyRead();
        myProcess.waitForFinished();
    }
}


void MainWindow::closeEvent(QCloseEvent *event)
{
        save_cur_file();
        event->accept();
}

void MainWindow::on_action_insert_table_triggered()
{
    GetTableInfoDialog dialog;
    if(QDialog::Accepted == dialog.exec())
    {
        ui->textEdit->insertPlainText(dialog.getData());
    }
}

void MainWindow::on_action_insert_image_triggered()
{
    GetImageInfoDialog dialog;
    if(QDialog::Accepted == dialog.exec())
    {
        ui->textEdit->insertPlainText(dialog.getImageData());
    }
}

void MainWindow::on_action_insert_link_triggered()
{
    GetLinkInfoDialog dialog;
    if(QDialog::Accepted == dialog.exec())
    {
        ui->textEdit->insertPlainText(dialog.getLinkData());
    }
}

void MainWindow::on_paste_in_testedit()
{
    QString match = "^((ht|f)tps?):\/\/[\w\-]+(\.[\w\-]+)+([\w\-\.,@?^=%&:\/~\+#]*[\w\-\@?^=%&\/~\+#])?$";

    if(!m_clipBoard->image().isNull())
    {
        GetImageInfoDialog dialog;
        dialog.setImage(m_clipBoard->image());
        if(QDialog::Accepted == dialog.exec())
        {
            ui->textEdit->insertPlainText(dialog.getImageData());
        }
    }else if(m_clipBoard->text().contains(QRegExp(match)))
    {
        GetLinkInfoDialog dialog;
        dialog.setLink(m_clipBoard->text());
        if(QDialog::Accepted == dialog.exec())
        {
            ui->textEdit->insertPlainText(dialog.getLinkData());
        }else
        {
            ui->textEdit->paste();
        }
    }else{
        ui->textEdit->paste();
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->textEdit)
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            switch (keyEvent->key()) {
            case Qt::Key_V:
                if(keyEvent->modifiers() & Qt::ControlModifier)
                {
                    on_paste_in_testedit();
                }
                break;
            default:
                return false;
                break;
            }
            return true;
        }else{
            return false;
        }
    }

    //其它部件产生的事件则交给基类处理
    return QMainWindow::eventFilter(watched, event);
}


