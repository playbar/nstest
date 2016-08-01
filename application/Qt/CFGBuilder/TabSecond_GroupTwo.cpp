/*
 * TabSecond中的NormalMap Group
 */

#include "TabSecond_GroupTwo.h"
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QTextCodec>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QDialogButtonBox>
#include "TabSecond_GroupTwo_Dialog.h"
#include "TabSecond_GroupTwo_Item.h"
#include "EasyXml.h"
#include "CustomDefine.h"
#include "XVirtualKeyData.h"

TabSecond_GroupTwo::TabSecond_GroupTwo(QWidget *parent)
    :QGroupBox(parent)
{
    iDialog = new TabSecond_GroupTwo_Dialog(this);

    // 排版
    iLayout = new QVBoxLayout;

    // 构造
    SetupGroup();

    this->setLayout(iLayout);
}

void TabSecond_GroupTwo::SetupGroup()
{
    this->setTitle(QWidget::tr("mouse_normal_define"));

    SetupName();
    SetupNormal();
    SetupButtonBox();
}

void TabSecond_GroupTwo::SetupName()
{
    iname = new QLabel(this);
    iname->setText(QLabel::tr("name"));

    iname_input = new QLineEdit(this);
    iname_input->setText(QLabel::tr("normal"));

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(iname);
    layout->addWidget(iname_input);

    iLayout->addLayout(layout);
}

void TabSecond_GroupTwo::SetupNormal()
{
    inormal = new QListWidget(this);

    // 关联双击ITEM及对话框
    QObject::connect(inormal, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                     this, SLOT(ModifyItem_list(QListWidgetItem*)));

    iLayout->addWidget(inormal);
}

void TabSecond_GroupTwo::SetupButtonBox()
{
    iButtonBox = new QDialogButtonBox(this);

    iAdd = new QPushButton(this);
    iAdd->setText(QPushButton::tr("Add"));
    QObject::connect(iAdd, SIGNAL(clicked()),
                     this, SLOT(AddItem()));
    iButtonBox->addButton(iAdd, QDialogButtonBox::ActionRole);

    iDel = new QPushButton(this);
    iDel->setText(QPushButton::tr("Del"));
    QObject::connect(iDel, SIGNAL(clicked()),
                     this, SLOT(DelCurrentItem()));
    iButtonBox->addButton(iDel, QDialogButtonBox::ActionRole);

    iLayout->addWidget(iButtonBox);
}

void TabSecond_GroupTwo::Default()
{
    if(iname_input)
        iname_input->setText(QLineEdit::tr("normal"));
    if(inormal)
    {
        for(int i=inormal->count()-1; i>=0; i--)
        {
            QListWidgetItem* item = inormal->item(i);
            inormal->takeItem(i);
            delete item;
        }
    }
}

void TabSecond_GroupTwo::ReadData(XEasyXml *xml)
{
    if(xml)
    {
        // 鼠标模式名称
        QString value;
        value = xml->FindAtt("mouse_normal_define", "name");
        if(iname_input)
        {
            iname_input->setText(value);
        }

        // 虚拟按键列表
        int count = 0;
        count = xml->ChildCount("mouse_normal_define", "mouse_normal_map");

        for(int i=0; i<count; i++)
        {
            TabSecond_GroupTwo_Item* item = new TabSecond_GroupTwo_Item;
            if(item)
            {
                value = xml->FindAttI("mouse_normal_map", "virtual_key", i);
                if(value.length()!=0)
                {
                    for(int j=0; j<Key_Count; j++)
                    {
                        if(value==Key_Code[j])
                        {
                            item->SetVirtualKey(Key_Name[j]);
                            break;
                        }
                    }
                }
//                    item->SetVirtualKey(value);
                else
                {
                    delete item;
                    continue;
                }

                value = xml->FindAttI("mouse_normal_map", "mouse_act", i);
                if(value.length()!=0)
                    item->SetMouseAct(value);
                else
                {
                    delete item;
                    continue;
                }

                QString itemtitle;
                itemtitle += ((TabSecond_GroupTwo_Item*)item)->VirtualKey();
                itemtitle += QString("\t->\t");
                itemtitle += ((TabSecond_GroupTwo_Item*)item)->MouseAct();
                item->setText(itemtitle);

                inormal->addItem(item);
            }
        }
    }
}

void TabSecond_GroupTwo::SaveData(XEasyXml *xml)
{
    if(xml)
    {
        if(!xml->HasEle("operate_define", "mouse_normal_define"))
            xml->AddEle("operate_define", "mouse_normal_define");
        if(!xml->HasEle("operate_define", "mouse_normal_define"))
            return;

        // 鼠标模式名称
        if(xml->HasAtt("mouse_normal_define", "name"))
            xml->DelAtt("mouse_normal_define", "name");

        if(!xml->HasAtt("mouse_normal_define", "name"))
            xml->AddAtt("mouse_normal_define", "name");
        if(!xml->HasAtt("mouse_normal_define", "name"))
            return;

        if(iname_input)
        {
//            if(!xml->HasAtt("mouse_normal_define", "name"))
//                xml->AddAtt("mouse_normal_define", "name");
            xml->ModifyAtt("mouse_normal_define", "name", iname_input->text());
        }

        // 清理引擎中已存在的映射
        while(xml->HasEle("mouse_normal_define", "mouse_normal_map"))
            xml->DelEle("mouse_normal_define", "mouse_normal_map");

        // 鼠标通常模式映射
        if(inormal)
        {
            // 添加列表中定义的映射
            int count = inormal->count();
            for(int i=0; i<count; i++)
            {
                xml->AddEle("mouse_normal_define", "mouse_normal_map");
                for(int j=0; j<Key_Count; j++)
                {
                    if(((TabSecond_GroupTwo_Item*)(inormal->item(i)))->VirtualKey()==Key_Name[j])
                    {
                        xml->AddAttI("mouse_normal_map", "virtual_key", Key_Code[j], i);
                        break;
                    }
                }

//                xml->AddAttI("mouse_normal_map", "virtual_key", ((TabSecond_GroupTwo_Item*)(inormal->item(i)))->VirtualKey(), i);
                xml->AddAttI("mouse_normal_map", "mouse_act", ((TabSecond_GroupTwo_Item*)(inormal->item(i)))->MouseAct(), i);
            }
        }
    }
}

void TabSecond_GroupTwo::AddItem()
{
    if(iDialog!=NULL)
        iDialog->ModifyItem_Item(inormal);
}

void TabSecond_GroupTwo::DelCurrentItem()
{
    QListWidgetItem* item = inormal->takeItem(inormal->currentRow());
    delete item;
}

void TabSecond_GroupTwo::ModifyItem_list(QListWidgetItem *item)
{
    if(iDialog!=NULL)
        iDialog->ModifyItem_Item(inormal, item);
}
