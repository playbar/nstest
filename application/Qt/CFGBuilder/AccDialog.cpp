#include "AccDialog.h"
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QRadioButton>
#include "xpopupkeymapto_lineedit.h"
#include "AccGroup_Item.h"

AccDialog::AccDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle(tr("AccSet"));
    
    iParent = NULL;
    iItem = NULL;
    
    iLayout = new QGridLayout;
    
    CreateDialog();
    this->setLayout(iLayout);
}

void AccDialog::ModifyItem_Item(QListWidget *parent, QListWidgetItem *item)
{
    iParent = parent;
    iItem = item;
    
    if(iItem!=NULL)
    {
        if(((AccGroup_Item*)item)->iDirection=="x")
            iDirection_List->setCurrentIndex(0);
        else if(((AccGroup_Item*)item)->iDirection=="-x")
            iDirection_List->setCurrentIndex(1);
        else if(((AccGroup_Item*)item)->iDirection=="y")
            iDirection_List->setCurrentIndex(2);
        else if(((AccGroup_Item*)item)->iDirection=="-y")
            iDirection_List->setCurrentIndex(3);
        else if(((AccGroup_Item*)item)->iDirection=="z")
            iDirection_List->setCurrentIndex(4);
        else if(((AccGroup_Item*)item)->iDirection=="-z")
            iDirection_List->setCurrentIndex(5);
        else
            iDirection_List->setCurrentIndex(0);
        
        iAccSpeed_input->setText(((AccGroup_Item*)item)->iAccSpeed);
        iStep_input->setText(((AccGroup_Item*)item)->iStep);
        
        if(((AccGroup_Item*)item)->BMapto)
        {
            slotMaptoChecked(true);
            iMapto_Input->setText(((AccGroup_Item*)item)->iMapto);
        }
        else if(((AccGroup_Item*)item)->BMouseAct)
        {
            slotMouseActChecked(true);
            if(((AccGroup_Item*)item)->iMouseAct=="up")
                iMouseAct_List->setCurrentIndex(0);
            else if(((AccGroup_Item*)item)->iMouseAct=="down")
                iMouseAct_List->setCurrentIndex(1);
            else if(((AccGroup_Item*)item)->iMouseAct=="left")
                iMouseAct_List->setCurrentIndex(2);
            else if(((AccGroup_Item*)item)->iMouseAct=="right")
                iMouseAct_List->setCurrentIndex(3);
            else if(((AccGroup_Item*)item)->iMouseAct=="left-key")
                iMouseAct_List->setCurrentIndex(4);
            else if(((AccGroup_Item*)item)->iMouseAct=="right-key")
                iMouseAct_List->setCurrentIndex(5);
            else
                iMouseAct_List->setCurrentIndex(0);
        }
        
        if(((AccGroup_Item*)item)->iOnlyOne=="true")
            iOnlyOne_List->setCurrentIndex(0);
        else
            iOnlyOne_List->setCurrentIndex(1);
    }
    else
    {
        iDirection_List->setCurrentIndex(0);
        iAccSpeed_input->clear();
        iStep_input->clear();
        iMapto_Input->clear();
        iMouseAct_List->setCurrentIndex(0);
        iOnlyOne_List->setCurrentIndex(1);
        slotMaptoChecked(true);
    }
    
    this->exec();
}

// slots
// Mapto Check slot
void AccDialog::slotMaptoChecked(bool b)
{
    if(b)
    {
        iMapto->setChecked(true);
        iMapto_Input->setEnabled(true);
        iMouseAct_List->setEnabled(false);
    }
}

// MouseAct Check slot
void AccDialog::slotMouseActChecked(bool b)
{
    if(b)
    {
        iMouseAct->setChecked(true);
        iMapto_Input->setEnabled(false);
        iMouseAct_List->setEnabled(true);
    }
}

// 点击OK
void AccDialog::slotOKClicked()
{
    if(iParent)
    {
        if(iItem)
        {
            ((AccGroup_Item*)iItem)->iDirection = iDirection_List->currentText();
            ((AccGroup_Item*)iItem)->iAccSpeed = iAccSpeed_input->text();
            ((AccGroup_Item*)iItem)->iStep = iStep_input->text();
            ((AccGroup_Item*)iItem)->BMapto = iMapto->isChecked();
            ((AccGroup_Item*)iItem)->iMapto = iMapto_Input->text();
            ((AccGroup_Item*)iItem)->BMouseAct = iMouseAct->isChecked();
            ((AccGroup_Item*)iItem)->iMouseAct = iMouseAct_List->currentText();
            ((AccGroup_Item*)iItem)->iOnlyOne = iOnlyOne_List->currentText();
            
            QString itemtitle;
            itemtitle +=  ((AccGroup_Item*)iItem)->iDirection;
            itemtitle += QString("\t->\t");
            if( ((AccGroup_Item*)iItem)->BMapto)
                itemtitle +=  ((AccGroup_Item*)iItem)->iMapto;
            else
                itemtitle +=  ((AccGroup_Item*)iItem)->iMouseAct;
            
            iItem->setText(itemtitle);
        }
        else
        {
            iItem = new AccGroup_Item;
            
            ((AccGroup_Item*)iItem)->iDirection = iDirection_List->currentText();
            ((AccGroup_Item*)iItem)->iAccSpeed = iAccSpeed_input->text();
            ((AccGroup_Item*)iItem)->iStep = iStep_input->text();
            ((AccGroup_Item*)iItem)->BMapto = iMapto->isChecked();
            ((AccGroup_Item*)iItem)->iMapto = iMapto_Input->text();
            ((AccGroup_Item*)iItem)->BMouseAct = iMouseAct->isChecked();
            ((AccGroup_Item*)iItem)->iMouseAct = iMouseAct_List->currentText();
            ((AccGroup_Item*)iItem)->iOnlyOne = iOnlyOne_List->currentText();
            
            QString itemtitle;
            itemtitle +=  ((AccGroup_Item*)iItem)->iDirection;
            itemtitle += QString("\t->\t");
            if( ((AccGroup_Item*)iItem)->BMapto)
                itemtitle +=  ((AccGroup_Item*)iItem)->iMapto;
            else
                itemtitle +=  ((AccGroup_Item*)iItem)->iMouseAct;
            
            iItem->setText(itemtitle);
            
            iParent->addItem(iItem);
        }
    }
    
    this->close();
}

void AccDialog::CreateDialog()
{
    CreateDirection();
    CreateAccSpeed();
    CreateStep();
    CreateMapto();
    CreateMouseAct();
    CreateOnlyOne();
    CreateButtonBox();
}

void AccDialog::CreateDirection()
{
    iDirection = new QLabel(tr("Direction"), this);
    iDirection_List = new QComboBox(this);
    iDirection_List->addItem(tr("x"));
    iDirection_List->addItem(tr("-x"));
    iDirection_List->addItem(tr("y"));
    iDirection_List->addItem(tr("-y"));
    iDirection_List->addItem(tr("z"));
    iDirection_List->addItem(tr("-z"));
    
    iLayout->addWidget(iDirection, 0, 0);
    iLayout->addWidget(iDirection_List, 0, 1);
}

void AccDialog::CreateAccSpeed()
{
    iAccSpeed = new QLabel(tr("AccSpeed"), this);
    iAccSpeed_input = new QLineEdit(this);
    iAccSpeed_input->setAlignment(Qt::AlignRight);
    iAccSpeed_input->clear();
    
    iLayout->addWidget(iAccSpeed, 1, 0);
    iLayout->addWidget(iAccSpeed_input, 1, 1);
}

void AccDialog::CreateStep()
{
    iStep = new QLabel(tr("Step"), this);
    iStep_input = new QLineEdit(this);
    iStep_input->setAlignment(Qt::AlignRight);
    iStep_input->clear();
    
    iLayout->addWidget(iStep, 2, 0);
    iLayout->addWidget(iStep_input, 2, 1);
}

void AccDialog::CreateMapto()
{
    iMapto = new QRadioButton(tr("Mapto"), this);
    connect(iMapto, SIGNAL(clicked(bool)),
            this, SLOT(slotMaptoChecked(bool)));
    iMapto_Input = new XPopupKeyMapTo_LineEdit(this);
    
    iLayout->addWidget(iMapto, 3, 0);
    iLayout->addWidget(iMapto_Input, 3, 1);
}

void AccDialog::CreateMouseAct()
{
    iMouseAct = new QRadioButton(tr("MouseAct"), this);
    connect(iMouseAct, SIGNAL(clicked(bool)),
            this, SLOT(slotMouseActChecked(bool)));
    iMouseAct_List = new QComboBox(this);
    iMouseAct_List->addItem(tr("up"));
    iMouseAct_List->addItem(tr("down"));
    iMouseAct_List->addItem(tr("left"));
    iMouseAct_List->addItem(tr("right"));
    iMouseAct_List->addItem(tr("left-key"));
    iMouseAct_List->addItem(tr("right-key"));
    
    slotMaptoChecked(true);
    
    iLayout->addWidget(iMouseAct, 4, 0);
    iLayout->addWidget(iMouseAct_List, 4, 1);
}

void AccDialog::CreateOnlyOne()
{
    iOnlyOne = new QLabel(tr("OnlyOne"), this);
    iOnlyOne_List = new QComboBox(this);
    iOnlyOne_List->addItem(tr("true"));
    iOnlyOne_List->addItem(tr("false"));
    iOnlyOne_List->setCurrentIndex(1);
    
    iLayout->addWidget(iOnlyOne, 5, 0);
    iLayout->addWidget(iOnlyOne_List, 5, 1);
}

void AccDialog::CreateButtonBox()
{
    iButtonBox = new QDialogButtonBox(this);
    iOk = new QPushButton(tr("OK"), this);
    connect(iOk, SIGNAL(clicked()),
            this, SLOT(slotOKClicked()));
    iButtonBox->addButton(iOk, QDialogButtonBox::ActionRole);
    
    iCancel = new QPushButton(tr("Cancel"), this);
    connect(iCancel, SIGNAL(clicked()),
            this, SLOT(close()));
    iButtonBox->addButton(iCancel, QDialogButtonBox::ActionRole);
    
    iLayout->addWidget(iButtonBox, 6, 0, 1, 2);
}
