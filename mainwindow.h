#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <webbrowser.h>
#include <QSplitter>
#include <QActionGroup>
#include <QMenu>
#include <QAction>
#include <QClipboard>


namespace Ui {
class MainWindow;
}

/**
 * @brief 自定义主窗口类
 *
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     *
     * @param parent
     */
    explicit MainWindow(QWidget *parent = 0);
    /**
     * @brief 析构函数
     *
     */
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    /**
     * @brief 槽函数，实时预览功能实现函数
     *        当点击实时预览按钮、按下快捷键、新打开文件时被触发
     */
    void on_action_preview_triggered();

    /**
     * @brief 槽函数，退出软件函数
     *        当退出按钮被点击时触发
     */
    void on_action_quit_triggered();

    /**
     * @brief 槽函数，打开文件函数
     *        当打开按钮被点击时触发
     */
    void on_action_open_triggered();

    /**
     * @brief 槽函数，双击文件数中的文件，打开文件并载入预览
     *        当双击文件数中的文件时被触发
     * @param index 被双击的节点
     */
    void on_treeView_doubleClicked(const QModelIndex &index);

    /**
     * @brief
     *
     */
    void on_action_utf8_triggered();

    /**
     * @brief
     *
     */
    void on_action_GBK_triggered();

    /**
     * @brief
     *
     */
    void on_action_save_triggered();

    /**
     * @brief
     *
     */
    void on_textEdit_textChanged();

    /**
     * @brief
     *
     */
    void on_action_treeview_triggered();

    /**
     * @brief
     *
     * @param checked
     */
    void on_action_treeview_triggered(bool checked);

    /**
     * @brief
     *
     */
    void on_action_edit_triggered();

    /**
     * @brief
     *
     * @param checked
     */
    void on_action_edit_triggered(bool checked);

    /**
     * @brief
     *
     * @param checked
     */
    void on_action_browser_triggered(bool checked);

    /**
     * @brief
     *
     * @param pos
     */
    void on_treeView_customContextMenuRequested(const QPoint &pos);

    /**
     * @brief
     *
     */
    void on_action_addfile_dir();

    /**
     * @brief
     *
     */
    void on_action_rename_dir();

    /**
     * @brief
     *
     */
    void on_action_mkdir_dir();

    /**
     * @brief
     *
     */
    void on_action_delete_dir();

    /**
     * @brief
     *
     */
    void on_action_rename_file();

    /**
     * @brief
     *
     */
    void on_action_delete_file();

    /**
     * @brief
     *
     */
    void on_action_open_in_explorer_file();

    void on_action_insert_table_triggered();

    void on_action_insert_image_triggered();

    void on_paste_in_testedit();

    void on_action_insert_link_triggered();

private:
    Ui::MainWindow *ui; /**< TODO: describe */
    QString m_cur_path; /**< TODO: describe */
    QFileSystemModel *m_filesystem_model; /**< TODO: describe */
    WebBrowser *m_web_browser; /**< TODO: describe */
    QSplitter *m_splitter; /**< TODO: describe */
    QString m_codec; /**< TODO: describe */
    QString m_cur_file; /**< TODO: describe */
    QActionGroup *m_codec_action_group; /**< TODO: describe */
    QMenu *m_dir_context_menu; /**< TODO: describe */
    QMenu *m_file_context_menu; /**< TODO: describe */
    QAction *m_dir_add_file_action; /**< TODO: describe */
    QAction *m_dir_rename_action; /**< TODO: describe */
    QAction *m_dir_add_dir_action; /**< TODO: describe */
    QAction *m_dir_delete_action; /**< TODO: describe */
    QAction *m_dir_search_action; /**< TODO: describe */

    QAction *m_file_rename_action; /**< TODO: describe */
    QAction *m_file_delete_action; /**< TODO: describe */
    QAction *m_file_open_in_explorer_action; /**< TODO: describe */

    QClipboard *m_clipBoard;
    /**
     * @brief
     *
     */
    void save_cur_file();
    /**
     * @brief
     *
     */
    void open_cur_file();
    /**
     * @brief
     *
     * @param dir
     */
    void open_dir(QString dir);
    bool is_test_changed; /**< TODO: describe */

};

#endif // MAINWINDOW_H
