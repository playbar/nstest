/*
 * 触屏方案中的某个具体方案
 */
#ifndef TOUCHINFO_H
#define TOUCHINFO_H

class TouchButtonInfo;

#include <QListWidgetItem>

class TouchInfo : public QListWidgetItem
{
public:
    TouchInfo(QListWidget * parent = 0, int type = Type);
    
public:
    // 具体的按键信息
    QList<TouchButtonInfo*> iButtonInfoList;
};

#endif // TOUCHINFO_H
