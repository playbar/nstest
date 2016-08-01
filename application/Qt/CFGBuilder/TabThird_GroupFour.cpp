/*
 * TabThird中的TxtInfo Group
 */

#include "TabThird_GroupFour.h"
#include <QListWidget>
#include <QTextCodec>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include "TabThird_GroupFour_Dialog.h"
#include <QListWidgetItem>
#include <QTextEdit>
#include "EasyXml.h"
#include "CustomDefine.h"
#include <QPlainTextEdit>

TabThird_GroupFour::TabThird_GroupFour(QWidget *parent)
    :QGroupBox(parent)
{
    // 排版
    iLayout = new QVBoxLayout;

    // 构造
    SetupGroup();

    this->setLayout(iLayout);
}

void TabThird_GroupFour::SetupGroup()
{
    this->setTitle(QWidget::tr("txt_define"));

    itxt_input = new QTextEdit(this);
    itxt_input->setAcceptRichText(false);
    itxt_input->setWordWrapMode(QTextOption::NoWrap);
    iLayout->addWidget(itxt_input);
}

void TabThird_GroupFour::Default()
{
    if(itxt_input)
        itxt_input->clear();
}

void TabThird_GroupFour::ReadData(XEasyXml *xml)
{
    if(xml)
    {
        QString value;
        value = xml->Content("txt_define");
        if(itxt_input)
        {
            itxt_input->setPlainText(value);
        }
    }
}

void TabThird_GroupFour::SaveData(XEasyXml *xml)
{
    if(xml)
    {
        if(!xml->HasEle("defines", "txt_define"))
            xml->AddEle("defines", "txt_define");
        if(!xml->HasEle("defines", "txt_define"))
            return;

        // 帮助
        if(itxt_input)
        {
            QString value = itxt_input->toPlainText();
            xml->SetContent("txt_define", itxt_input->toPlainText());
        }
    }
}
