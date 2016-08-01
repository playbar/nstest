/*
 * TabFirst中的MouseBaseInfo Group
 */

#include "TabFirst_GroupTwo.h"
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QTextCodec>
#include <QGridLayout>
#include "EasyXml.h"
#include "CustomDefine.h"

TabFirst_GroupTwo::TabFirst_GroupTwo(QWidget *parent)
    : QGroupBox(parent)
{
    // 排版
    iLayout = new QGridLayout;

    // 构造Group
    SetupGroup();

    this->setLayout(iLayout);
}

void TabFirst_GroupTwo::SetupGroup()
{
    this->setTitle(QWidget::tr("MouseInfo"));

    SetupMousemode();
    SetupMousemodechange();
}

void TabFirst_GroupTwo::SetupMousemode()
{
    iMousemode = new QLabel(this);
    iMousemode->setText(QLabel::tr("default_operate_mode"));

    iMousemode_input = new QLineEdit(this);
    iMousemode_input->setAlignment(Qt::AlignRight);

    iLayout->addWidget(iMousemode, 0, 0);
    iLayout->addWidget(iMousemode_input, 0, 1);
}

void TabFirst_GroupTwo::SetupMousemodechange()
{
    iMousemodechange = new QLabel(this);
    iMousemodechange->setText(QLabel::tr("hotkey_for_zoom"));

    iMousemodechange_input = new QLineEdit(this);
    iMousemodechange_input->setAlignment(Qt::AlignRight);
    iMousemodechange_input->setText("#");

    iLayout->addWidget(iMousemodechange, 0, 2);
    iLayout->addWidget(iMousemodechange_input, 0, 3);
}

void TabFirst_GroupTwo::Default()
{
    if(iMousemode_input)
        iMousemode_input->clear();
    if(iMousemodechange_input)
        iMousemodechange_input->setText("#");
}

void TabFirst_GroupTwo::ReadData(XEasyXml *xml)
{
    if(xml)
    {
        // 鼠标操作模式
        QString value;
        value = xml->FindAtt("file_info", "default_operate_mode");

        if(iMousemode_input)
            iMousemode_input->setText(value);

        value = xml->FindAtt("file_info", "hotkey_for_zoom");
            iMousemodechange_input->setText(value);
    }
}

void TabFirst_GroupTwo::SaveData(XEasyXml *xml)
{
    if(xml)
    {
        if(!xml->HasEle("defines", "file_info"))
            return;

        // 鼠标操作模式
        if(xml->HasAtt("file_info", "default_operate_mode"))
            xml->DelAtt("file_info", "default_operate_mode");

        if(iMousemode_input)
        {
            if(!xml->HasAtt("file_info", "default_operate_mode"))
                xml->AddAtt("file_info", "default_operate_mode");
            if(!xml->HasAtt("file_info", "default_operate_mode"))
                return;

            xml->ModifyAtt("file_info", "default_operate_mode", iMousemode_input->text());
        }

        // 模式切换热键
        if(xml->HasAtt("file_info", "hotkey_for_zoom"))
            xml->DelAtt("file_info", "hotkey_for_zoom");

        if(iMousemodechange_input)
        {
            if(!xml->HasAtt("file_info", "hotkey_for_zoom"))
                xml->AddAtt("file_info", "hotkey_for_zoom");
            if(!xml->HasAtt("file_info", "hotkey_for_zoom"))
                return;

            xml->ModifyAtt("file_info", "hotkey_for_zoom", iMousemodechange_input->text());
        }
    }
}
