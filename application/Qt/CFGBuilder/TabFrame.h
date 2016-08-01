/*
 * Builder主界面的Tab
 */

#ifndef TABFRAME_H
#define TABFRAME_H

// declearation
class TabFirst_ConfigBaseInfo;
class TabSecond_OperatorInfo;
class TabThird_OtherInfo;
class TabFourth_Preview;
class XEasyXml;

// include
#include <QTabWidget>
#include "TabFourth_Preview.h"
#include "TabSecond_OperatorInfo.h"

class TabFrame : public QTabWidget
{
    Q_OBJECT

public:
    TabFrame(QWidget* parent = NULL);

private:
    // 构造Tab
    void SetupTab();

    // 第一签
    // 构造
    void SetupTabFirst();

    // 第二签
    // 构造
    void SetupTabSecond();

    // 第三签
    // 构造
    void SetupTabThird();

    // 第四签
    // 构造
    void SetupTabFourth();


    
private slots:
//    // 生成预览信息
//    void CreatePreview(int index);
    void slotDirChange(ModelDir d)
    {
        emit signalDirChange(d);
    }
    
    void slotUpdatePathItoO(QString path)
    {
        emit signalUpdatePathItoO(path);
    }
    
    void slotUpdatePathOtoI(QString path)
    {
        emit signalUpdatePathOtoI(path);
    }
    
signals:
    void signalDirChange(ModelDir);
    
    void signalUpdatePathOtoI(QString);
    void signalUpdatePathItoO(QString);

public:

    void SetMainWindowForPreview(MainWindow* MainWindow)
    {
        if(iTabFourth)
            iTabFourth->SetMainWindow(MainWindow);
        if(iTabSecond)
            iTabSecond->SetMainWindow(MainWindow);
    }

    // 恢复默认
    void Default();

    // 读取数据
    void ReadData(XEasyXml* xml);

    // 保存数据
    void SaveData(XEasyXml* xml);

private:
    // 第一签 基本信息
    TabFirst_ConfigBaseInfo*    iTabFirst;

    // 第二签 操作信息
    TabSecond_OperatorInfo*     iTabSecond;

    // 第三签 其他信息
    TabThird_OtherInfo*         iTabThird;

    // 第四签 预览
    TabFourth_Preview*          iTabFourth;
};

#endif // TABFRAME_H
