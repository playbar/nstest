/*
 * Builder主界面的Tab
 */

#include "TabFrame.h"
#include "TabFirst_ConfigBaseInfo.h"
#include "TabSecond_OperatorInfo.h"
#include "TabThird_OtherInfo.h"
#include "TabFourth_Preview.h"
#include "CustomDefine.h"

TabFrame::TabFrame(QWidget *parent)
    :QTabWidget(parent)
{
    // 构造
    SetupTab();
//    this->setTabPosition(QTabWidget::West);
//    this->setTabShape(QTabWidget::Triangular);
    this->setMovable(true);
//    this->setUsesScrollButtons(true);
}

void TabFrame::SetupTab()
{
    SetupTabFirst();
    SetupTabSecond();
    SetupTabThird();
    SetupTabFourth();
}

void TabFrame::SetupTabFirst()
{
    iTabFirst = new TabFirst_ConfigBaseInfo(this);
    connect(iTabFirst, SIGNAL(signalUpdatePathItoO(QString)),
            this, SLOT(slotUpdatePathItoO(QString)));
    
    connect(this, SIGNAL(signalUpdatePathOtoI(QString)),
            iTabFirst, SLOT(slotUpdatePathOtoI(QString)));
//    connect(this, SIGNAL(signalUpdatePath(QString)),
//            iTabFirst, SLOT(slotUpdatePath(QString)));
//    connect(iTabFirst, SIGNAL(signalUpdatePath(QString)),
//            this, SLOT(slotUpdatePath(QString)));

    this->addTab(iTabFirst, tr("BaseInfo"));
}

void TabFrame::SetupTabSecond()
{
    iTabSecond = new TabSecond_OperatorInfo(this);
    connect(this, SIGNAL(signalDirChange(ModelDir)),
            iTabSecond, SLOT(slotDirChange(ModelDir)));

    this->addTab(iTabSecond, tr("OperateInfo"));
}

void TabFrame::SetupTabThird()
{
    iTabThird = new TabThird_OtherInfo(this);
    connect(iTabThird, SIGNAL(signalDirChange(ModelDir)),
            this, SLOT(slotDirChange(ModelDir)));

    this->addTab(iTabThird, tr("OtherInfo"));
}

void TabFrame::SetupTabFourth()
{
    iTabFourth = new TabFourth_Preview(this);
//    connect(iTabFourth, SIGNAL(signalDirChange(ModelDir)),
//            this, SLOT(slotDirChange(ModelDir)));

    this->addTab(iTabFourth, tr("Preview"));

}

//void TabFrame::CreatePreview(int index)
//{
//    if(index==this->indexOf(iTabFourth))
//    {
//        // 生成预览信息
//        QString str("test");

//        // 传递给Preview
//        if(iTabFourth!=NULL)
//        {
//            iTabFourth->SetPreview(str);
//        }
//    }
//}

//void TabFrame::SetPreview(const QString &str)
//{
//    if(iTabFourth!=NULL)
//    {
//        iTabFourth->SetPreview(str);
//    }
//}

void TabFrame::Default()
{
    if(iTabFirst)
        iTabFirst->Default();
    if(iTabSecond)
        iTabSecond->Default();
    if(iTabThird)
        iTabThird->Default();
    if(iTabFourth)
        iTabFourth->Default();
}

void TabFrame::ReadData(XEasyXml* xml)
{
    if(iTabFirst)
        iTabFirst->ReadData(xml);
    if(iTabSecond)
        iTabSecond->ReadData(xml);
    if(iTabThird)
        iTabThird->ReadData(xml);
//    if(iTabFourth)
//        iTabFourth->ReadData(xml);
}

void TabFrame::SaveData(XEasyXml *xml)
{
    if(iTabFirst)
        iTabFirst->SaveData(xml);
    if(iTabSecond)
        iTabSecond->SaveData(xml);
    if(iTabThird)
        iTabThird->SaveData(xml);
}
