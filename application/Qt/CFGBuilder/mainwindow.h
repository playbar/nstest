/*
 * Builder的整体框架
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// declearation
class QMenu;
class QAction;
class TabFrame;
class XEasyXml;

// include
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    // 构造菜单栏
    void CreateMenu();
    // 构造File菜单
    void CreateMenu_File();
    // 构造Set菜单
    void CreateMenu_Set();
    // 构造Help菜单
    void CreateMenu_Help();

    // 构造主体部分
    void CreateFrame();


    
private slots:
    // 菜单部分
    // 新建文件
    void CreateFile();
    // 打开文件
    void OpenFile();
    // 保存文件
    void SaveFile();
    // 另存为
    void SaveAs();
    // 关闭
    void CloseFile();

    // 帮助部分
    void About();

    // CurDTD
    void CurDTD();
    
    // 更改文件路径
    void slotChangeFilePath(QString path) {iPath = path;}
    
    // 更改文件编码
    void slotChangeEncode(QString encode) {iEncode = encode;}
    
    // 请求save文件
    void slotSaveFile() {SaveFile();}
    
    // 更新path
    void slotUpdatePathItoO(QString path);
    
signals:
    void signalUpdatePathOtoI(QString);

public:
    XEasyXml* Engine() const { return iEngine; }

    void SaveAsForPreview() { SaveAs(); }

    QString PreviewContent() const;
private:
    // 菜单部分
    // 文件
    QMenu*      iFileMenu;
    // 新建
    QAction*    iFileCreate;
    // 打开文件
    QAction*    iFileOpen;
    // 保存文件
    QAction*    iFileSave;
    // 另存为
    QAction*    iFileSaveAs;
    // 关闭
    QAction*    iClose;
    // 退出
    QAction*    iExit;

    // 设置
    QMenu*      iSetMenu;

    // 帮助
    QMenu*      iHelpMenu;
    // 帮助文档
    QAction*    iHelpDoc;
    // 对应DTD
    QAction*    iCurDtd;
    // 关于
    QAction*    iAbout;

    // 主体部分
    TabFrame*   iTabFrame; /*!Central Widget*/

    // 记录临时路径
    QString     iPath;
    
    // 记录文件编码方式
    QString iEncode;
    
    // 文件的原始编码方式
    QString iEncodeOld;

    // 主要部分
    XEasyXml*   iEngine;
};

#endif // MAINWINDOW_H
