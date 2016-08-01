/*
 * TabSecond中的KeyInfo Group
 */

#include "TabSecond_GroupFour.h"
#include <QListWidget>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include "TabSecond_GroupFour_Dialog.h"
#include "TabSecond_GroupFour_Item.h"
#include "EasyXml.h"
#include "CustomDefine.h"
#include "XVirtualKeyData.h"
#include <QLabel>
#include <QLineEdit>

TabSecond_GroupFour::TabSecond_GroupFour(QWidget *parent)
    :QGroupBox(parent)
{
    iDialog = new TabSecond_GroupFour_Dialog(this);

    // 排版
    iLayout = new QVBoxLayout;

    // 构造Group
    SetupGroup();

    this->setLayout(iLayout);
}

void TabSecond_GroupFour::SetupGroup()
{
    this->setTitle(QWidget::tr("key_define"));

    SetupKey();
    SetupButtonBox();
}


void TabSecond_GroupFour::SetupKey()
{
    iKey = new QListWidget(this);

    // 关联双击查看及KeyInfo对话框
    QObject::connect(iKey, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                     this, SLOT(ModifyItem_list(QListWidgetItem*)));

    iLayout->addWidget(iKey);
}

void TabSecond_GroupFour::SetupButtonBox()
{
    iButtonbox = new QDialogButtonBox(this);

    iAdd = new QPushButton(this);
    iAdd->setText(QPushButton::tr("Add"));
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

void TabSecond_GroupFour::Default()
{
    if(iKey)
    {
        for(int i=iKey->count()-1; i>=0; i--)
        {
            QListWidgetItem* item = iKey->item(i);
            iKey->takeItem(i);
            delete item;
        }
    }
}

void TabSecond_GroupFour::ReadData(XEasyXml *xml)
{
    if(xml)
    {
        int count = 0;
        count = xml->ChildCount("key_define", "key_map");

        for(int i=0; i<count; i++)
        {
            TabSecond_GroupFour_Item* item = new TabSecond_GroupFour_Item;
            if(item)
            {
                // 被按下的手机按键
                QString value;
                value = xml->FindAttI("key_map", "key_pressed", i);
                if(value.length()!=0)
                    item->SetKeypress(value);
                else
                {
                    delete item;
                    continue;
                }

                // 映射按键
                value = xml->FindAttI("key_map", "key_map_to", i);
                if(value.length()!=0)
                {
                    for(int j=0; j<Key_Count; j++)
                    {
                        if(value==Key_Code[j])
                        {
                            item->SetKeymapto(Key_Name[j]);
                            break;
                        }
                    }
//                    item->SetKeymapto(value);
                }
                else
                {
                    delete item;
                    continue;
                }

                // 是否支持长按锁定
                value = xml->FindAttI("key_map", "long_press", i);
                if(value=="true")
                    item->SetLongpress("true");
                else
                    item->SetLongpress("false");

                // 长按锁定时间
                value = xml->FindAttI("key_map", "long_press_lock_time", i);
                item->SetLocktime(value);

                QString itemtitle;
                itemtitle += ((TabSecond_GroupFour_Item*)item)->Keypress();
                itemtitle += QString("\t->\t");
                itemtitle += ((TabSecond_GroupFour_Item*)item)->Keymapto();
                item->setText(itemtitle);

                iKey->addItem(item);
            }
        }
    }
}

void TabSecond_GroupFour::SaveData(XEasyXml *xml)
{
    if(xml)
    {
        if(!xml->HasEle("operate_define", "key_define"))
            xml->AddEle("operate_define", "key_define");
        if(!xml->HasEle("operate_define", "key_define"))
            return;

        // 清理引擎中存在的映射
        while(xml->HasEle("key_define", "key_map"))
            xml->DelEle("key_define", "key_map");

        if(iKey)
        {
            // 添加列表中定义的映射
            int count = iKey->count();
            for(int i=0; i<count; i++)
            {
                xml->AddEle("key_define", "key_map");

                xml->AddAttI("key_map", "key_pressed", ((TabSecond_GroupFour_Item*)(iKey->item(i)))->Keypress(), i);

                for(int j=0; j<Key_Count; j++)
                {
                    if(((TabSecond_GroupFour_Item*)(iKey->item(i)))->Keymapto()==Key_Name[j])
                    {
                        xml->AddAttI("key_map", "key_map_to", Key_Code[j], i);
                        break;
                    }
                }

//                xml->AddAttI("key_map", "key_map_to", ((TabSecond_GroupFour_Item*)(iKey->item(i)))->Keymapto(), i);

                if(((TabSecond_GroupFour_Item*)(iKey->item(i)))->Longpress().length()!=0)
                {
                    if(((TabSecond_GroupFour_Item*)(iKey->item(i)))->Longpress()=="true")
                        xml->AddAttI("key_map", "long_press", ((TabSecond_GroupFour_Item*)(iKey->item(i)))->Longpress(), i);
                }

                if(((TabSecond_GroupFour_Item*)(iKey->item(i)))->Locktime().length()!=0)
                    xml->AddAttI("key_map", "long_press_lock_time", ((TabSecond_GroupFour_Item*)(iKey->item(i)))->Locktime(), i);
            }
        }
    }
}

void TabSecond_GroupFour::AddItem()
{
    if(iDialog!=NULL)
    {
        iDialog->ModifyItem_Item(iKey);
    }
}

void TabSecond_GroupFour::ModifyItem_list(QListWidgetItem *item)
{
    if(iDialog!=NULL)
    {
        iDialog->ModifyItem_Item(iKey, item);
    }
}

void TabSecond_GroupFour::DelCurrentItem()
{
    QListWidgetItem* item = iKey->takeItem(iKey->currentRow());
    delete item;
}
