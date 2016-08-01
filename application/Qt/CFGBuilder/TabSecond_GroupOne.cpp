/*
 * TabSecond中的OperatorBaseInfo Group
 */

#include "TabSecond_GroupOne.h"
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include "EasyXml.h"
#include "CustomDefine.h"
#include <QSpinBox>

TabSecond_GroupOne::TabSecond_GroupOne(QWidget *parent)
    :QGroupBox(parent)
{
    // 排版
    iLayout = new QGridLayout;

    // 构造
    SetupGroup();

    this->setLayout(iLayout);
}

void TabSecond_GroupOne::SetupGroup()
{
    this->setTitle(QWidget::tr("operate_define"));

    SetupMousemovepace();
    SetupLocktime();
    SetupTouchSize();
    SetupZacc();
    SetupZbutton();
}

void TabSecond_GroupOne::SetupMousemovepace()
{
    iMousemovepace = new QLabel(this);
    iMousemovepace->setText(QLabel::tr("default_mouse_move_pace"));

    iMousemovepace_input = new QLineEdit(this);
    iMousemovepace_input->setAlignment(Qt::AlignRight);
    iMousemovepace_input->setText("3");

    iLayout->addWidget(iMousemovepace, 0, 0);
    iLayout->addWidget(iMousemovepace_input, 0, 1);
}

void TabSecond_GroupOne::SetupLocktime()
{
    iLocktime = new QLabel(this);
    iLocktime->setText(QLabel::tr("default_long_press_lock_time"));

    iLocktime_input = new QLineEdit(this);
    iLocktime_input->setAlignment(Qt::AlignRight);
    iLocktime_input->setText("1000");

    iLayout->addWidget(iLocktime, 0, 2);
    iLayout->addWidget(iLocktime_input, 0, 3);
}

void TabSecond_GroupOne::SetupTouchSize()
{
    iTouchSize = new QLabel(QLabel::tr("TouchSize"), this);
    
    iTouchSize_input = new QLineEdit(this);
    iTouchSize_input->setAlignment(Qt::AlignRight);
    iTouchSize_input->setText("1");
    
    iLayout->addWidget(iTouchSize, 1, 0);
    iLayout->addWidget(iTouchSize_input, 1, 1);
}

void TabSecond_GroupOne::SetupZacc()
{
    iZacc = new QLabel(QLabel::tr("Zacc"), this);
    iZacc_list = new QComboBox;
    iZacc_list->addItem(QComboBox::tr("false"));
    iZacc_list->addItem(QComboBox::tr("true"));
    iZacc_list->setCurrentIndex(0);

    iLayout->addWidget(iZacc, 2, 0);
    iLayout->addWidget(iZacc_list, 2, 1);
}

void TabSecond_GroupOne::SetupZbutton()
{
    iZbutton = new QLabel(QLabel::tr("Zbutton"), this);
    iZbutton_list = new QComboBox;
    iZbutton_list->addItem(QComboBox::tr("false"));
    iZbutton_list->addItem(QComboBox::tr("true"));
    iZbutton_list->setCurrentIndex(0);

    iLayout->addWidget(iZbutton, 2, 2);
    iLayout->addWidget(iZbutton_list, 2, 3);

}

void TabSecond_GroupOne::Default()
{
    if(iMousemovepace_input)
        iMousemovepace_input->setText("3");
    if(iLocktime_input)
        iLocktime_input->setText("1000");
    if(iTouchSize_input)
        iTouchSize_input->setText("1");
    if(iZacc_list)
        iZacc_list->setCurrentIndex(0);
    if(iZbutton_list)
        iZbutton_list->setCurrentIndex(0);
}

void TabSecond_GroupOne::ReadData(XEasyXml *xml)
{
    if(xml)
    {
        // 鼠标移动步长
        QString value;
        value = xml->FindAtt("operate_define", "default_mouse_move_pace");

        if(iMousemovepace_input)
        {
            if(value.length()!=0)
                iMousemovepace_input->setText(value);
            else
                iMousemovepace_input->clear();
        }

        // 长按锁定时间
        value = xml->FindAtt("operate_define", "default_long_press_lock_time");

        if(iLocktime_input)
        {
            if(value.length()!=0)
                iLocktime_input->setText(value);
            else
                iLocktime_input->clear();
        }
        
        // 触屏半径
        value = xml->FindAtt("operate_define", "touchsize");

        if(iTouchSize_input)
        {
            if(value.length()!=0)
                iTouchSize_input->setText(value);
            else
                iTouchSize_input->clear();
        }

        // Zacc
        value = xml->FindAtt("operate_define", "zacc");

        if(iZacc_list)
        {
            if(value=="true")
                iZacc_list->setCurrentIndex(1);
            else
                iZacc_list->setCurrentIndex(0);
        }

        // Zbutton
        value = xml->FindAtt("operate_define", "zbutton");

        if(iZbutton_list)
        {
            if(value=="true")
                iZbutton_list->setCurrentIndex(1);
            else
                iZbutton_list->setCurrentIndex(0);
        }
    }
}

void TabSecond_GroupOne::SaveData(XEasyXml *xml)
{
    if(xml)
    {
        if(!xml->HasEle("defines", "operate_define"))
            xml->AddEle("defines", "operate_define");

        if(!xml->HasEle("defines", "operate_define"))
            return;


        if(xml->HasAtt("operate_define", "default_long_press_lock_time"))
            xml->DelAtt("operate_define", "default_long_press_lock_time");

        // 鼠标移动步长
        if(xml->HasAtt("operate_define", "default_mouse_move_pace"))
            xml->DelAtt("operate_define", "default_mouse_move_pace");
        
        if(xml->HasAtt("operate_define", "touchsize"))
            xml->DelAtt("operate_define", "touchsize");

        if(!xml->HasAtt("operate_define", "default_mouse_move_pace"))
            xml->AddAtt("operate_define", "default_mouse_move_pace");
        if(!xml->HasAtt("operate_define", "default_mouse_move_pace"))
            return;

        if(iMousemovepace_input)
        {
            xml->ModifyAtt("operate_define", "default_mouse_move_pace", iMousemovepace_input->text());
        }

        // 长按锁定时间
        if(!xml->HasAtt("operate_define", "default_long_press_lock_time"))
            xml->AddAtt("operate_define", "default_long_press_lock_time");

        if(!xml->HasAtt("operate_define", "default_long_press_lock_time"))
            return;

        if(iLocktime_input)
        {
             xml->ModifyAtt("operate_define", "default_long_press_lock_time", iLocktime_input->text());
        }
        
        // 触屏半径
        if(!xml->HasAtt("operate_define", "touchsize"))
            xml->AddAtt("operate_define", "touchsize");
        if(!xml->HasAtt("operate_define", "touchsize"))
            return;
        
        if(iTouchSize_input)
        {
            xml->ModifyAtt("operate_define", "touchsize", iTouchSize_input->text());
        }

        // zacc
        if(!xml->HasAtt("operate_define", "zacc"))
            xml->AddAtt("operate_define", "zacc");
        if(!xml->HasAtt("operate_define", "zacc"))
            return;

        if(iZacc_list->currentIndex()==1)
            xml->ModifyAtt("operate_define", "zacc", "true");
        else
            xml->ModifyAtt("operate_define", "zacc", "");

        // zbutton
        if(!xml->HasAtt("operate_define", "zbutton"))
            xml->AddAtt("operate_define", "zbutton");
        if(!xml->HasAtt("operate_define", "zbutton"))
            return;

        if(iZbutton_list->currentIndex()==1)
            xml->ModifyAtt("operate_define", "zbutton", "true");
        else
            xml->ModifyAtt("operate_define", "zbutton", "");
    }
}
