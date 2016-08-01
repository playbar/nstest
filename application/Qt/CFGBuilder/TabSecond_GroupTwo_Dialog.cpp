
#include "TabSecond_GroupTwo_Dialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include "TabSecond_GroupTwo_Item.h"
#include "CustomDefine.h"
#include <QBoxLayout>
#include <QHBoxLayout>

TabSecond_GroupTwo_Dialog::TabSecond_GroupTwo_Dialog(QWidget *parent)
    :QDialog(parent)
{
    iParent = NULL;
    iItem = NULL;

    // ÅÅ°æ
//    iLayout = new QGridLayout;
//    iLayout->setColumnStretch(4, 2);
    iLayout = new QBoxLayout(QBoxLayout::TopToBottom);

    // ¹¹Ôì
    SetupDialog();

    this->setLayout(iLayout);
}

void TabSecond_GroupTwo_Dialog::SetupDialog()
{
    this->setWindowTitle(QDialog::tr("mouse_normal_map"));

    SetupVirtualKey();
    SetupMouseAct();
    SetupButtonBox();
}

void TabSecond_GroupTwo_Dialog::SetupVirtualKey()
{
    iVirtualKey = new QLabel(this);
    iVirtualKey->setText(QLabel::tr("virtual_key"));

//    iVirtualKey_input = new QLineEdit(this);
    iVirtualKey_list = new QComboBox(this);
    iVirtualKey_list->addItem(QComboBox::tr("VK1"));
    iVirtualKey_list->addItem(QComboBox::tr("VK2"));
    iVirtualKey_list->addItem(QComboBox::tr("VK3"));
    iVirtualKey_list->addItem(QComboBox::tr("VK4"));
    iVirtualKey_list->addItem(QComboBox::tr("VK5"));
    iVirtualKey_list->addItem(QComboBox::tr("VK6"));
    iVirtualKey_list->addItem(QComboBox::tr("VK7"));
    iVirtualKey_list->addItem(QComboBox::tr("VK8"));
    iVirtualKey_list->addItem(QComboBox::tr("VK9"));
    iVirtualKey_list->addItem(QComboBox::tr("VK10"));

    isubLayout = new QHBoxLayout;

//    iLayout->addWidget(iVirtualKey, 0, 0);
//    iLayout->addWidget(iVirtualKey_input, 0, 1);
//    iLayout->addWidget(iVirtualKey_list, 0, 1);
    isubLayout->addWidget(iVirtualKey);
    isubLayout->addWidget(iVirtualKey_list);
}

void TabSecond_GroupTwo_Dialog::SetupMouseAct()
{
    iMouseAct = new QLabel(this);
    iMouseAct->setText(QLabel::tr("mouse_act"));

    iMouseAct_list = new QComboBox(this);
    iMouseAct_list->addItem(QComboBox::tr("up"));
    iMouseAct_list->addItem(QComboBox::tr("down"));
    iMouseAct_list->addItem(QComboBox::tr("left"));
    iMouseAct_list->addItem(QComboBox::tr("right"));
    iMouseAct_list->addItem(QComboBox::tr("left-key"));
    iMouseAct_list->addItem(QComboBox::tr("right-key"));

//    iLayout->addWidget(iMouseAct, 0, 2);
//    iLayout->addWidget(iMouseAct_list, 0, 3);
    isubLayout->addWidget(iMouseAct);
    isubLayout->addWidget(iMouseAct_list);

    iLayout->addLayout(isubLayout);
}

void TabSecond_GroupTwo_Dialog::SetupButtonBox()
{
    iButtonBox = new QDialogButtonBox(this);

    iOk = new QPushButton(this);
    iOk->setText(QPushButton::tr("OK"));
    QObject::connect(iOk, SIGNAL(clicked()),
                     this, SLOT(ClickOK()));
    iButtonBox->addButton(iOk, QDialogButtonBox::ActionRole);

    iCancel = new QPushButton(this);
    iCancel->setText(QPushButton::tr("CANCEL"));
    QObject::connect(iCancel, SIGNAL(clicked()),
                     this, SLOT(close()));
    iButtonBox->addButton(iCancel, QDialogButtonBox::ActionRole);

    iLayout->addWidget(iButtonBox);
}

void TabSecond_GroupTwo_Dialog::ModifyItem_Item(QListWidget* parent, QListWidgetItem* item)
{
    iParent = parent;
    iItem = item;

    if(iItem!=NULL)
    {
//        iVirtualKey_input->setText(((TabSecond_GroupTwo_Item*)iItem)->VirtualKey());
        if(((TabSecond_GroupTwo_Item*)iItem)->VirtualKey()=="VK1")
            iVirtualKey_list->setCurrentIndex(0);
        else if(((TabSecond_GroupTwo_Item*)iItem)->VirtualKey()=="VK2")
            iVirtualKey_list->setCurrentIndex(1);
        else if(((TabSecond_GroupTwo_Item*)iItem)->VirtualKey()=="VK3")
            iVirtualKey_list->setCurrentIndex(2);
        else if(((TabSecond_GroupTwo_Item*)iItem)->VirtualKey()=="VK4")
            iVirtualKey_list->setCurrentIndex(3);
        else if(((TabSecond_GroupTwo_Item*)iItem)->VirtualKey()=="VK5")
            iVirtualKey_list->setCurrentIndex(4);
        else if(((TabSecond_GroupTwo_Item*)iItem)->VirtualKey()=="VK6")
            iVirtualKey_list->setCurrentIndex(5);
        else if(((TabSecond_GroupTwo_Item*)iItem)->VirtualKey()=="VK7")
            iVirtualKey_list->setCurrentIndex(6);
        else if(((TabSecond_GroupTwo_Item*)iItem)->VirtualKey()=="VK8")
            iVirtualKey_list->setCurrentIndex(7);
        else if(((TabSecond_GroupTwo_Item*)iItem)->VirtualKey()=="VK9")
            iVirtualKey_list->setCurrentIndex(8);
        else if(((TabSecond_GroupTwo_Item*)iItem)->VirtualKey()=="VK10")
            iVirtualKey_list->setCurrentIndex(9);

        if(((TabSecond_GroupTwo_Item*)iItem)->MouseAct()=="up")
            iMouseAct_list->setCurrentIndex(0);
        else if(((TabSecond_GroupTwo_Item*)iItem)->MouseAct()=="down")
            iMouseAct_list->setCurrentIndex(1);
        else if(((TabSecond_GroupTwo_Item*)iItem)->MouseAct()=="left")
            iMouseAct_list->setCurrentIndex(2);
        else if(((TabSecond_GroupTwo_Item*)iItem)->MouseAct()=="right")
            iMouseAct_list->setCurrentIndex(3);
        else if(((TabSecond_GroupTwo_Item*)iItem)->MouseAct()=="left-key")
            iMouseAct_list->setCurrentIndex(4);
        else if(((TabSecond_GroupTwo_Item*)iItem)->MouseAct()=="right-key")
            iMouseAct_list->setCurrentIndex(5);
    }
    else
    {
//        iVirtualKey_input->clear();
        iVirtualKey_list->setCurrentIndex(0);
        iMouseAct_list->setCurrentIndex(0);
    }

    this->exec();
}

void TabSecond_GroupTwo_Dialog::ClickOK()
{
    if(iParent!=NULL)
    {
        if(iItem!=NULL)
        {
//            if(iVirtualKey_input->text().length()==0)
//                return;

//            ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey(iVirtualKey_input->text());
//            if(iVirtualKey_list->currentIndex()==0)
//                ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey("10001");
//            else if(iVirtualKey_list->currentIndex()==1)
//                ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey("10002");
//            else if(iVirtualKey_list->currentIndex()==2)
//                ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey("10003");
//            else if(iVirtualKey_list->currentIndex()==3)
//                ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey("10004");
//            else if(iVirtualKey_list->currentIndex()==4)
//                ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey("10005");
//            else if(iVirtualKey_list->currentIndex()==5)
//                ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey("10006");
//            else if(iVirtualKey_list->currentIndex()==6)
//                ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey("10007");
//            else if(iVirtualKey_list->currentIndex()==7)
//                ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey("10008");
//            else if(iVirtualKey_list->currentIndex()==8)
//                ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey("10009");
//            else if(iVirtualKey_list->currentIndex()==9)
//                ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey("10010");

            ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey(iVirtualKey_list->currentText());
            if(iMouseAct_list->currentIndex()==0)
                ((TabSecond_GroupTwo_Item*)iItem)->SetMouseAct("up");
            else if(iMouseAct_list->currentIndex()==1)
                ((TabSecond_GroupTwo_Item*)iItem)->SetMouseAct("down");
            else if(iMouseAct_list->currentIndex()==2)
                ((TabSecond_GroupTwo_Item*)iItem)->SetMouseAct("left");
            else if(iMouseAct_list->currentIndex()==3)
                ((TabSecond_GroupTwo_Item*)iItem)->SetMouseAct("right");
            else if(iMouseAct_list->currentIndex()==4)
                ((TabSecond_GroupTwo_Item*)iItem)->SetMouseAct("left-key");
            else if(iMouseAct_list->currentIndex()==5)
                ((TabSecond_GroupTwo_Item*)iItem)->SetMouseAct("right-key");

            // item txt style (key -> maptokey)
            QString itemtitle;
            itemtitle += iVirtualKey_list->currentText();
            itemtitle += QString("\t->\t");
            itemtitle += ((TabSecond_GroupTwo_Item*)iItem)->MouseAct();
            iItem->setText(itemtitle);
        }
        else
        {
//            if(iVirtualKey_input->text().length()==0)
//                return;

            iItem = new TabSecond_GroupTwo_Item;

//            ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey(iVirtualKey_input->text());

//            if(iVirtualKey_list->currentIndex()==0)
//                ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey("10001");
//            else if(iVirtualKey_list->currentIndex()==1)
//                ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey("10002");
//            else if(iVirtualKey_list->currentIndex()==2)
//                ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey("10003");
//            else if(iVirtualKey_list->currentIndex()==3)
//                ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey("10004");
//            else if(iVirtualKey_list->currentIndex()==4)
//                ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey("10005");
//            else if(iVirtualKey_list->currentIndex()==5)
//                ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey("10006");
//            else if(iVirtualKey_list->currentIndex()==6)
//                ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey("10007");
//            else if(iVirtualKey_list->currentIndex()==7)
//                ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey("10008");
//            else if(iVirtualKey_list->currentIndex()==8)
//                ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey("10009");
//            else if(iVirtualKey_list->currentIndex()==9)
//                ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey("10010");

            ((TabSecond_GroupTwo_Item*)iItem)->SetVirtualKey(iVirtualKey_list->currentText());
            if(iMouseAct_list->currentIndex()==0)
                ((TabSecond_GroupTwo_Item*)iItem)->SetMouseAct("up");
            else if(iMouseAct_list->currentIndex()==1)
                ((TabSecond_GroupTwo_Item*)iItem)->SetMouseAct("down");
            else if(iMouseAct_list->currentIndex()==2)
                ((TabSecond_GroupTwo_Item*)iItem)->SetMouseAct("left");
            else if(iMouseAct_list->currentIndex()==3)
                ((TabSecond_GroupTwo_Item*)iItem)->SetMouseAct("right");
            else if(iMouseAct_list->currentIndex()==4)
                ((TabSecond_GroupTwo_Item*)iItem)->SetMouseAct("left-key");
            else if(iMouseAct_list->currentIndex()==5)
                ((TabSecond_GroupTwo_Item*)iItem)->SetMouseAct("right-key");

            // item txt style (key -> maptokey)
            QString itemtitle;
            itemtitle += iVirtualKey_list->currentText();
            itemtitle += QString("\t->\t");
            itemtitle += ((TabSecond_GroupTwo_Item*)iItem)->MouseAct();
            iItem->setText(itemtitle);

            iParent->addItem(iItem);
        }
    }

    this->close();
}
