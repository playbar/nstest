/*
 * 重力感应配置的某一项
 */
#ifndef ACCGROUP_ITEM_H
#define ACCGROUP_ITEM_H

#include <QListWidgetItem>

class AccGroup_Item : public QListWidgetItem
{
public:
    AccGroup_Item(QListWidget *parent = 0, int type = Type);

public:
    // 方向
    QString iDirection;
    
    // accspeed
    QString iAccSpeed;
    
    // step
    QString iStep;
    
    // 按键映射
    bool BMapto;
    QString iMapto;
    
    //  鼠标动作
    bool BMouseAct;
    QString iMouseAct;
    
    // OnlyOne
    QString iOnlyOne;

};

#endif // ACCGROUP_ITEM_H
