#include "TouchDefine_Dialog_Pad_Position.h"
#include <QBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include "TouchDefine_Dialog_Pad_Button.h"

TouchDefine_Dialog_Pad_Position::TouchDefine_Dialog_Pad_Position(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle(tr("Button Positon"));
    iZoom = 1.0;
    iButton = NULL;
    iLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    
    CreateDialog();
    
    this->setLayout(iLayout);
}

void TouchDefine_Dialog_Pad_Position::InitDialog(QSizeF screensize, float zoom, int org, TouchDefine_Dialog_Pad_Button *button)
{
    iScreenSize = screensize;
    iZoom = zoom;
    iButton = button;
    iOrg = org;
    
    iXValue->setValue(iButton->iButtonPos.x());
    iYValue->setValue(iButton->iButtonPos.y());
    
    bool r = false;
    bool b = false;
    if(iOrg==0 || iOrg==2)
    {
        r = false;
        if(iOrg==2)
            b = true;
        else
            b = false;
    }
    else if(iOrg==1 || iOrg==3)
    {
        r = true;
        if(iOrg==3)
            b = true;
        else 
            b = false;
    }
    
    iR->setChecked(r);
    iB->setChecked(b);
}

void TouchDefine_Dialog_Pad_Position::CreateDialog()
{
    CreateX();
    CreateY();
    CreateButtonBox();
}

void TouchDefine_Dialog_Pad_Position::CreateX()
{
    iXLabel = new QLabel(tr("X Pos:"), this);
    iXValue = new QSpinBox(this);
    iXValue->setRange(-1000, 1000);
    iR = new QCheckBox(tr("R"), this);
    connect(iR, SIGNAL(clicked(bool)),
	    this, SLOT(slotRChecked(bool)));
    
    QHBoxLayout* sublayout = new QHBoxLayout;
    sublayout->setAlignment(Qt::AlignHCenter);
    sublayout->addWidget(iXLabel);
    sublayout->addWidget(iXValue);
    sublayout->addWidget(iR);
    
    iLayout->addLayout(sublayout);
}

void TouchDefine_Dialog_Pad_Position::CreateY()
{
    iYLabel = new QLabel(tr("Y Pos:"), this);
    iYValue = new QSpinBox(this);
    iYValue->setRange(-1000, 1000);
    iB = new QCheckBox(tr("B"), this);
    connect(iB, SIGNAL(clicked(bool)),
	    this, SLOT(slotBChecked(bool)));
    
    QHBoxLayout* sublayout = new QHBoxLayout;
    sublayout->setAlignment(Qt::AlignHCenter);
    sublayout->addWidget(iYLabel);
    sublayout->addWidget(iYValue);
    sublayout->addWidget(iB);
    
    iLayout->addLayout(sublayout);
}

void TouchDefine_Dialog_Pad_Position::CreateButtonBox()
{
    iButtonBox = new QDialogButtonBox(this);
    iOk = new QPushButton(tr("OK"), iButtonBox);
    connect(iOk, SIGNAL(clicked()),
	    this, SLOT(slotClickOK()));
    iButtonBox->addButton(iOk, QDialogButtonBox::ActionRole);
    
    iCancel = new QPushButton(tr("Cancel"), iButtonBox);
    connect(iCancel, SIGNAL(clicked()),
	    this, SLOT(close()));
    iButtonBox->addButton(iCancel, QDialogButtonBox::ActionRole);
    
    iLayout->addWidget(iButtonBox);
}

void TouchDefine_Dialog_Pad_Position::slotRChecked(bool b)
{
    // 如果R is Checked
    if(b)
    {
	iXValue->setValue(iScreenSize.width()-iXValue->value()-37);
        if(iB->isChecked())
        {
            iOrg = 3;
        }
        else
        {
            iOrg = 1;
        }
    }
    // 如果R is UnChecked
    else
    {
	iXValue->setValue(iScreenSize.width()-iXValue->value()-37);
        if(iB->isChecked())
        {
            iOrg = 2;
        }
        else
        {
            iOrg = 0;
        }
    }
}

void TouchDefine_Dialog_Pad_Position::slotBChecked(bool b)
{
    // 如果B is Checked
    if(b)
    {
        iYValue->setValue(iScreenSize.height()-iYValue->value()-37);
        if(iR->isChecked())
            iOrg = 3;
        else
            iOrg = 2;
    }
    // 如果B is UnChecked
    else
    {
        iYValue->setValue(iScreenSize.height()-iYValue->value()-37);
        if(iR->isChecked())
            iOrg = 1;
        else
            iOrg = 3;
    }
}

void TouchDefine_Dialog_Pad_Position::slotClickOK()
{
    QPointF pos;
    QSizeF offset;
    // 这里为button的size，因为不知道啥原因出错，所以指定为37x37
    offset.setWidth(37);
    offset.setHeight(37);
      
    if(iR->isChecked())
    {
        if(iB->isChecked())
            iOrg = 3;
        else
            iOrg = 1;
    }
    else
    {
        if(iB->isChecked())
            iOrg = 2;
        else
            iOrg = 0;
    }
        
    iButton->iOrg = iOrg;
    iButton->iButtonPos = QPointF(iXValue->value(), iYValue->value());
    
//    if(iOrg==1)
//    {
//        pos.setX(iXValue->value());
//        pos.setY(iYValue->value());
//    }
//    else if(iOrg==2)
//    {
//        pos.setX(iScreenSize.width()-iXValue->value()-offset.width());
//        pos.setY(iYValue->value());
//    }
//    else if(iOrg==3)
//    {
//        pos.setX(iScreenSize.width()-iXValue->value()-offset.width());
//        pos.setY(iScreenSize.height()-iYValue->value()-offset.height());
//    }
//    else if(iOrg==4)
//    {
//        pos.setX(iXValue->value());
//        pos.setY(iScreenSize.height()-iYValue->value()-offset.height());
//    }
    
////    if(iR->isChecked())
////        pos.setX(iScreenSize.width()-iXValue->value()-offset.width());
////    else
////        pos.setX(iXValue->value());
////    if(iB->isChecked())
////        pos.setY(iScreenSize.height()-iYValue->value()-offset.height());
////    else
////        pos.setY(iYValue->value());
//    pos *= iZoom;
    
////    iButton->move(pos.toPoint());
////    iButton->move(50, 50);
    
    this->close();
}