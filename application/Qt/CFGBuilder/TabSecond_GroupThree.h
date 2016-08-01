/*
 * TabSecond中的TabMap Group
 */
#ifndef TABSECOND_GROUPTHREE_H
#define TABSECOND_GROUPTHREE_H

// declearation
class QLabel;
class QComboBox;
class QGridLayout;
class QListWidget;
class QPushButton;
class QDialogButtonBox;
class QVBoxLayout;
class QLineEdit;
class QListWidgetItem;
class TabSecond_GroupThree_Dialog;
class XEasyXml;

// include
#include <QGroupBox>

class TabSecond_GroupThree : public QGroupBox
{
    Q_OBJECT

public:
    TabSecond_GroupThree(QWidget* parent = NULL);

private:
    // 构造Group
    void SetupGroup();

    // 鼠标模式名称
    void SetupName();

    // 列表
    void SetupNormal();

    // 按键组
    void SetupButtonBox();

public:
    // 恢复默认
    void Default();

    // 读取数据
    void ReadData(XEasyXml* xml);

    // 保存数据
    void SaveData(XEasyXml* xml);

private slots:
    // 添加Item
    void AddItem();
    // 修改Item
    void ModifyItem_list(QListWidgetItem* item);
    // 删除Item
    void DelCurrentItem();

private:
    // 鼠标模式名称
    QLabel*         iname;
    QLineEdit*      iname_input;

    // 列表
    QListWidget*    inormal;

    // 按键
    QPushButton*    iAdd;
    QPushButton*    iDel;
    QDialogButtonBox*   iButtonBox;

    // 排版
    QVBoxLayout*    iLayout;

    // Dialog
    TabSecond_GroupThree_Dialog*  iDialog;
};

#endif // TABSECOND_GROUPTHREE_H
