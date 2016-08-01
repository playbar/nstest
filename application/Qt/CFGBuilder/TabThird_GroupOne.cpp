/*
 * TabThird中的DisplayInfo Group
 */

#include "TabThird_GroupOne.h"
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QTextCodec>
#include <QLineEdit>
#include "EasyXml.h"
#include "CustomDefine.h"

TabThird_GroupOne::TabThird_GroupOne(QWidget *parent)
    :QGroupBox(parent)
{
    // 排版
    iLayout = new QGridLayout;

    // 构造
    SetupGroup();

    this->setLayout(iLayout);
}

void TabThird_GroupOne::SetupGroup()
{
    this->setTitle(QWidget::tr("display_define"));

    SetupScreendir();
    SetupZoomsize();
    SetupQuality();
    SetupAdPos();
    SetupAdUrl();
}

void TabThird_GroupOne::SetupScreendir()
{
    iScreendir = new QLabel(this);
    iScreendir->setText(QLabel::tr("screen_direction"));

    iScreendir_list = new QComboBox;
    iScreendir_list->addItem(QComboBox::tr("Vertical"));
    iScreendir_list->addItem(QComboBox::tr("Horizon"));
    connect(iScreendir_list, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotDirChange(int)));
    iScreendir_list->setCurrentIndex(1);
    this->slotDirChange(1);

    iLayout->addWidget(iScreendir, 0, 0);
    iLayout->addWidget(iScreendir_list, 0, 1);
}

void TabThird_GroupOne::SetupZoomsize()
{
    iZoomsize = new QLabel(this);
    iZoomsize->setText(QLabel::tr("zoom_size"));

    iZoomsize_list = new QComboBox;
    iZoomsize_list->addItem(QComboBox::tr("Original"));
    iZoomsize_list->addItem(QComboBox::tr("Fit"));
    iZoomsize_list->setCurrentIndex(1);

    iLayout->addWidget(iZoomsize, 0, 2);
    iLayout->addWidget(iZoomsize_list, 0, 3);
}

void TabThird_GroupOne::SetupQuality()
{
    iQuality = new QLabel(QLabel::tr("Quality"), this);
    iQuality_list = new QComboBox(this);
    iQuality_list->addItem(QComboBox::tr("low"));
    iQuality_list->addItem(QComboBox::tr("medium"));
    iQuality_list->addItem(QComboBox::tr("high"));
    iQuality_list->addItem(QComboBox::tr("auto"));
    iQuality_list->setCurrentIndex(3);
    
    iLayout->addWidget(iQuality, 1, 0);
    iLayout->addWidget(iQuality_list, 1, 1);
}

void TabThird_GroupOne::SetupAdPos()
{
    iAdPos = new QLabel(QLabel::tr("AdPos(1-9)"), this);
    iAdPos_input = new QLineEdit(this);
    iAdPos_input->setText("1");

    iLayout->addWidget(iAdPos, 1, 2);
    iLayout->addWidget(iAdPos_input, 1, 3);
}

void TabThird_GroupOne::SetupAdUrl()
{
    iAdUrl = new QLabel(QLabel::tr("AdUrl"), this);
    iAdUrl_input = new QLineEdit(this);
    iAdUrl_input->clear();

    iLayout->addWidget(iAdUrl, 2, 0);
    iLayout->addWidget(iAdUrl_input, 2, 1, 1, 3);
}

void TabThird_GroupOne::Default()
{
    if(iScreendir_list)
        iScreendir_list->setCurrentIndex(1);
    if(iZoomsize_list)
        iZoomsize_list->setCurrentIndex(1);
    if(iQuality_list)
        iQuality_list->setCurrentIndex(3);
    if(iAdPos_input)
        iAdPos_input->setText("1");
    if(iAdUrl_input)
        iAdUrl_input->clear();
}

void TabThird_GroupOne::ReadData(XEasyXml *xml)
{
    if(xml)
    {
        // 屏幕显示方向
        QString value;
        value = xml->FindAtt("display_define", "screen_direction");
        if(iScreendir_list)
        {
            if(value=="Vertical")
                iScreendir_list->setCurrentIndex(0);
            else if(value=="Horizon")
                iScreendir_list->setCurrentIndex(1);
            else
                iScreendir_list->setCurrentIndex(0);
        }

        value = xml->FindAtt("display_define", "zoom_size");
        if(iZoomsize_list)
        {
            if(value=="Original")
                iZoomsize_list->setCurrentIndex(0);
            else
                iZoomsize_list->setCurrentIndex(1);
        }
        
        value = xml->FindAtt("display_define", "quality");
        if(iQuality_list)
        {
            if(value=="low")
                iQuality_list->setCurrentIndex(0);
            else if(value=="medium")
                iQuality_list->setCurrentIndex(1);
            else if(value=="high")
                iQuality_list->setCurrentIndex(2);
            else
                iQuality_list->setCurrentIndex(3);
        }
        value = xml->FindAtt("display_define", "zad_url");
        if(iAdUrl_input)
            iAdUrl_input->setText(value);
        value = xml->FindAtt("display_define", "zad_pos");
        if(iAdPos_input)
            iAdPos_input->setText(value);
    }
}

void TabThird_GroupOne::SaveData(XEasyXml *xml)
{
    if(xml)
    {
        if(!xml->HasEle("defines", "display_define"))
            xml->AddEle("defines", "display_define");
        if(!xml->HasEle("defines", "display_define"))
            return;

        // 屏幕显示方向
        if(xml->HasAtt("display_define", "screen_direction"))
            xml->DelAtt("display_define", "screen_direction");

        if(iScreendir_list)
        {
            if(!xml->HasAtt("display_define", "screen_direction"))
                xml->AddAtt("display_define", "screen_direction");
            if(!xml->HasAtt("display_define", "screen_direction"))
                return;

            QString value;
            switch(iScreendir_list->currentIndex())
            {
            case 0:
                value = "Vertical";
                break;
            case 1:
                value = "Horizon";
                break;
            case 2:
                value = "Auto";
                break;
            default:
                value = "Auto";
                break;
            }

            xml->ModifyAtt("display_define", "screen_direction", value);
        }

        // 显示缩放比例
        if(xml->HasAtt("display_define", "zoom_size"))
            xml->DelAtt("display_define", "zoom_size");

        if(iZoomsize_list)
        {
            if(!xml->HasAtt("display_define", "zoom_size"))
                xml->AddAtt("display_define", "zoom_size");
            if(!xml->HasAtt("display_define", "zoom_size"))
                return;

            QString value;
            switch(iZoomsize_list->currentIndex())
            {
            case 0:
                value = "Original";
                break;
            case 1:
                value = "Fit";
                break;
            default:
                value = "Fit";
                break;
            }

            xml->ModifyAtt("display_define", "zoom_size", value);
        }
        
        // Quality
        if(xml->HasAtt("display_define", "quality"))
            xml->DelAtt("display_define", "quality");
        
        if(iQuality_list)
        {
            if(!xml->HasAtt("display_define", "quality"))
                xml->AddAtt("display_define", "quality");
            if(!xml->HasAtt("display_define", "quality"))
                return;
            
            QString value;
           switch(iQuality_list->currentIndex())
           {
           case 0:
               {
                   value = "low";
               }
               break;
           case 1:
               {
                   value = "medium";
               }
               break;
           case 2:
               {
                   value = "high";
               }
               break;
           default:
               {
                   value = "auto";
               }
               break;
           }
           
           xml->ModifyAtt("display_define", "quality", value);
        }

        // adurl
        if(xml->HasAtt("display_define", "zad_url"))
            xml->DelAtt("display_define", "zad_url");

        if(iAdUrl_input)
        {
            if(!xml->HasAtt("display_define", "zad_url"))
                xml->AddAtt("display_define", "zad_url");
            if(!xml->HasAtt("display_define", "zad_url"))
                return;

            xml->ModifyAtt("display_define", "zad_url", iAdUrl_input->text());
        }

        // adpos
        if(xml->HasAtt("display_define", "zad_pos"))
            xml->DelAtt("display_define", "zad_pos");

        if(iAdPos_input)
        {
            if(!xml->HasAtt("display_define", "zad_pos"))
                xml->AddAtt("display_define", "zad_pos");
            if(!xml->HasAtt("display_define", "zad_pos"))
                return;

            xml->ModifyAtt("display_define", "zad_pos", iAdPos_input->text());
        }
    }
}
