/*
 * Builder中Tab的第二个签，手机操作的鼠标按键映射
 */

#ifndef TABSECOND_OPERATORINFO_H
#define TABSECOND_OPERATORINFO_H

// declearation
class TabSecond_GroupOne;
class TabSecond_GroupTwo;
class TabSecond_GroupThree;
class TabSecond_GroupFour;
class TabSecond_GroupFive;
class QGridLayout;
class QVBoxLayout;
class XEasyXml;
class AccGroup;

// include
#include <QWidget>
#include "TouchDefine_Dialog_Pad.h"
#include "TabSecond_GroupFive.h"

class TabSecond_OperatorInfo : public QWidget
{
    
    Q_OBJECT
    
public:
    TabSecond_OperatorInfo(QWidget* parent = NULL);
    
    void SetMainWindow(MainWindow* MainWindow)
    {
        if(iGroupFive)
            iGroupFive->SetMainWindow(MainWindow);
    }

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

public:
//    // 操作基本信息
//    TabSecond_GroupOne*     iGroupOne;

    // 鼠标通常模式
    TabSecond_GroupTwo*     iGroupTwo;

    // 鼠标列表模式
    TabSecond_GroupThree*     iGroupThree;

    // 按键映射
    TabSecond_GroupFour*     iGroupFour;

    // 触屏按键映射
    TabSecond_GroupFive*     iGroupFive;
    
    // 重力配置
    AccGroup* iAccGroup;

    // 排版
    QVBoxLayout*            iLayout;
};

#endif // TABSECOND_OPERATORINFO_H
