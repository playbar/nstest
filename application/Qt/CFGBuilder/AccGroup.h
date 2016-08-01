/*
 * 重力感应配置列表
 */

#ifndef ACCGROUP_H
#define ACCGROUP_H

class QVBoxLayout;
class QListWidget;
class QDialogButtonBox;
class QPushButton;
class XEasyXml;
class QListWidgetItem;
class AccDialog;

#include <QGroupBox>

class AccGroup : public QGroupBox
{
    Q_OBJECT
public:
    explicit AccGroup(QWidget *parent = 0);

public:
    // 恢复默认
    void Default();
    // 读取数据
    void ReadData(XEasyXml* xml);
    // 保存数据
    void SaveData(XEasyXml* xml);
    
signals:

public slots:
    // 添加ITEM
    void AddItem();
    // 修改ITEM
    void ModifyItem_list(QListWidgetItem* item);
    // 删除ITEM
    void DelCurrentItem();
    
private:
    void CreateGroup();
    void CreateAccs();
    void CreateButtonBox();
    
private:
    // 列表
    QListWidget* iAccs;
    
    // 按键
    QDialogButtonBox* iButtonBox;
    QPushButton* iAdd;
    QPushButton* iDel;
    
    // 排版
    QVBoxLayout* iLayout;
    
    // dialog
    AccDialog* iDialog;
};

#endif // ACCGROUP_H
