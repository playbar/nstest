/*
 * TabSecond中ButtonInfo List Item的详细信息
 */
#ifndef TABSECOND_GROUPFIVE_DIALOG_H
#define TABSECOND_GROUPFIVE_DIALOG_H

// declearation
class QLabel;
class QLineEdit;
class QComboBox;
class QGridLayout;
class QPushButton;
class QDialogButtonBox;
class QListWidget;
class QListWidgetItem;

// include
#include <QDialog>

class TabSecond_GroupFive_Dialog : public QDialog
{
    Q_OBJECT

public:
    TabSecond_GroupFive_Dialog(QWidget* parent = NULL);

private:
    // 构造Dialog
    void SetupDialog();

    // 需要在触屏上添加的按键
    void SetupButtonadd();

    // 按键水平位置
    void SetupButtonhorpos();

    // 按键垂直位置
    void SetupButtonverpos();

//    // 按键的显示模式
//    void SetupButtonDisplayMode();

    // 按键对应的PC按键
    void SetupButtonmapto();

//    // 固定位置按键的坐标
//    void SetupButtonFixedAtt();

//    // 漂浮按键的浮动位置
//    void SetupButtonFloatside();

//    // 漂浮按键的对齐方式
//    void SetupButtonFLoatalign();

    // 是否支持长按
    void SetupButtonLongpress();

    // 长按锁定时间
    void SetupButtonLocktime();

    // 按键
    void SetupButtonbox();

public:
    // 修改ITEM
    void ModifyItem_Item(QListWidget* parent, QListWidgetItem* item = NULL);

private slots:
    // 处理点击OK按键
    void ClickOK();

private:
    // 需要在触屏上添加的按键
    QLabel*         iButtonadd;
    QComboBox*      iButtonadd_list;

    // 按键水平位置
    QLabel*         iButtonhorpos;
    QLineEdit*      iButtonhorpos_input;

    // 按键垂直位置
    QLabel*         iButtonverpos;
    QLineEdit*      iButtonverpos_input;

    // 按键对应的PC按键
    QLabel*         iButtonmapto;
    QLineEdit*      iButtonmapto_input;

    // 是否支持长按
    QLabel*         iButtonLongpress;
    QComboBox*      iButtonLongpress_list;

    // 按键长按锁定时间
    QLabel*         iButtonLocktime;
    QLineEdit*      iButtonLocktime_input;

    // 排版
    QGridLayout*    iLayout;

    // 按键
    QPushButton*    iOk;
    QPushButton*    iCancel;
    QDialogButtonBox*   iButtonbox;

    // List Parent
    QListWidget*    iListParent;

    // Item Points
    QListWidgetItem*    iItem;
};

#endif // TABSECOND_GROUPFIVE_DIALOG_H
