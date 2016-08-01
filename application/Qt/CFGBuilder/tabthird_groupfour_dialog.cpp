/*
 * TabThird中TxtInfo Group List Item的详情
 */

#include "TabThird_GroupFour_Dialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QTextCodec>
#include <QGridLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <TabThird_GroupFour_Item.h>

TabThird_GroupFour_Dialog::TabThird_GroupFour_Dialog(QWidget *parent)
    :QDialog(parent)
{
    iListparent = NULL;
    iItem = NULL;

    // 中文化
    iCodec = QTextCodec::codecForName("GB2312");

    // 排版
    iLayout = new QGridLayout;
    iLayout->setColumnStretch(2, 3);

    // 构造
    SetupDialog();

    this->setLayout(iLayout);
}

void TabThird_GroupFour_Dialog::SetupDialog()
{
    SetupTxtname();
    SetupTxtdefine();
    SetupButtongroup();
}

void TabThird_GroupFour_Dialog::SetupTxtname()
{
    itxtname = new QLabel(this);
    itxtname->setText(iCodec->toUnicode("文本名称"));

    itxtname_input = new QLineEdit(this);

    iLayout->addWidget(itxtname, 0, 0);
    iLayout->addWidget(itxtname_input, 0, 1);
}

void TabThird_GroupFour_Dialog::SetupTxtdefine()
{
    itxtdefine = new QLabel(this);
    itxtdefine->setText(iCodec->toUnicode("文本内容"));

    itxtdefine_input = new QTextEdit(this);

    iLayout->addWidget(itxtdefine, 1, 0);
    iLayout->addWidget(itxtdefine_input, 1, 1);
}

void TabThird_GroupFour_Dialog::SetupButtongroup()
{
    iButtonbox = new QDialogButtonBox(this);

    iOk = new QPushButton(this);
    iOk->setText(iCodec->toUnicode("确定"));
    iButtonbox->addButton(iOk, QDialogButtonBox::ActionRole);
    QObject::connect(iOk, SIGNAL(clicked()),
                     this, SLOT(ClickOK()));

    iCancel = new QPushButton(this);
    iCancel->setText(iCodec->toUnicode("取消"));
    iButtonbox->addButton(iCancel, QDialogButtonBox::ActionRole);
    QObject::connect(iCancel, SIGNAL(clicked()),
                     this, SLOT(close()));

    iLayout->addWidget(iButtonbox, 2, 1);
}

void TabThird_GroupFour_Dialog::ModifyItem_Item(QListWidget* parent, QListWidgetItem* item)
{
    iListparent = parent;
    iItem = item;

    if(iItem!=NULL)
    {
        itxtname_input->setText(((TabThird_GroupFour_Item*)iItem)->Txtname());
        itxtdefine_input->setText(((TabThird_GroupFour_Item*)iItem)->Txtdefine());
    }
    else
    {
        itxtname_input->clear();
        itxtdefine_input->clear();
    }

    this->exec();
}

void TabThird_GroupFour_Dialog::ClickOK()
{
    if(iListparent!=NULL)
    {
        if(itxtname_input->text().length()!=0 && itxtdefine_input->toPlainText()!=0)
        {
            if(iItem!=NULL)
            {
                ((TabThird_GroupFour_Item*)iItem)->SetTxtname(itxtname_input->text());
                ((TabThird_GroupFour_Item*)iItem)->SetTxtdefine(itxtdefine_input->toPlainText());
                ((TabThird_GroupFour_Item*)iItem)->setText(itxtname_input->text());
            }
            else
            {
                TabThird_GroupFour_Item* item = new TabThird_GroupFour_Item;
                item->SetTxtname(itxtname_input->text());
                item->SetTxtdefine(itxtdefine_input->toPlainText());
                item->setText(itxtname_input->text());
                iListparent->addItem(item);
            }
        }
    }

    this->close();
}
