/*
 * TabThird中TxtInfo Group List Item的详情
 */
#ifndef TABTHIRD_GROUPFOUR_DIALOG_H
#define TABTHIRD_GROUPFOUR_DIALOG_H

// declearation
class QLabel;
class QLineEdit;
class QTextCodec;
class QTextEdit;
class QGridLayout;
class QPushButton;
class QListWidgetItem;
class QDialogButtonBox;
class QListWidget;
class TabThird_GroupFour;
class TabThird_GroupFour_Item;

// include
#include <QDialog>

class TabThird_GroupFour_Dialog : public QDialog
{
    Q_OBJECT

public:
    TabThird_GroupFour_Dialog(QWidget* parent = NULL);

public:
    // 用于添加，查看，修改ITEM
    void ModifyItem_Item(QListWidget* parent, QListWidgetItem* item = NULL);

private:
    // 构造Dialog
    void SetupDialog();

    // 文本名称
    // 构造
    void SetupTxtname();

    // 文本内容
    // 构造
    void SetupTxtdefine();

    // 按键
    void SetupButtongroup();

private slots:
    // 处理点击OK按键
    void ClickOK();

private:
    // 文本名称
    QLabel*     itxtname;
    QLineEdit*  itxtname_input;

    // 文本内容
    QLabel*     itxtdefine;
    QTextEdit*  itxtdefine_input;

    // 排版
    QGridLayout*    iLayout;

    // 按键
    QPushButton*    iOk;
    QPushButton*    iCancel;
    QDialogButtonBox*   iButtonbox;

    // List的指针
    QListWidget*    iListparent;

    // 当前ITEM的指针
    QListWidgetItem*    iItem;
};

#endif // TABTHIRD_GROUPFOUR_DIALOG_H
