/*
 *
 */

#include "TabSecond_GroupFour_Dialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QListWidgetItem>
#include "TabSecond_GroupFour_Item.h"
#include "CustomDefine.h"
#include <QSpinBox>
#include "xpopuplineedit.h"
#include "xpopupkeymapto_lineedit.h"

TabSecond_GroupFour_Dialog::TabSecond_GroupFour_Dialog(QWidget *parent)
    :QDialog(parent)
{
    iParent = NULL;
    iItem = NULL;

    // 排版
    iLayout = new QGridLayout;
    iLayout->setColumnStretch(4, 3);

    // 构造
    SetupDialog();

    this->setLayout(iLayout);
}

void TabSecond_GroupFour_Dialog::ModifyItem_Item(QListWidget* parent, QListWidgetItem* item)
{
    iParent = parent;
    iItem = item;

    if(iItem!=NULL)
    {
        iKeypress_input->setText(((TabSecond_GroupFour_Item*)iItem)->Keypress());
        iKeymapto_input->setText(((TabSecond_GroupFour_Item*)iItem)->Keymapto());
        iLocktime_input->setText(((TabSecond_GroupFour_Item*)iItem)->Locktime());

        if(((TabSecond_GroupFour_Item*)iItem)->Longpress()=="true")
            iLongpress_list->setCurrentIndex(0);
        else
            iLongpress_list->setCurrentIndex(1);
    }
    else
    {
        iKeypress_input->clear();
        iKeymapto_input->clear();
        iLocktime_input->clear();
        iLongpress_list->setCurrentIndex(1);
    }

    this->exec();
}

void TabSecond_GroupFour_Dialog::SetupDialog()
{
    this->setWindowTitle(QDialog::tr("key_map"));

    SetupKeypress();
    SetupKeymapto();
    SetupLongpress();
    SetupLocktime();
    SetupButtonbox();

    QWidget::setTabOrder(iOk, iCancel);
    QWidget::setTabOrder(iCancel, iKeypress_input);
    QWidget::setTabOrder(iKeypress_input, iKeymapto_input);
    QWidget::setTabOrder(iKeymapto_input, iLongpress_list);
    QWidget::setTabOrder(iLongpress_list, iLocktime_input);
}

void TabSecond_GroupFour_Dialog::SetupKeypress()
{
    iKeypress = new QLabel(this);
    iKeypress->setText(QLabel::tr("key_pressed"));

//    iKeypress_input = new QLineEdit(this);
    iKeypress_input = new XPopupLineEdit(this);

    iLayout->addWidget(iKeypress, 0, 0);
    iLayout->addWidget(iKeypress_input, 0, 1);
}

void TabSecond_GroupFour_Dialog::SetupKeymapto()
{
    iKeymapto = new QLabel(this);
    iKeymapto->setText(QLabel::tr("key_map_to"));

//    iKeymapto_input = new QLineEdit(this);
    iKeymapto_input = new XPopupKeyMapTo_LineEdit(this);

    iLayout->addWidget(iKeymapto, 0, 2);
    iLayout->addWidget(iKeymapto_input, 0, 3);
}

void TabSecond_GroupFour_Dialog::SetupLongpress()
{
    iLongpress = new QLabel(this);
    iLongpress->setText(QLabel::tr("long_press"));

    iLongpress_list = new QComboBox(this);
    iLongpress_list->addItem(QComboBox::tr("true"));
    iLongpress_list->addItem(QComboBox::tr("false"));
    iLongpress_list->setCurrentIndex(1);

    iLayout->addWidget(iLongpress, 1, 0);
    iLayout->addWidget(iLongpress_list, 1, 1);
}

void TabSecond_GroupFour_Dialog::SetupLocktime()
{
    iLocktime = new QLabel(this);
    iLocktime->setText(QLabel::tr("long_press_lock_time"));

    iLocktime_input = new QLineEdit(this);
//    iLocktime_input->setAlignment(Qt::AlignRight);

    iLayout->addWidget(iLocktime, 1, 2);
    iLayout->addWidget(iLocktime_input, 1, 3);
}

void TabSecond_GroupFour_Dialog::SetupButtonbox()
{
    iButtonbox = new QDialogButtonBox(this);

    iOk = new QPushButton(this);
    iOk->setText(QPushButton::tr("OK"));
    QObject::connect(iOk, SIGNAL(clicked()),
                     this, SLOT(ClickOK()));
    iButtonbox->addButton(iOk, QDialogButtonBox::ActionRole);

    iCancel = new QPushButton(this);
    iCancel->setText(QPushButton::tr("CANCEL"));
    QObject::connect(iCancel, SIGNAL(clicked()),
                     this, SLOT(close()));
    iButtonbox->addButton(iCancel, QDialogButtonBox::ActionRole);

    iLayout->addWidget(iButtonbox, 2, 3);
}

void TabSecond_GroupFour_Dialog::ClickOK()
{
    if(iParent!=NULL)
    {
        if(iItem!=NULL)
        {
            if(iKeypress_input->text().length()==0 || iKeymapto_input->text().length()==0)
                return;

            ((TabSecond_GroupFour_Item*)iItem)->SetKeypress(iKeypress_input->text());
            ((TabSecond_GroupFour_Item*)iItem)->SetKeymapto(iKeymapto_input->text());
            if(iLongpress_list->currentIndex()==0)
                ((TabSecond_GroupFour_Item*)iItem)->SetLongpress("true");
            else
                ((TabSecond_GroupFour_Item*)iItem)->SetLongpress("false");

            ((TabSecond_GroupFour_Item*)iItem)->SetLocktime(iLocktime_input->text());

            QString itemtitle;
            itemtitle += ((TabSecond_GroupFour_Item*)iItem)->Keypress();
            itemtitle += QString("\t->\t");
            itemtitle += ((TabSecond_GroupFour_Item*)iItem)->Keymapto();
            iItem->setText(itemtitle);
        }
        else
        {
            if(iKeypress_input->text().length()==0 || iKeymapto_input->text().length()==0)
                return;

            iItem = new TabSecond_GroupFour_Item;

            ((TabSecond_GroupFour_Item*)iItem)->SetKeypress(iKeypress_input->text());
            ((TabSecond_GroupFour_Item*)iItem)->SetKeymapto(iKeymapto_input->text());
            if(iLongpress_list->currentIndex()==0)
                ((TabSecond_GroupFour_Item*)iItem)->SetLongpress("true");
            else
                ((TabSecond_GroupFour_Item*)iItem)->SetLongpress("false");

            ((TabSecond_GroupFour_Item*)iItem)->SetLocktime(iLocktime_input->text());

            QString itemtitle;
            itemtitle += ((TabSecond_GroupFour_Item*)iItem)->Keypress();
            itemtitle += QString("\t->\t");
            itemtitle += ((TabSecond_GroupFour_Item*)iItem)->Keymapto();
            iItem->setText(itemtitle);

            iParent->addItem(iItem);
        }
    }

    this->close();
}
