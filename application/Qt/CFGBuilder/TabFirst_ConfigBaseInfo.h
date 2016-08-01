/*
 * Builder中Tab第一个签，配置文件的基本信息及鼠标操作的基本信息
 */
#ifndef TABFIRST_CONFIGBASEINFO_H
#define TABFIRST_CONFIGBASEINFO_H

// declearation
class TabFirst_GroupOne;
class TabFirst_GroupTwo;
class QVBoxLayout;
class XEasyXml;
class TabSecond_GroupOne;

// include
#include <QWidget>

class TabFirst_ConfigBaseInfo : public QWidget
{
    Q_OBJECT

public:
    TabFirst_ConfigBaseInfo(QWidget* parent = NULL);

private:
    // 构造TAB
    void SetupTab();
    
public slots:
    void slotUpdatePathItoO(QString path)
    {
        emit signalUpdatePathItoO(path);
    }
    
    void slotUpdatePathOtoI(QString path)
    {
        emit signalUpdatePathOtoI(path);
    }
    
signals:
    void signalUpdatePathItoO(QString);
    void signalUpdatePathOtoI(QString);

public:
    // 恢复默认
    void Default();

    // 读取数据
    void ReadData(XEasyXml* xml);

    // 保存数据
    void SaveData(XEasyXml* xml);

private:
    // 配置文件基本信息
    TabFirst_GroupOne*  iConfigInfo;

    // 鼠标操作基本信息
    TabFirst_GroupTwo*  iMouseInfo;
    
    // 操作基本信息
    TabSecond_GroupOne*     iGroupOne;

    // 排版(垂直排版)
    QVBoxLayout*    iLayout;
};

#endif // TABFIRST_CONFIGBASEINFO_H
