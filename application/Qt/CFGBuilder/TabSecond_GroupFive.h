/*
 * TabSecond中的TouchInfo Group
 */
#ifndef TABSECOND_GROUPFIVE_H
#define TABSECOND_GROUPFIVE_H

// declearation
class QListWidget;
class QPushButton;
class QDialogButtonBox;
class QVBoxLayout;
class TabSecond_GroupFive_Dialog;
class QListWidgetItem;
class XEasyXml;
class XTouchPopup_Dialog;
class TouchDefine_Dialog;
#include "TouchDefine_Dialog_Pad.h"
class MainWindow;
class QLineEdit;
class QListWidget;
class QComboBox;

// include
#include <QGroupBox>

class TabSecond_GroupFive : public QGroupBox
{
    Q_OBJECT

public:
    TabSecond_GroupFive(QWidget* parent = NULL);
    
    void SetMainWindow(MainWindow* MainWindow) { iMainWindow = MainWindow; }

private:
    // 构造Group
    void SetupGroup();

    // mousepos
    void SetupMousePos();
    // 列表
    // 构造
    void SetupButton();

    // 按键组
    // 构造
    void SetupButtonbox();

public:
    // 恢复默认
    void Default();

    // 读取数据
    void ReadData(XEasyXml* xml);

    // 保存数据
    void SaveData(XEasyXml* xml);

signals:
    // 请求刷新
    void signalRefresh();
    
private slots:
    // 添加ITEM
    void AddItem();
    
    // 修改（应该是弹出包含所有按键信息的对话框）
    void ModifyItem();

    // 修改ITEM
    void ModifyItem_list(QListWidgetItem* item);

    // 删除ITEM
    void DelCurrentItem();
    
    // 设置方向
    void slotSetDir(ModelDir d) { dir = d;}

private:
    // mousepos
    QLabel* imousePos;
    QComboBox* imousePos_list;

    // 列表
    QListWidget*    iButton;

    // 按键组
    QPushButton*    iAdd;
    QPushButton*    iModify;
    QPushButton*    iDel;
    QDialogButtonBox*   iButtonbox;

    // 排版
    QVBoxLayout*    iLayout;

    // Dialog
//    TabSecond_GroupFive_Dialog* iDialog;

    // NewDialog
//    XTouchPopup_Dialog* iDialog;
    // 触屏方案的对话框
    TouchDefine_Dialog* iDialog;
    
    // 外部设置手机方向为
    ModelDir dir;
    
    MainWindow*     iMainWindow;
};

#endif // TABSECOND_GROUPFIVE_H
