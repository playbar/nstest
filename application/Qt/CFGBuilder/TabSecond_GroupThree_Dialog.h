#ifndef TABSECOND_GROUPTHREE_DIALOG_H
#define TABSECOND_GROUPTHREE_DIALOG_H


// declearation
class QListWidget;
class QListWidgetItem;
class QLabel;
class QComboBox;
class QGridLayout;
class QPushButton;
class QDialogButtonBox;
class QLineEdit;
class QHBoxLayout;
class QBoxLayout;

// include
#include <QDialog>

class TabSecond_GroupThree_Dialog : public QDialog
{
    Q_OBJECT

public:
    TabSecond_GroupThree_Dialog(QWidget* parent = NULL);

public:
    // 修改item
    void ModifyItem_Item(QListWidget* parent, QListWidgetItem* item = NULL);

private:
    // 构造Dialog
    void SetupDialog();

    // 虚拟按键
    void SetupVirtualKey();

    // 对应鼠标动作
    void SetupMouseAct();

    // 按键组
    void SetupButtonBox();

public slots:
    // 处理点击OK按键
    void ClickOK();

private:
    // 虚拟按键
    QLabel*     iVirtualKey;
//    QLineEdit*  iVirtualKey_input;
    QComboBox* iVirtualKey_list;

    // 对应鼠标动作
    QLabel*     iMouseAct;
    QComboBox*  iMouseAct_list;

    // 排版
//    QGridLayout*    iLayout;
    QBoxLayout* iLayout;
    QHBoxLayout* isubLayout;

    // 按键
    QPushButton*        iOk;
    QPushButton*        iCancel;
    QDialogButtonBox*   iButtonBox;

    // List Point
    QListWidget*        iParent;

    // Item Point
    QListWidgetItem*    iItem;
};

#endif // TABSECOND_GROUPTHREE_DIALOG_H
