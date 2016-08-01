/*
 * TabSecond中的KeyInfo Group
 */
#ifndef TABSECOND_GROUPFOUR_H
#define TABSECOND_GROUPFOUR_H

// declearation
class QListWidget;
class QPushButton;
class QDialogButtonBox;
class QVBoxLayout;
class TabSecond_GroupFour_Dialog;
class QListWidgetItem;
class XEasyXml;
class QLabel;
class QLineEdit;

// include
#include <QGroupBox>

class TabSecond_GroupFour : public QGroupBox
{
    Q_OBJECT

public:
    TabSecond_GroupFour(QWidget* parent = NULL);

private:
    // 构造Group
    void SetupGroup();

    // buttontype
    void SetupButtonType();

    // 列表
    // 构造
    void SetupKey();

    // 按键组
    // 构造
    void SetupButtonBox();

public:
    // 恢复默认
    void Default();

    // 读取数据
    void ReadData(XEasyXml* xml);

    // 保存数据
    void SaveData(XEasyXml* xml);

private slots:
    // 添加ITEM
    void AddItem();
    // 修改ITEM
    void ModifyItem_list(QListWidgetItem* item);
    // 删除ITEM
    void DelCurrentItem();

private:
    // buttontype
    QLabel* itype;
    QLineEdit*  itype_input;
    // 列表
    QListWidget*    iKey;

    // 按键
    QPushButton*    iAdd;
    QPushButton*    iDel;
    QDialogButtonBox*   iButtonbox;

    // 排版
    QVBoxLayout*    iLayout;

    // Dialog
    TabSecond_GroupFour_Dialog* iDialog;
};

#endif // TABSECOND_GROUPFOUR_H
