/*
 * Builder中Tab的第一个签，配置文件的基本信息及鼠标操作的基本信息
 */

#include "TabFirst_ConfigBaseInfo.h"
#include <QVBoxLayout>
#include "TabFirst_GroupOne.h"
#include "TabFirst_GroupTwo.h"
#include "EasyXml.h"
#include "CustomDefine.h"
#include "TabSecond_GroupOne.h"

TabFirst_ConfigBaseInfo::TabFirst_ConfigBaseInfo(QWidget *parent)
    :QWidget(parent)
{
    // 排版
    iLayout = new QVBoxLayout;

    // 构造
    SetupTab();

    this->setLayout(iLayout);
}

void TabFirst_ConfigBaseInfo::SetupTab()
{
    iGroupOne = new TabSecond_GroupOne(this);
    
    iConfigInfo = new TabFirst_GroupOne(this);
    connect(iConfigInfo, SIGNAL(signalUpdatePathItoO(QString)),
            this, SLOT(slotUpdatePathItoO(QString)));
    
    connect(this, SIGNAL(signalUpdatePathOtoI(QString)),
            iConfigInfo, SLOT(slotUpdatePathOtoI(QString)));

    iMouseInfo = new TabFirst_GroupTwo(this);


    iLayout->addWidget(iConfigInfo);
    iLayout->addWidget(iMouseInfo);
    iLayout->addWidget(iGroupOne);
}

void TabFirst_ConfigBaseInfo::Default()
{

    if(iConfigInfo)
        iConfigInfo->Default();
    if(iMouseInfo)
        iMouseInfo->Default();
    if(iGroupOne)
        iGroupOne->Default();
}

void TabFirst_ConfigBaseInfo::ReadData(XEasyXml *xml)
{
    if(iConfigInfo)
        iConfigInfo->ReadData(xml);
    if(iMouseInfo)
        iMouseInfo->ReadData(xml);
    if(iGroupOne)
        iGroupOne->ReadData(xml);
}

void TabFirst_ConfigBaseInfo::SaveData(XEasyXml *xml)
{
    if(xml)
    {
        xml->SetVersion("1.0");
        xml->SetEncoding("GB2312");
        xml->SetRootName("config");
        xml->SetDtdLocation("config.dtd");

        if(!xml->HasEle("", "defines"))
            xml->AddEle("", "defines");
    }

    if(iConfigInfo)
        iConfigInfo->SaveData(xml);

    if(iMouseInfo)
        iMouseInfo->SaveData(xml);
    
    if(iGroupOne)
        iGroupOne->SaveData(xml);
}
