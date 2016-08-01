/*
 * TabSecond中的TouchInfo Group
 */

#include "TabSecond_GroupFive.h"
#include <QListWidget>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include "TabSecond_GroupFive_Dialog.h"
#include "EasyXml.h"
#include "TabSecond_GroupFive_Item.h"
#include "CustomDefine.h"
#include "xtouchpopup_dialog.h"
#include "tabsecond_groupfive_new.h"
#include "XVirtualKeyData.h"
#include "TouchDefine_Dialog.h"
#include "TouchInfo.h"
#include "TouchButtonInfo.h"
#include "XVirtualKeyData.h"
#include "mainwindow.h"
#include <QLineEdit>
#include <QComboBox>

TabSecond_GroupFive::TabSecond_GroupFive(QWidget *parent)
    :QGroupBox(parent)
{
    dir = Dir_Normal;
    
    // 排版
    iLayout = new QVBoxLayout;

    // 构造
    SetupGroup();

    this->setLayout(iLayout);
}

void TabSecond_GroupFive::SetupGroup()
{
    this->setTitle(QWidget::tr("touch_define"));

    SetupMousePos();
    SetupButton();
    SetupButtonbox();
}

void TabSecond_GroupFive::SetupMousePos()
{
    imousePos = new QLabel(this);
    imousePos->setText(QLabel::tr("mousepos"));

    imousePos_list = new QComboBox(this);
    imousePos_list->addItem(QComboBox::tr("false"));
    imousePos_list->addItem(QComboBox::tr("true"));;

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(imousePos);
    layout->addWidget(imousePos_list);

    iLayout->addLayout(layout);
}

void TabSecond_GroupFive::SetupButton()
{
    iButton = new QListWidget(this);

    iLayout->addWidget(iButton);
}

void TabSecond_GroupFive::SetupButtonbox()
{
    iButtonbox = new QDialogButtonBox(this);

    iAdd = new QPushButton(this);
    iAdd->setText(QPushButton::tr("Add && Modify"));
    QObject::connect(iAdd, SIGNAL(clicked()),
                     this, SLOT(AddItem()));
    iButtonbox->addButton(iAdd, QDialogButtonBox::ActionRole);
    
    iDel = new QPushButton(this);
    iDel->setText(QPushButton::tr("Del"));
    QObject::connect(iDel, SIGNAL(clicked()),
                     this, SLOT(DelCurrentItem()));
    iButtonbox->addButton(iDel, QDialogButtonBox::ActionRole);

    iLayout->addWidget(iButtonbox);
}

void TabSecond_GroupFive::Default()
{
    if(imousePos_list)
        imousePos_list->setCurrentIndex(0);

    if(iButton)
    {
        for(int i=iButton->count()-1; i>=0; i--)
        {
            QListWidgetItem* item = iButton->item(i);
            iButton->takeItem(i);
            delete item;
        }
    }
}

void TabSecond_GroupFive::ReadData(XEasyXml *xml)
{
    if(xml)
    {
        QString value;
        value = xml->FindAtt("touch_define", "zmouse_pos");
        if(imousePos_list)
        {
            if(value=="true")
                imousePos_list->setCurrentIndex(1);
            else
                imousePos_list->setCurrentIndex(0);
        }

        int count = 0;
        count = xml->ChildCount("touch_define", "touch_map");
        
        for(int i=0; i<count; i++)
        {
            TouchButtonInfo* item = new TouchButtonInfo;
            
            QString value;
            value = xml->FindAttI("touch_map", "button_name", i);
            item->iButtonName = value;
            
            value = xml->FindAttI("touch_map", "org", i);
            item->iOrg = value;
            
            value = xml->FindAttI("touch_map", "button_hor_pos", i);
            item->iHorPos = value;
            
            value = xml->FindAttI("touch_map", "button_ver_pos", i);
            item->iVerPos = value;
            
            value = xml->FindAttI("touch_map", "button_map_to", i);
            if(value.length()!=0)
            {
                for(int j=0; j<Key_Count; j++)
                {
                    if(value==Key_Code[j])
                    {
                        item->iMapto = Key_Name[j];
                        break;
                    }
                }
            }
            
            value = xml->FindAttI("touch_map", "button_long_press", i);
            item->iLongPress = value;
            
            value = xml->FindAttI("touch_map", "button_long_press_lock_time", i);
            item->iLockTime = value;
                
            QString title;
            title += item->iButtonName;
            title += "\t->\t";
            title += item->iMapto;
            
            item->setText(title);
            
            iButton->addItem(item);
        }
        
        QString dirset = xml->FindAtt("display_define", "screen_direction");
        if(dirset=="Horizon")
            dir = Dir_ACW90;
        else if(dirset=="Vertical")
            dir = Dir_Normal;
    }
}

void TabSecond_GroupFive::SaveData(XEasyXml *xml)
{
    if(xml)
    {
        if(!xml->HasEle("operate_define", "touch_define"))
            xml->AddEle("operate_define", "touch_define");
        if(!xml->HasEle("operate_define", "touch_define"))
            return;
        
        // buttontype
        if(xml->HasAtt("touch_define", "zmouse_pos"))
            xml->DelAtt("touch_define", "zmouse_pos");
        if(!xml->HasAtt("touch_define", "zmouse_pos"))
            xml->AddAtt("touch_define", "zmouse_pos");
        if(!xml->HasAtt("touch_define", "zmouse_pos"))
            return;

        if(imousePos_list)
        {
            QString value;
            switch(imousePos_list->currentIndex())
            {
            case 1:
                value = "true";
                break;
            case 0:
                defalut:
                value = "false";
                break;
            }
            xml->ModifyAtt("touch_define", "zmouse_pos", value);
        }

        while(xml->HasEle("touch_define", "touch_map"))
            xml->DelEle("touch_define", "touch_map");
        
        if(iButton)
        {
            int count = 0;
            count = iButton->count();
            
            for(int i=0; i<count; i++)
            {
                xml->AddEle("touch_define", "touch_map");
                
                xml->AddAttI("touch_map", "button_name", ((TouchButtonInfo*)(iButton->item(i)))->iButtonName, i);
                xml->AddAttI("touch_map", "org", ((TouchButtonInfo*)(iButton->item(i)))->iOrg, i);
                xml->AddAttI("touch_map", "button_hor_pos",((TouchButtonInfo*)(iButton->item(i)))->iHorPos, i);
                xml->AddAttI("touch_map", "button_ver_pos", ((TouchButtonInfo*)(iButton->item(i)))->iVerPos, i);
                for(int j=0; j<Key_Count; j++)
                {
                    if(((TouchButtonInfo*)(iButton->item(i)))->iMapto==Key_Name[j])
                    {
                        xml->AddAttI("touch_map", "button_map_to", Key_Code[j], i);
                        break;
                    }
                }
                
                xml->AddAttI("touch_map", "button_long_press", ((TouchButtonInfo*)(iButton->item(i)))->iLongPress, i);
                xml->AddAttI("touch_map", "button_long_press_lock_time", ((TouchButtonInfo*)(iButton->item(i)))->iLockTime, i);
            }
        }
    }
    


}

void TabSecond_GroupFive::AddItem()
{
    iDialog = new TouchDefine_Dialog(this, dir);

    TouchInfo* info = new TouchInfo;
    for(int i=0; i<iButton->count(); i++)
    {
        info->iButtonInfoList.append(((TouchButtonInfo*)(iButton->item(i))));
    }

    iDialog->ModifyItem_Item(iButton, info);
}

void TabSecond_GroupFive::ModifyItem()
{
    iDialog = new TouchDefine_Dialog(this, dir);

    TouchInfo* info = new TouchInfo;
    for(int i=0; i<iButton->count(); i++)
    {
        info->iButtonInfoList.append(((TouchButtonInfo*)(iButton->item(i))));
    }
    iDialog->ModifyItem_Item(iButton, info);
}

void TabSecond_GroupFive::ModifyItem_list(QListWidgetItem *item)
{
    iDialog = new TouchDefine_Dialog(this, dir);
    TouchInfo* info = new TouchInfo;
    for(int i=0; i<iButton->count(); i++)
    {
        info->iButtonInfoList.append(((TouchButtonInfo*)(iButton->item(i))));
    }
    iDialog->ModifyItem_Item(iButton, info);
}

void TabSecond_GroupFive::DelCurrentItem()
{
    QListWidgetItem* item = iButton->takeItem(iButton->currentRow());
    delete item;
}
