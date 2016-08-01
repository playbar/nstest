#include "AccGroup.h"
#include <QListWidget>
#include <QBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include "AccDialog.h"
#include "EasyXml.h"
#include "AccGroup_Item.h"
#include "XVirtualKeyData.h"

AccGroup::AccGroup(QWidget *parent) :
    QGroupBox(parent)
{
    this->setTitle(tr("Acc"));
    
    iDialog = new AccDialog(this);
    
    iLayout = new QVBoxLayout;
    
    CreateGroup();
    
    this->setLayout(iLayout);
}

void AccGroup::Default()
{
    if(iAccs)
    {
        for(int i=iAccs->count()-1;i>=0;i--)
        {
            QListWidgetItem* item = iAccs->takeItem(i);
            delete item;
        }
    }
}

void AccGroup::ReadData(XEasyXml *xml)
{
    if(xml)
    {
        int count = 0;
        count = xml->ChildCount("operate_define", "acc");
        
        for(int i=0; i<count; i++)
        {
            AccGroup_Item* item = new AccGroup_Item;
            if(item)
            {
                // 方向
                QString value;
                value = xml->FindAttI("acc", "direction", i);
                if(value.length()!=0)
                    item->iDirection = value;
                
                // accspeed
                value = xml->FindAttI("acc", "accspeed", i);
                if(value.length()!=0)
                    item->iAccSpeed = value;
                
                // step
                value = xml->FindAttI("acc", "step", i);
                if(value.length()!=0)
                    item->iStep = value;
                
                // mapto
                value = xml->FindAttI("acc", "key_map_to", i);
                if(value.length()!=0)
                {
                    for(int j=0; i<Key_Count; j++)
                    {
                        if(value==Key_Code[j])
                        {
                            item->BMapto = true;
                            item->iMapto = Key_Name[j];
                            break;
                        }
                    }
                }
                
                // mouse_act
                value = xml->FindAttI("acc", "mouse_act", i);
                if(value.length()!=0)
                {
                    item->BMouseAct = true;
                    item->iMouseAct = value;
                }
                
                // OnlyOne
                value = xml->FindAttI("acc", "onlyone", i);
                if(value.length()!=0)
                {
                    item->iOnlyOne = value;
                }
                
                QString itemtitle;
                itemtitle += item->iDirection;
                itemtitle += QString("\t->\t");
                if(item->BMapto)
                    itemtitle += item->iMapto;
                else
                    itemtitle += item->iMouseAct;
                item->setText(itemtitle);
                
                iAccs->addItem(item);
            }
        }
    }
}

void AccGroup::SaveData(XEasyXml *xml)
{
    if(xml)
    {
        if(!xml->HasEle("defines", "operate_define"))
            xml->AddEle("defines", "operate_define");
        if(!xml->HasEle("defines", "operate_define"))
            return;
        
        // 清理
        while(xml->HasEle("operate_define", "acc"))
            xml->DelEle("operate_define", "acc");
        
        if(iAccs)
        {
            // 添加已经定义的重力配置
            int count = iAccs->count();
            for(int i=0; i<count; i++)
            {
                xml->AddEle("operate_define", "acc");
                
                xml->AddAttI("acc", "direction", ((AccGroup_Item*)iAccs->item(i))->iDirection, i);
                
                xml->AddAttI("acc", "accspeed", ((AccGroup_Item*)iAccs->item(i))->iAccSpeed, i);
                
                xml->AddAttI("acc", "step", ((AccGroup_Item*)iAccs->item(i))->iStep, i);
                
                if(((AccGroup_Item*)iAccs->item(i))->BMapto)
                {
                    for(int j=0; j<Key_Count; j++)
                    {
                        if(((AccGroup_Item*)iAccs->item(i))->iMapto==Key_Name[j])
                        {
                            xml->AddAttI("acc", "key_map_to", Key_Code[j], i);
                            break;
                        }
                    }
                }
                else
                {
                    xml->AddAttI("acc", "mouse_act", ((AccGroup_Item*)iAccs->item(i))->iMouseAct, i);
                }
                
                xml->AddAttI("acc", "onlyone", ((AccGroup_Item*)iAccs->item(i))->iOnlyOne, i);
                
            }
        }
    }
}


// slots
// 添加ITEM
void AccGroup::AddItem()
{
    if(iDialog)
    {
        iDialog->ModifyItem_Item(iAccs);
    }
}

void AccGroup::ModifyItem_list(QListWidgetItem *item)
{
    if(iDialog)
    {
        iDialog->ModifyItem_Item(iAccs, item);
    }
}

void AccGroup::DelCurrentItem()
{
    QListWidgetItem* item = iAccs->takeItem(iAccs->currentRow());
    delete item;
}

void AccGroup::CreateGroup()
{
    this->setWindowTitle(tr("Accs"));
    
    CreateAccs();
    CreateButtonBox();
}

void AccGroup::CreateAccs()
{
    iAccs = new QListWidget(this);
    // 关联双击
    connect(iAccs, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(ModifyItem_list(QListWidgetItem*)));
    
    iLayout->addWidget(iAccs);
}

void AccGroup::CreateButtonBox()
{
    iButtonBox = new QDialogButtonBox(this);
    iAdd = new QPushButton(tr("Add"));
    connect(iAdd, SIGNAL(clicked()),
            this, SLOT(AddItem()));
    iButtonBox->addButton(iAdd, QDialogButtonBox::ActionRole);
    
    iDel = new QPushButton(tr("Del"));
    connect(iDel, SIGNAL(clicked()),
            this, SLOT(DelCurrentItem()));
    iButtonBox->addButton(iDel, QDialogButtonBox::ActionRole);
    
    iLayout->addWidget(iButtonBox);
}
