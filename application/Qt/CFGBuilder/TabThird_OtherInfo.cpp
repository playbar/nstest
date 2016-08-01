/*
 * Builder中Tab的第三个签，屏幕显示，声音，网络，文本信息
 */

#include "TabThird_OtherInfo.h"
#include "TabThird_GroupOne.h"
#include "TabThird_GroupTwo.h"
#include "TabThird_GroupThree.h"
#include "TabThird_GroupFour.h"
#include <QGridLayout>
#include "EasyXml.h"

TabThird_OtherInfo::TabThird_OtherInfo(QWidget *parent)
    :QWidget(parent)
{
    // 排版
    iLayout = new QGridLayout;
    iLayout->setColumnStretch(2, 2);

    // 构造
    SetupTab();

    this->setLayout(iLayout);
}

void TabThird_OtherInfo::SetupTab()
{
    iGroupOne = new TabThird_GroupOne(this);
    connect(iGroupOne, SIGNAL(signalDirChange(ModelDir)),
            this, SLOT(slotDirChange(ModelDir)));
    iGroupTwo = new TabThird_GroupTwo(this);
    iGroupThree = new TabThird_GroupThree(this);
    iGroupFour = new TabThird_GroupFour(this);

    iLayout->addWidget(iGroupOne, 0, 0);
    iLayout->addWidget(iGroupTwo, 0, 1);
    iLayout->addWidget(iGroupThree, 1, 0);
    iLayout->addWidget(iGroupFour, 1, 1);
}

void TabThird_OtherInfo::Default()
{
    if(iGroupOne)
        iGroupOne->Default();
    if(iGroupTwo)
        iGroupTwo->Default();
    if(iGroupThree)
        iGroupThree->Default();
    if(iGroupFour)
        iGroupFour->Default();
}

void TabThird_OtherInfo::ReadData(XEasyXml *xml)
{
    if(iGroupOne)
        iGroupOne->ReadData(xml);
    if(iGroupTwo)
        iGroupTwo->ReadData(xml);
    if(iGroupThree)
        iGroupThree->ReadData(xml);
    if(iGroupFour)
        iGroupFour->ReadData(xml);
}

void TabThird_OtherInfo::SaveData(XEasyXml *xml)
{
    if(iGroupOne)
        iGroupOne->SaveData(xml);
    if(iGroupTwo)
        iGroupTwo->SaveData(xml);
    if(iGroupThree)
        iGroupThree->SaveData(xml);
    if(iGroupFour)
        iGroupFour->SaveData(xml);
}
