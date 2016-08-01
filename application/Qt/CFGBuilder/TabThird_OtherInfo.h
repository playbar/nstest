/*
 * Builder中Tab的第三个签，屏幕显示，声音，网络，文本信息
 */

#ifndef TABTHIRD_OTHERINFO_H
#define TABTHIRD_OTHERINFO_H

// declearation
class TabThird_GroupOne;
class TabThird_GroupTwo;
class TabThird_GroupThree;
class TabThird_GroupFour;
class QGridLayout;
class XEasyXml;

// include
#include <QWidget>
#include "TouchDefine_Dialog_Pad.h"

class TabThird_OtherInfo : public QWidget
{
    Q_OBJECT
    
public:
    TabThird_OtherInfo(QWidget* parent = NULL);

private:
    // 构造Tab
    void SetupTab();

public slots:
    void slotDirChange(ModelDir d)
    {
        emit signalDirChange(d);
    }
    
signals:
    void signalDirChange(ModelDir);
    
public:
    // 恢复默认
    void Default();

    // 读取数据
    void ReadData(XEasyXml* xml);

    // 保存数据
    void SaveData(XEasyXml* xml);

private:
    // 显示设置
    TabThird_GroupOne*      iGroupOne;

    // 声音设置
    TabThird_GroupTwo*      iGroupTwo;

    // 网络设置
    TabThird_GroupThree*    iGroupThree;

    // 文本设置
    TabThird_GroupFour*     iGroupFour;

    // Grid排版(2, 2)
    QGridLayout*            iLayout;
};

#endif // TABTHIRD_OTHERINFO_H
