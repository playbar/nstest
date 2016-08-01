/*
 * 触屏按键方案中某个具体的按键信息
 * 包括 @按键名称
 *         @对其点
 *         @坐标
 *         @映射
 *         @长按锁定
 *         @长按锁定时间
 */
#ifndef TOUCHBUTTONINFO_H
#define TOUCHBUTTONINFO_H

#include <QString>
#include <QListWidgetItem>

class TouchButtonInfo : public QListWidgetItem
{
public:
    TouchButtonInfo(QListWidget * parent = 0, int type = Type);
    
public:
    // 按键名称
    QString iButtonName;
    // 对齐点
    QString iOrg;
    // 水平坐标
    QString iHorPos;
    // 垂直坐标
    QString iVerPos;
    // 映射
    QString iMapto;
    // 长按锁定
    QString iLongPress;
    // 锁定时间
    QString iLockTime;
};

#endif // TOUCHBUTTONINFO_H
