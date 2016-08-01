/*
 * TabThird中的SoundInfo Group
 */

#include "TabThird_GroupTwo.h"
#include <QLabel>
#include <QLineEdit>
#include <QTextCodec>
#include <QComboBox>
#include <QGridLayout>
#include "EasyXml.h"
#include "CustomDefine.h"
#include <QSpinBox>

TabThird_GroupTwo::TabThird_GroupTwo(QWidget *parent)
    :QGroupBox(parent)
{
    // 排版
    iLayout = new QGridLayout;
    iLayout->setColumnStretch(4, 2);

    // 构造
    SetupGroup();

    this->setLayout(iLayout);
}

void TabThird_GroupTwo::SetupGroup()
{
    this->setTitle(QWidget::tr("sound_define"));

    SetupMute();
    SetupVolume();
    SetupSounddevice();
}

void TabThird_GroupTwo::SetupMute()
{
    imute = new QLabel(this);
    imute->setText(QLabel::tr("mute"));

    imute_list = new QComboBox(this);
    imute_list->addItem(QComboBox::tr("true"));
    imute_list->addItem(QComboBox::tr("false"));
    imute_list->setCurrentIndex(1);

    iLayout->addWidget(imute, 0, 0);
    iLayout->addWidget(imute_list, 0, 1);
}

void TabThird_GroupTwo::SetupVolume()
{
    ivolume = new QLabel(this);
    ivolume->setText(QLabel::tr("volume"));

//    ivolume_box = new QSpinBox(this);
//    ivolume_box->setAlignment(Qt::AlignRight);
//    ivolume_box->setRange(1, 5);
//    ivolume_box->setValue(4);
    ivolume_input = new QLineEdit(this);
    ivolume_input->setAlignment(Qt::AlignRight);
    ivolume_input->setText("4");

    iLayout->addWidget(ivolume, 0, 2);
//    iLayout->addWidget(ivolume_box, 0, 3);
    iLayout->addWidget(ivolume_input, 0, 3);
}

void TabThird_GroupTwo::SetupSounddevice()
{
    isounddevice = new QLabel(this);
    isounddevice->setText(QLabel::tr("sound_device"));

    isounddevice_list = new QComboBox(this);
    isounddevice_list->addItem(QComboBox::tr("Speaker"));
    isounddevice_list->addItem(QComboBox::tr("Phone"));
    isounddevice_list->addItem(QComboBox::tr("Earset"));
    isounddevice_list->setCurrentIndex(0);

    iLayout->addWidget(isounddevice, 1, 0);
    iLayout->addWidget(isounddevice_list, 1, 1);
}

void TabThird_GroupTwo::Default()
{
    if(imute_list)
        imute_list->setCurrentIndex(1);
//    if(ivolume_box)
//        ivolume_box->setValue(4);
    if(ivolume_input)
        ivolume_input->setText("4");
    if(isounddevice_list)
        isounddevice_list->setCurrentIndex(0);
}

void TabThird_GroupTwo::ReadData(XEasyXml *xml)
{
    if(xml)
    {
        // 是否静音
        QString value;
        value = xml->FindAtt("sound_define", "mute");
        if(imute_list)
        {
            if(value=="true")
                imute_list->setCurrentIndex(0);
            else
                imute_list->setCurrentIndex(1);
        }

        // 音量大小
        value = xml->FindAtt("sound_define", "volume");
//        if(ivolume_box)
//        {
//            if(value.length()!=0)
//                ivolume_box->setValue(value.toInt());
//            else
//                ivolume_box->clear();
//        }
        if(ivolume_input)
        {
            if(value.length()!=0)
                ivolume_input->setText(value);
            else
                ivolume_input->clear();
        }

        // 声音输出设备
        value = xml->FindAtt("sound_define", "sound_device");
        if(isounddevice_list)
        {
            if(value=="Phone")
                isounddevice_list->setCurrentIndex(1);
            else if(value=="Earset")
                isounddevice_list->setCurrentIndex(2);
            else
                isounddevice_list->setCurrentIndex(0);
        }
    }
}

void TabThird_GroupTwo::SaveData(XEasyXml *xml)
{
    if(xml)
    {
        if(!xml->HasEle("defines", "sound_define"))
            xml->AddEle("defines", "sound_define");
        if(!xml->HasEle("defines", "sound_define"))
            return;

        // 是否静音
        if(xml->HasAtt("sound_define", "mute"))
            xml->DelAtt("sound_define", "mute");

        if(imute_list)
        {
            if(!xml->HasAtt("sound_define", "mute"))
                xml->AddAtt("sound_define", "mute");
            if(!xml->HasAtt("sound_define", "mute"))
                return;

            QString value;
            switch(imute_list->currentIndex())
            {
            case 0:
                value = "true";
                break;
            case 1:
                value = "false";
                break;
            default:
                value = "false";
                break;
            }

            xml->ModifyAtt("sound_define", "mute", value);
        }

        // 音量大小
        if(xml->HasAtt("sound_define", "volume"))
            xml->DelAtt("sound_define", "volume");

//        if(ivolume_box)
        if(ivolume_input)
        {
            if(!xml->HasAtt("sound_define", "volume"))
                xml->AddAtt("sound_define", "volume");
            if(!xml->HasAtt("sound_define", "volume"))
                return;

//            xml->ModifyAtt("sound_define", "volume", ivolume_box->text());
            xml->ModifyAtt("sound_define", "volume", ivolume_input->text());
        }

        // 声音输出设备
        if(xml->HasAtt("sound_define", "sound_device"))
            xml->DelAtt("sound_define", "sound_device");

        if(isounddevice_list)
        {
            if(!xml->HasAtt("sound_define", "sound_device"))
                xml->AddAtt("sound_define", "sound_device");
            if(!xml->HasAtt("sound_define", "sound_device"))
                return;

            QString value;
            switch(isounddevice_list->currentIndex())
            {
            case 0:
                value = "Speaker";
                break;
            case 1:
                value = "Phone";
                break;
            case 2:
                value = "Earset";
                break;
            default:
                value = "Speaker";
                break;
            }

            xml->ModifyAtt("sound_define", "sound_device", value);
        }
    }
}
