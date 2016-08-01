/*
 * Builder中Tab第四个签，用于预览配置文件信息
 */
#ifndef TABFOURTH_PREVIEW_H
#define TABFOURTH_PREVIEW_H

// declearation
class QTextEdit;
class QPushButton;
class QDialogButtonBox;
class QVBoxLayout;
class XEasyXml;
class MainWindow;

// include
#include <QWidget>

class TabFourth_Preview : public QWidget
{
    Q_OBJECT

public:
    TabFourth_Preview(QWidget* parent = NULL);

private:
    // 构造Tab
    void SetupTab();

    // 预览区域
    void SetupPreview();

    // 按键组
    void SetupButtonbox();

private slots:
    void SetPreview();

    void CreateCfg();

public:
//    // 设置预览区域文字
//    void SetPreview(const QString& str);

//    // 获取预览区域蚊子
//    QString Preview() const;

    void SetMainWindow(MainWindow* MainWindow) { iMainWindow = MainWindow; }

    // 恢复默认
    void Default();

    // 读取数据
    void ReadData(XEasyXml* xml);

    // 保存数据
    void SaveData(const XEasyXml* xml);

private:
    // 预览区域
    QTextEdit*      iPreview;

    // 按键组
    QPushButton*    iCreate;
    QPushButton*    iOk;
    QDialogButtonBox*   iButtonbox;

    // 排版
    QVBoxLayout*    iLayout;

    MainWindow*     iMainWindow;
};

#endif // TABFOURTH_PREVIEW_H
