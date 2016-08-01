/*
 * Builder中的第二签，手机操作的鼠标按键映射
 */

#include "TabSecond_OperatorInfo.h"
#include "TabSecond_GroupOne.h"
#include "TabSecond_GroupTwo.h"
#include "TabSecond_GroupThree.h"
#include "TabSecond_GroupFour.h"
#include "TabSecond_GroupFive.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include "EasyXml.h"
#include "AccGroup.h"

TabSecond_OperatorInfo::TabSecond_OperatorInfo(QWidget *parent)
    :QWidget(parent)
{
    iLayout = new QVBoxLayout;

    // 构造Tab
    SetupTab();

    this->setLayout(iLayout);
}

void TabSecond_OperatorInfo::SetupTab()
{
//    iGroupOne = new TabSecond_GroupOne(this);
    iAccGroup = new AccGroup(this);
    iGroupTwo = new TabSecond_GroupTwo(this);
    iGroupThree = new TabSecond_GroupThree(this);
    iGroupFour = new TabSecond_GroupFour(this);
    iGroupFive = new TabSecond_GroupFive(this);
    connect(this, SIGNAL(signalDirChange(ModelDir)),
            iGroupFive, SLOT(slotSetDir(ModelDir)));
    

//    iLayout->addWidget(iGroupOne);

    QHBoxLayout* layout2 = new QHBoxLayout;
    layout2->addWidget(iGroupFour);
    layout2->addWidget(iGroupFive);
    iLayout->addLayout(layout2);

    QHBoxLayout* layout3 = new QHBoxLayout;
    layout3->addWidget(iGroupTwo);
    layout3->addWidget(iGroupThree);
    iLayout->addLayout(layout3);
    
    QHBoxLayout* layout4 = new QHBoxLayout;
    layout4->addWidget(iAccGroup);
    layout4->addStretch();
    layout4->setAlignment(Qt::AlignRight);
    iLayout->addLayout(layout4);
}

void TabSecond_OperatorInfo::Default()
{
//    if(iGroupOne)
//        iGroupOne->Default();
    if(iAccGroup)
        iAccGroup->Default();
    if(iGroupTwo)
        iGroupTwo->Default();
    if(iGroupThree)
        iGroupThree->Default();
    if(iGroupFour)
        iGroupFour->Default();
    if(iGroupFive)
        iGroupFive->Default();
}

void TabSecond_OperatorInfo::ReadData(XEasyXml *xml)
{
//    if(iGroupOne)
//        iGroupOne->ReadData(xml);
    if(iAccGroup)
        iAccGroup->ReadData(xml);
    if(iGroupTwo)
        iGroupTwo->ReadData(xml);
    if(iGroupThree)
        iGroupThree->ReadData(xml);
    if(iGroupFour)
        iGroupFour->ReadData(xml);
    if(iGroupFive)
        iGroupFive->ReadData(xml);
}

void TabSecond_OperatorInfo::SaveData(XEasyXml *xml)
{
//    if(iGroupOne)
//        iGroupOne->SaveData(xml);
    if(iAccGroup)
        iAccGroup->SaveData(xml);
    if(iGroupTwo)
        iGroupTwo->SaveData(xml);
    if(iGroupThree)
        iGroupThree->SaveData(xml);
    if(iGroupFour)
        iGroupFour->SaveData(xml);
    if(iGroupFive)
        iGroupFive->SaveData(xml);
}
