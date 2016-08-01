/*
 * TabSecond中的KeyInfo Group list item的详细信息
 */
#ifndef TABSECOND_GROUPFOUR_DIALOG_H
#define TABSECOND_GROUPFOUR_DIALOG_H

// declearation
class QLabel;
class QLineEdit;
class QComboBox;
class QGridLayout;
class QPushButton;
class QDialogButtonBox;
class QListWidget;
class QListWidgetItem;
class QSpinBox;
class XPopupLineEdit;
class XPopupKeyMapTo_LineEdit;

// include
#include <QDialog>

class TabSecond_GroupFour_Dialog : public QDialog
{
    Q_OBJECT

public:
    TabSecond_GroupFour_Dialog(QWidget* parent = NULL);

public:
    // 修改ITEM
    void ModifyItem_Item(QListWidget* parent, QListWidgetItem* item = NULL);

private:
    // 构造Dialog
    void SetupDialog();

    // 被按下的手机按键
    void SetupKeypress();

    // 映射按键
    void SetupKeymapto();

    // 是否支持长按
    void SetupLongpress();

    // 长按锁定时间
    void SetupLocktime();

    // 按键组
    void SetupButtonbox();

public slots:
    // 处理点击OK按键
    void ClickOK();


private:
    // 被按下的手机按键
    QLabel*     iKeypress;
//    QLineEdit*  iKeypress_input;
    XPopupLineEdit* iKeypress_input;

    // 映射按键
    QLabel*     iKeymapto;
//    QLineEdit*  iKeymapto_input;
    XPopupKeyMapTo_LineEdit* iKeymapto_input;

    // 是否支持长按
    QLabel*     iLongpress;
    QComboBox*  iLongpress_list;

    // 长按锁定时间
    QLabel*     iLocktime;
    QLineEdit*  iLocktime_input;

    // 按键
    QPushButton*    iOk;
    QPushButton*    iCancel;
    QDialogButtonBox*   iButtonbox;

    // 排版
    QGridLayout*    iLayout;

    // List points
    QListWidget*    iParent;

    // Item points
    QListWidgetItem*    iItem;
};

#endif // TABSECOND_GROUPFOUR_DIALOG_H
