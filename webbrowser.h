#ifndef WEBBROWSER_H
#define WEBBROWSER_H
#include <QAxWidget>

/**
 * @brief WebBrowser类，用于实时预览md文件转换成的html文件
 *
 */
class WebBrowser : public QAxWidget
{
public:
    /**
     * @brief 构造函数，通过QAxWidget初始化WebBrowser功能
     *
     */
    WebBrowser();
    /**
     * @brief 载入html文件并显示
     *
     * @param fileName html文件相对路径，放在当前文件夹下
     */
    void LoadHtml(QString fileName);
};

#endif // WEBBROWSER_H
