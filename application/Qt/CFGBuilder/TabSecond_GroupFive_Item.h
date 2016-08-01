/*
 * TabSecond中ButtonInfo Group List Item
 */
#ifndef TABSECOND_GROUPFIVE_ITEM_H
#define TABSECOND_GROUPFIVE_ITEM_H

// declearation

// include
#include <QListWidgetItem>

class TabSecond_GroupFive_Item
{
public:
    TabSecond_GroupFive_Item();

public:
    // 触屏上添加的按键
    void SetButtonadd(const QString& Buttonadd);
    QString Buttonadd() const;

//    // 按键的显示模式
//    void SetButtonDisplayMode(const QString& ButtonDisplayMode);
//    QString ButtonDisplayMode() const;

    // 按键的水平位置
    void SetButtonhorpos(const QString& Buttonhorpos);
    QString Buttonhorpos() const;

    // 按键的垂直位置
    void SetButtonverpos(const QString& Buttonverpos);
    QString Buttonverpos() const;

    //按键对应的PC按键
    void SetButtonmapto(const QString& Buttonmapto);
    QString Buttonmapto() const;

//    //固定位置按键的坐标
//    void SetButtonFixedAtt(const QString& ButtonFixedAtt);
//    QString ButtonFixedAtt() const;

//    // 漂浮按键的浮动位置
//    void SetButtonFloatside(const QString& ButtonFloatside);
//    QString ButtonFloatside() const;

//    // 漂浮按键的对齐方式
//    void SetButtonFloatalign(const QString& ButtonFloatalign);
//    QString ButtonFloatalign() const;

    // 是否支持长按
    void SetButtonLongpress(const QString& ButtonLongpress);
    QString ButtonLongpress() const;

    // 长按锁定时间
    void SetButtonLocktime(const QString& ButtonLocktime);
    QString ButtonLocktime() const;

private:
    // 触屏上添加的按键
    QString     iButtonadd;

//    // 按键的显示模式
//    QString     iButtonDisplayMode;

    // 按键的水平位置
    QString     iButtonhorpos;

    // 按键的垂直位置
    QString     iButtonverpos;

    // 按键对应的PC按键
    QString     iButtonmapto;

//    // 固定位置按键的坐标
//    QString     iButtonFixedAtt;

//    // 漂浮按键的浮动位置
//    QString     iButtonFloatside;

//    // 漂浮按键的对齐方式
//    QString     iButtonFloatalign;

    // 是否支持长按
    QString     iButtonLongpress;

    // 按键长按锁定时间
    QString     iButtonLocktime;
};

#endif // TABSECOND_GROUPFIVE_ITEM_H
