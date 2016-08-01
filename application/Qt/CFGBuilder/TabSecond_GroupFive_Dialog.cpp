/**/

#include "TabSecond_GroupFive_Dialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include "TabSecond_GroupFive_Item.h"
#include "CustomDefine.h"

TabSecond_GroupFive_Dialog::TabSecond_GroupFive_Dialog(QWidget *parent)
    :QDialog(parent)
{
    iListParent = NULL;
    iItem = NULL;

    // 排版
    iLayout = new QGridLayout;
    iLayout->setColumnStretch(4, 4);

    // 构造Dialog
    SetupDialog();

    this->setLayout(iLayout);
}

void TabSecond_GroupFive_Dialog::SetupDialog()
{
    SetupButtonadd();
    SetupButtonhorpos();
    SetupButtonverpos();
    SetupButtonmapto();
    SetupButtonLongpress();
    SetupButtonLocktime();
    SetupButtonbox();
}

void TabSecond_GroupFive_Dialog::SetupButtonadd()
{
    iButtonadd = new QLabel(this);
    iButtonadd->setText(QLabel::tr("button_name"));

    iButtonadd_list = new QComboBox(this);
    iButtonadd_list->addItem(QComboBox::tr("left"));
    iButtonadd_list->addItem(QComboBox::tr("right"));
    iButtonadd_list->addItem(QComboBox::tr("up"));
    iButtonadd_list->addItem(QComboBox::tr("down"));
    iButtonadd_list->addItem(QComboBox::tr("a"));
    iButtonadd_list->addItem(QComboBox::tr("b"));

    iLayout->addWidget(iButtonadd, 0, 0);
    iLayout->addWidget(iButtonadd_list, 0, 1);
}

void TabSecond_GroupFive_Dialog::SetupButtonhorpos()
{
    iButtonhorpos = new QLabel(this);
    iButtonhorpos->setText(QLabel::tr("button_hor_pos"));

    iButtonhorpos_input = new QLineEdit(this);

    iLayout->addWidget(iButtonhorpos, 1, 0);
    iLayout->addWidget(iButtonhorpos_input, 1, 1);
}

void TabSecond_GroupFive_Dialog::SetupButtonverpos()
{
    iButtonverpos = new QLabel(this);
    iButtonverpos->setText(QLabel::tr("button_ver_pos"));

    iButtonverpos_input = new QLineEdit(this);

    iLayout->addWidget(iButtonverpos, 1, 2);
    iLayout->addWidget(iButtonverpos_input, 1, 3);
}

void TabSecond_GroupFive_Dialog::SetupButtonmapto()
{
    iButtonmapto = new QLabel(this);
    iButtonmapto->setText(QLabel::tr("button_map_to"));

    iButtonmapto_input = new QLineEdit(this);

    iLayout->addWidget(iButtonmapto, 0, 2);
    iLayout->addWidget(iButtonmapto_input, 0, 3);
}

void TabSecond_GroupFive_Dialog::SetupButtonLongpress()
{
    iButtonLongpress = new QLabel(this);
    iButtonLongpress->setText(QLabel::tr("button_long_press"));

    iButtonLongpress_list = new QComboBox(this);
    iButtonLongpress_list->addItem(QComboBox::tr("true"));
    iButtonLongpress_list->addItem(QComboBox::tr("false"));
    iButtonLongpress_list->setCurrentIndex(1);

    iLayout->addWidget(iButtonLongpress, 2, 0);
    iLayout->addWidget(iButtonLongpress_list, 2, 1);
}

void TabSecond_GroupFive_Dialog::SetupButtonLocktime()
{
    iButtonLocktime = new QLabel(this);
    iButtonLocktime->setText(QLabel::tr("button_long_press_lock_time"));

    iButtonLocktime_input = new QLineEdit(this);

    iLayout->addWidget(iButtonLocktime, 2, 2);
    iLayout->addWidget(iButtonLocktime_input, 2, 3);
}

void TabSecond_GroupFive_Dialog::SetupButtonbox()
{
    iButtonbox = new QDialogButtonBox(this);

    iOk = new QPushButton;
    iOk->setText(QPushButton::tr("OK"));
    QObject::connect(iOk, SIGNAL(clicked()),
                     this, SLOT(ClickOK()));
    iButtonbox->addButton(iOk, QDialogButtonBox::ActionRole);

    iCancel = new QPushButton;
    iCancel->setText(QPushButton::tr("CANCEL"));
    QObject::connect(iCancel, SIGNAL(clicked()),
                     this, SLOT(close()));
    iButtonbox->addButton(iCancel, QDialogButtonBox::ActionRole);

    iLayout->addWidget(iButtonbox, 4, 3);
}

void TabSecond_GroupFive_Dialog::ModifyItem_Item(QListWidget *parent, QListWidgetItem *item)
{
    iListParent = parent;
    iItem = item;

    if(iItem!=NULL)
    {
        // Button_name
        if(((TabSecond_GroupFive_Item*)iItem)->Buttonadd()=="left")
            iButtonadd_list->setCurrentIndex(0);
        else if(((TabSecond_GroupFive_Item*)iItem)->Buttonadd()=="right")
            iButtonadd_list->setCurrentIndex(1);
        else if(((TabSecond_GroupFive_Item*)iItem)->Buttonadd()=="up")
            iButtonadd_list->setCurrentIndex(2);
        else if(((TabSecond_GroupFive_Item*)iItem)->Buttonadd()=="down")
            iButtonadd_list->setCurrentIndex(3);
        else if(((TabSecond_GroupFive_Item*)iItem)->Buttonadd()=="a")
            iButtonadd_list->setCurrentIndex(4);
        else if(((TabSecond_GroupFive_Item*)iItem)->Buttonadd()=="b")
            iButtonadd_list->setCurrentIndex(5);

        // Button_hor_pos
        iButtonhorpos_input->setText(((TabSecond_GroupFive_Item*)iItem)->Buttonhorpos());
        // Button_ver_pos
        iButtonverpos_input->setText(((TabSecond_GroupFive_Item*)iItem)->Buttonverpos());
        // Button_map_to
        iButtonmapto_input->setText(((TabSecond_GroupFive_Item*)iItem)->Buttonmapto());
        // Button_long_press
        if(((TabSecond_GroupFive_Item*)iItem)->ButtonLongpress()=="true")
            iButtonLongpress_list->setCurrentIndex(0);
        else if((((TabSecond_GroupFive_Item*)iItem)->ButtonLongpress()=="false"))
            iButtonLongpress_list->setCurrentIndex(1);
        // Button_long_press_lock_time
        iButtonLocktime_input->setText(((TabSecond_GroupFive_Item*)iItem)->ButtonLocktime());
    }
    else
    {
        iButtonadd_list->setCurrentIndex(0);
        iButtonhorpos_input->clear();
        iButtonverpos_input->clear();
        iButtonmapto_input->clear();
        iButtonLongpress_list->setCurrentIndex(1);
        iButtonLocktime_input->clear();
    }

    this->exec();
}

void TabSecond_GroupFive_Dialog::ClickOK()
{
    if(iListParent!=NULL)
    {
        if(iItem!=NULL)
        {
            if(iButtonmapto_input->text().length()==0)
                return;

            // Button_name
            if(iButtonadd_list->currentIndex()==0)
                ((TabSecond_GroupFive_Item*)iItem)->SetButtonadd("left");
            else if(iButtonadd_list->currentIndex()==1)
                ((TabSecond_GroupFive_Item*)iItem)->SetButtonadd("right");
            else if(iButtonadd_list->currentIndex()==2)
                ((TabSecond_GroupFive_Item*)iItem)->SetButtonadd("up");
            else if(iButtonadd_list->currentIndex()==3)
                ((TabSecond_GroupFive_Item*)iItem)->SetButtonadd("down");
            else if(iButtonadd_list->currentIndex()==4)
                ((TabSecond_GroupFive_Item*)iItem)->SetButtonadd("a");
            else if(iButtonadd_list->currentIndex()==5)
                ((TabSecond_GroupFive_Item*)iItem)->SetButtonadd("b");

            // Button_hor_pos
            ((TabSecond_GroupFive_Item*)iItem)->SetButtonhorpos(iButtonhorpos_input->text());
            // Button_ver_pos
            ((TabSecond_GroupFive_Item*)iItem)->SetButtonverpos(iButtonverpos_input->text());
            // Button_map_to
            ((TabSecond_GroupFive_Item*)iItem)->SetButtonmapto(iButtonmapto_input->text());
            // Button_long_press
            if(iButtonLongpress_list)
            {
                if(iButtonLongpress_list->currentIndex()==0)
                    ((TabSecond_GroupFive_Item*)iItem)->SetButtonLongpress("true");
                else if(iButtonLongpress_list->currentIndex()==1)
                    ((TabSecond_GroupFive_Item*)iItem)->SetButtonLongpress("false");
            }
            // Button_long_press_lock_time
            ((TabSecond_GroupFive_Item*)iItem)->SetButtonLocktime(iButtonLocktime_input->text());

            QString itemtitle;
            itemtitle += ((TabSecond_GroupFive_Item*)iItem)->Buttonadd();
            itemtitle += QString("\t->\t");
            itemtitle += ((TabSecond_GroupFive_Item*)iItem)->Buttonmapto();
            iItem->setText(itemtitle);
        }
        else
        {
            if(iButtonmapto_input->text().length()==0)
                return;

            TabSecond_GroupFive_Item* item = new TabSecond_GroupFive_Item;

            // Button_name
            if(iButtonadd_list->currentIndex()==0)
                item->SetButtonadd("left");
            else if(iButtonadd_list->currentIndex()==1)
                item->SetButtonadd("right");
            else if(iButtonadd_list->currentIndex()==2)
                item->SetButtonadd("up");
            else if(iButtonadd_list->currentIndex()==3)
                item->SetButtonadd("down");
            else if(iButtonadd_list->currentIndex()==4)
                item->SetButtonadd("a");
            else if(iButtonadd_list->currentIndex()==5)
                item->SetButtonadd("b");

            // Button_hor_pos
            item->SetButtonhorpos(iButtonhorpos_input->text());
            // Button_ver_pos
            item->SetButtonverpos(iButtonverpos_input->text());
            // Button_map_to
            item->SetButtonmapto(iButtonmapto_input->text());
            // Button_long_press
            if(iButtonLongpress_list)
            {
                if(iButtonLongpress_list->currentIndex()==0)
                    item->SetButtonLongpress("true");
                else if(iButtonLongpress_list->currentIndex()==1)
                    item->SetButtonLongpress("false");
            }
            // Button_long_press_lock_time
            item->SetButtonLocktime(iButtonLocktime_input->text());

            QString itemtitle;
            itemtitle += ((TabSecond_GroupFive_Item*)item)->Buttonadd();
            itemtitle += QString("\t->\t");
            itemtitle += ((TabSecond_GroupFive_Item*)item)->Buttonmapto();
            item->setText(itemtitle);

            iListParent->addItem(item);
        }
    }

    this->close();
}
