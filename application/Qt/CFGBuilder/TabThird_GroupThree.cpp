/*
 * TabThird中的NetworkInfo Group
 */

#include "TabThird_GroupThree.h"
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QTextCodec>
#include <QGridLayout>
#include <QTextEdit>
#include "EasyXml.h"
#include "CustomDefine.h"

TabThird_GroupThree::TabThird_GroupThree(QWidget *parent)
    :QGroupBox(parent)
{
    // 排版
    iLayout = new QGridLayout;

    // 构造
    SetupGroup();

    this->setLayout(iLayout);
}

void TabThird_GroupThree::SetupGroup()
{
    this->setTitle(QWidget::tr("network_define"));

    SetupScoretype();
    SetupScorevarname();
    SetupAutoTag();
    SetupSubmiturl();
}

void TabThird_GroupThree::SetupScoretype()
{
    iscoretype = new QLabel(this);
    iscoretype->setText(QLabel::tr("score_type"));

    iscoretype_list = new QComboBox(this);
    iscoretype_list->addItem(QComboBox::tr("NULL"));
    iscoretype_list->addItem(QComboBox::tr("var"));
    iscoretype_list->addItem(QComboBox::tr("obj"));

    iscoretype_list->setCurrentIndex(1);

    iLayout->addWidget(iscoretype, 0, 0);
    iLayout->addWidget(iscoretype_list, 0, 1);
}

void TabThird_GroupThree::SetupScorevarname()
{
    iscorevarname = new QLabel(this);
    iscorevarname->setText(QLabel::tr("score_var_name"));

    iscorevarname_input = new QLineEdit(this);

    iLayout->addWidget(iscorevarname, 1, 0);
    iLayout->addWidget(iscorevarname_input, 1, 1);
}

void TabThird_GroupThree::SetupAutoTag()
{
    iAutoTag = new QLabel(tr("AutoTag"), this);
    iAutoTag_input = new QLineEdit(this);
    
    QObject::connect(iscoretype_list, SIGNAL(currentIndexChanged(int)),
            this, SLOT(lock(int)));
    iscoretype_list->setCurrentIndex(1);
    
    iLayout->addWidget(iAutoTag, 2, 0);
    iLayout->addWidget(iAutoTag_input, 2, 1);
}

void TabThird_GroupThree::lock(int index)
{
    if(index==0)
    {
        iscorevarname_input->setEnabled(false);
        isubmiturl_input->setEnabled(false);
        iAutoTag_input->setEnabled(false);
    }
    else
    {
        iscorevarname_input->setEnabled(true);
        isubmiturl_input->setEnabled(true);
        iAutoTag_input->setEnabled(true);
    }
}

void TabThird_GroupThree::SetupSubmiturl()
{
    isubmiturl = new QLabel(this);
    isubmiturl->setText(QLabel::tr("submit_url"));

    isubmiturl_input = new QTextEdit(this);
    isubmiturl_input->setText("http://m.jqbar.com/submit.aspx");

    iLayout->addWidget(isubmiturl, 3, 0);
    iLayout->addWidget(isubmiturl_input, 3, 1);
}

void TabThird_GroupThree::Default()
{
    if(iscoretype_list)
        iscoretype_list->setCurrentIndex(1);
    if(iscorevarname_input)
        iscorevarname_input->clear();
    if(iAutoTag_input)
        iAutoTag_input->clear();
    if(isubmiturl_input)
        isubmiturl_input->setText("http://m.jqbar.com/submit.aspx");
}

void TabThird_GroupThree::ReadData(XEasyXml *xml)
{
    if(xml)
    {
        // 积分的类型
        QString value;
        value = xml->FindAtt("network_define", "score_type");
        if(iscoretype_list)
        {
            if(value=="obj")
                iscoretype_list->setCurrentIndex(2);
            else if(value=="var")
                iscoretype_list->setCurrentIndex(1);
            else
                iscoretype_list->setCurrentIndex(0);
        }

        // 上传积分变量名
        value = xml->FindAtt("network_define", "score_var_name");
        if(iscorevarname_input)
            iscorevarname_input->setText(value);

        // 上传积分的URL
        value = xml->FindAtt("network_define", "submit_url");
        if(isubmiturl_input)
            isubmiturl_input->setText(value);
        
        // autotag
        value = xml->FindAtt("network_define", "autotag");
        if(iAutoTag_input)
            iAutoTag_input->setText(value);

    }
}

void TabThird_GroupThree::SaveData(XEasyXml *xml)
{
    if(xml)
    {
        if(!xml->HasEle("defines", "network_define"))
            xml->AddEle("defines", "network_define");
        if(!xml->HasEle("defines", "network_define"))
            return;

        // 积分类型
        if(xml->HasAtt("network_define", "score_type"))
            xml->DelAtt("network_define", "score_type");

        if(iscoretype_list)
        {
            if(!xml->HasAtt("network_define", "score_type"))
                xml->AddAtt("network_define", "score_type");
            if(!xml->HasAtt("network_define", "score_type"))
                return;

            if(iscoretype_list->currentIndex()==1)
                xml->ModifyAtt("network_define", "score_type", "var");
            else if(iscoretype_list->currentIndex()==2)
                xml->ModifyAtt("network_define", "score_type", "obj");
            else
                xml->ModifyAtt("network_define", "score_type", "");
        }

        // 上传积分变量名
        if(xml->HasAtt("network_define", "score_var_name"))
            xml->DelAtt("network_define", "score_var_name");

        if(iscorevarname_input && iscoretype_list->currentIndex()!=0)
        {
            if(iscorevarname_input->text().length()!=0)
            {
                if(!xml->HasAtt("network_define", "score_var_name"))
                    xml->AddAtt("network_define", "score_var_name");
                if(!xml->HasAtt("network_define", "score_var_name"))
                    return;

                xml->ModifyAtt("network_define", "score_var_name", iscorevarname_input->text());
            }
        }

        // 上传积分的URL
        if(xml->HasAtt("network_define", "submit_url"))
            xml->DelAtt("network_define", "submit_url");

        if(isubmiturl_input && iscoretype_list->currentIndex()!=0)
        {
            if(isubmiturl_input->toPlainText().length()!=0)
            {
                if(!xml->HasAtt("network_define", "submit_url"))
                    xml->AddAtt("network_define", "submit_url");
                if(!xml->HasAtt("network_define", "submit_url"))
                    return;

                int index = 0;
                QString value = isubmiturl_input->toPlainText();
                while(index!=-1)
                {
                    index = value.indexOf(GlobalNewLineOther, index);

                    if(index!=-1)
                    {
                        value.replace(index, 1, "");
                    }
                }

                if(value.length()!=0)
                    xml->ModifyAtt("network_define", "submit_url", value);
            }
        }
        
        // autotag
        if(xml->HasAtt("network_define", "autotag"))
            xml->DelAtt("network_define", "autotag");
        
        if(iAutoTag_input && iscoretype_list->currentIndex()!=0)
        {
            if(!xml->HasAtt("network_define", "autotag"))
                xml->AddAtt("network_define", "autotag");
            if(!xml->HasAtt("network_define", "autotag"))
                return;
            
            xml->ModifyAtt("network_define", "autotag", iAutoTag_input->text());
        }
    }
}

