/*
 * TabFirst中的ConfigBaseInfo Group
 */

#include "TabFirst_GroupOne.h"
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include "EasyXml.h"
#include "CustomDefine.h"
#include <QSpinBox>
#include "xpopupcontentid_lineedit.h"
#include <QCheckBox>

TabFirst_GroupOne::TabFirst_GroupOne(QWidget *parent)
    : QGroupBox(parent)
{
    // 排版
    iLayout = new QGridLayout;

    // 构造Group
    SetupGroup();

    this->setLayout(iLayout);
}

void TabFirst_GroupOne::Default()
{
    if(icontent_id_input)
        icontent_id_input->clear();
//    if(icontent_level_box)
//        icontent_level_box->clear();
    if(icontent_level_input)
        icontent_level_input->clear();
    if(ioperator_type_list)
        ioperator_type_list->setCurrentIndex(0);
    if(icontent_type_list)
        icontent_type_list->setCurrentIndex(0);
}

void TabFirst_GroupOne::ReadData(XEasyXml* xml)
{
    if(xml)
    {
        // FLASH内容ID
        QString value;
        value = xml->FindAtt("file_info", "content_ID");

        if(icontent_id_input)
        {
            if(value.length()!=0)
                icontent_id_input->setText(value);
            else
                icontent_id_input->clear();
        }

        // FLASH内容等级
        value = xml->FindAtt("file_info", "content_level");
//        if(icontent_level_box)
//        {
//            if(value.length()!=0)
//                icontent_level_box->setValue(value.toInt());
//            else
//                icontent_level_box->clear();
//        }
        if(icontent_level_input)
        {
            if(value.length()!=0)
                icontent_level_input->setText(value);
            else
                icontent_level_input->clear();
        }

        // content_type
        value = xml->FindAtt("file_info", "zcontent_type");
        if(icontent_type_list)
        {
            if(value=="media")
                icontent_type_list->setCurrentIndex(1);
            else
                icontent_type_list->setCurrentIndex(0);
        }

        // 手机操作类型
        value = xml->FindAtt("file_info", "mobile_operate_type");
        if(ioperator_type_list)
        {
            if(value=="Nontouch_Numpad")
                ioperator_type_list->setCurrentIndex(0);
            else if(value=="Nontouch_Fullpad")
                ioperator_type_list->setCurrentIndex(1);
            else if(value==QString("Touch_Nopad"))
                ioperator_type_list->setCurrentIndex(2);
            else if(value==QString("Touch_Numpad"))
                ioperator_type_list->setCurrentIndex(3);
            else if(value==QString("Touch_Fullpad"))
                ioperator_type_list->setCurrentIndex(4);
            else
                ioperator_type_list->setCurrentIndex(0);
        }
    }
}

void TabFirst_GroupOne::SaveData(XEasyXml *xml)
{
    if(xml)
    {
        if(!xml->HasEle("defines", "file_info"))
            xml->AddEle("defines", "file_info");
        if(!xml->HasEle("defines", "file_info"))
            return;

        // FLASH内容ID
        if(xml->HasAtt("file_info", "content_ID"))
            xml->DelAtt("file_info", "content_ID");

        if(icontent_id_input)
        {
                if(!xml->HasAtt("file_info", "content_ID"))
                    xml->AddAtt("file_info", "content_ID");
                if(!xml->HasAtt("file_info", "content_ID"))
                    return;

                xml->ModifyAtt("file_info", "content_ID", icontent_id_input->text());
        }

        // FLASH内容等级
        if(xml->HasAtt("file_info", "content_level"))
            xml->DelAtt("file_info", "content_level");

//        if(icontent_level_box)
        if(icontent_level_input)
        {
                if(!xml->HasAtt("file_info", "content_level"))
                    xml->AddAtt("file_info", "content_level");
                if(!xml->HasAtt("file_info", "content_level"))
                    return;

//                xml->ModifyAtt("file_info", "content_level", icontent_level_box->text());
                xml->ModifyAtt("file_info", "content_level", icontent_level_input->text());
        }

        if(xml->HasAtt("file_info", "zcontent_type"))
            xml->DelAtt("file_info", "zcontent_type");
        if(icontent_type_list)
        {
            if(!xml->HasAtt("file_info", "zcontent_type"))
                xml->AddAtt("file_info", "zcontent_type");
            if(!xml->HasAtt("file_info", "zcontent_type"))
                return;
            QString value;
            switch(icontent_type_list->currentIndex())
            {
            case 0:
                value = "game";
                break;
            case 1:
                value = "media";
                break;
            default:
                value = "game";
            }
            xml->ModifyAtt("file_info", "zcontent_type", value);
        }

        // 操作类型
        if(xml->HasAtt("file_info", "mobile_operate_type"))
            xml->DelAtt("file_info", "mobile_operate_type");

        if(ioperator_type_list)
        {
            if(!xml->HasAtt("file_info", "mobile_operate_type"))
                xml->AddAtt("file_info", "mobile_operate_type");
            if(!xml->HasAtt("file_info", "mobile_operate_type"))
                return;

            QString value;
            switch(ioperator_type_list->currentIndex())
            {
            case 0:
                value = "Nontouch_Numpad";
                break;
            case 1:
                value = "Nontouch_Fullpad";
                break;
            case 2:
                value = "Touch_Nopad";
                break;
            case 3:
                value = "Touch_Numpad";
                break;
            case 4:
                value = "Touch_Fullpad";
                break;
            default:
                value = "Nontouch_Numpad";
                break;
            }

            xml->ModifyAtt("file_info", "mobile_operate_type", value);
        }
    }
}

void TabFirst_GroupOne::SetupGroup()
{
    this->setTitle(QWidget::tr("BaseInfoGroup"));

    SetupContentid();
    SetupContentlevel();
    SetupOperatortype();
    SetupContentType();
}

void TabFirst_GroupOne::SetupContentid()
{
    icontent_id = new QLabel(this);
    icontent_id->setText(QLabel::tr("content_ID"));

    icontent_id_input = new XPopupContentId_LineEdit(this);
    icontent_id_input->setAlignment(Qt::AlignRight);
    icontent_id_input->clear();
    QObject::connect(this, SIGNAL(ChangeMD5(int,int)),
                     icontent_id_input, SLOT(MidMd5(int,int)));
    connect(this, SIGNAL(signalUpdatePathOtoI(QString)),
            icontent_id_input, SLOT(slotUpdatePath(QString)));
    
    connect(icontent_id_input, SIGNAL(signalUpdatePath(QString)),
            this, SLOT(slotUpdatePathItoO(QString)));

    iChangBox = new QCheckBox(this);
    iChangBox->setText(tr("last 16"));
    QObject::connect(icontent_id_input, SIGNAL(ContentChanged(bool)),
                    iChangBox, SLOT(setChecked(bool)) );
    QObject::connect(iChangBox, SIGNAL(clicked(bool)),
                     this, SLOT(ClickCheckBox(bool)));

    // 排版
    iLayout->addWidget(icontent_id, 0, 0);
//    iLayout->addWidget(icontent_id_box, 0, 1);
    iLayout->addWidget(icontent_id_input, 0, 1);
    iLayout->addWidget(iChangBox, 0, 2);
}

void TabFirst_GroupOne::SetupContentlevel()
{
    icontent_level = new QLabel(this);
    icontent_level->setText(QLabel::tr("content_level"));

//    icontent_level_box = new QSpinBox(this);
//    icontent_level_box->setAlignment(Qt::AlignRight);
//    icontent_level_box->setRange(0, 255);
//    icontent_level_box->clear();
    icontent_level_input = new QLineEdit(this);
    icontent_level_input->setAlignment(Qt::AlignRight);
    icontent_level_input->clear();

    // 排版
    iLayout->addWidget(icontent_level, 0, 3);
//    iLayout->addWidget(icontent_level_box, 0, 4);
    iLayout->addWidget(icontent_level_input, 0, 4);
}

void TabFirst_GroupOne::SetupOperatortype()
{
    ioperator_type = new QLabel(this);
    ioperator_type->setText(QLabel::tr("mobile_operate_type"));

    ioperator_type_list = new QComboBox(this);
    ioperator_type_list->addItem(QComboBox::tr("Nontouch_Numpad"));
    ioperator_type_list->addItem(QComboBox::tr("Nontouch_Fullpad"));
    ioperator_type_list->addItem(QComboBox::tr("Touch_Nopad"));
    ioperator_type_list->addItem(QComboBox::tr("Touch_Numpad"));
    ioperator_type_list->addItem(QComboBox::tr("Touch_Fullpad"));

    // 排版
    iLayout->addWidget(ioperator_type, 1, 0);
    iLayout->addWidget(ioperator_type_list, 1, 1);
}

void TabFirst_GroupOne::SetupContentType()
{
    icontent_type = new QLabel(this);
    icontent_type->setText(QLabel::tr("content_type"));

    icontent_type_list = new QComboBox(this);
    icontent_type_list->addItem(QComboBox::tr("game"));
    icontent_type_list->addItem(QComboBox::tr("media"));

    iLayout->addWidget(icontent_type, 1, 3);
    iLayout->addWidget(icontent_type_list, 1, 4);
}

void TabFirst_GroupOne::ClickCheckBox(bool b)
{
    if(!b)
    {
        emit ChangeMD5(0, 16);
    }
    else
    {
        emit ChangeMD5(16, 16);
    }
}
