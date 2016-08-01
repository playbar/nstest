#include "TouchDefine_Dialog.h"
#include <QLabel>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QComboBox>
#include <QGridLayout>
#include "TouchDefine_Dialog_Pad.h"
#include <QFile>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTextStream>
#include "TouchInfo.h"
#include "TouchButtonInfo.h"
#include "TouchDefine_Dialog_Pad_Button.h"
#include "TouchDefine_Dialog_Pad_Screen.h"

// model.list中一项所包含的子项
const int ModelItemCount = 10;
const int ModelNameItem = 0;
const int ModelSizeWidthItem = 1;
const int ModelSizeHeightItem = 2;
const int ModelScreenSizeWidthItem = 3;
const int ModelScreenSizeHeightItem = 4;
const int ModelScreenOffsetWidthItem = 5;
const int ModelScreenOffsetHeightItem = 6;
const int ModelZoomItem = 7;
const int ModelDirItem = 8;
const int ModelPic = 9;

TouchDefine_Dialog::TouchDefine_Dialog(QWidget *parent, ModelDir dir) :
    QDialog(parent)
{
    iDir = dir;
    this->setWindowTitle(tr("Add Touch Button Dialog"));
    
    iLayout = new QBoxLayout(QBoxLayout::TopToBottom);
       
    CreateDialog();
    
    LoadModelList();
    
    this->setLayout(iLayout);
}

void TouchDefine_Dialog::Load()
{

}


void TouchDefine_Dialog::ModifyItem_Item(QListWidget *parent, QListWidgetItem *item)
{
    iButtonAdded_list.clear();
    
    iParent = parent;
    iItem = item;
    
    if(parent)
    {
        if(iItem)
        {
            for(int i=0; i<((TouchInfo*)item)->iButtonInfoList.count(); i++)
            {
                TouchDefine_Dialog_Pad_Button* button = new TouchDefine_Dialog_Pad_Button((QWidget*)(iModelPad->iScreen));
                button->iButtonAdded_list = &iButtonAdded_list;
                button->iStatus = TouchDefine_Dialog_Pad_Button::ButtonStatus_Screen;
                if(((TouchInfo*)item)->iButtonInfoList.at(i)->iLongPress=="true")
                    button->SetButtonLock(TouchDefine_Dialog_Pad_Button::ButtonLock_ON);
                else if(((TouchInfo*)item)->iButtonInfoList.at(i)->iLongPress=="false")
                    button->SetButtonLock(TouchDefine_Dialog_Pad_Button::ButtonLock_OFF);
                
                button->iOrg = ((TouchInfo*)item)->iButtonInfoList.at(i)->iOrg.toInt();
                
                button->iButtonPos.setX(((TouchInfo*)item)->iButtonInfoList.at(i)->iHorPos.toInt());
                button->iButtonPos.setY(((TouchInfo*)item)->iButtonInfoList.at(i)->iVerPos.toInt());
                button->iButtonLockTime = ((TouchInfo*)item)->iButtonInfoList.at(i)->iLockTime.toInt();
                button->iButtonMapto = ((TouchInfo*)item)->iButtonInfoList.at(i)->iMapto;
                
                if(((TouchInfo*)item)->iButtonInfoList.at(i)->iButtonName=="up")
                    button->SetButtonType(TouchDefine_Dialog_Pad_Button::Button_Up);
                else if(((TouchInfo*)item)->iButtonInfoList.at(i)->iButtonName=="down")
                    button->SetButtonType(TouchDefine_Dialog_Pad_Button::Button_Down);
                else if(((TouchInfo*)item)->iButtonInfoList.at(i)->iButtonName=="left")
                    button->SetButtonType(TouchDefine_Dialog_Pad_Button::Button_Left);
                else if(((TouchInfo*)item)->iButtonInfoList.at(i)->iButtonName=="right")
                    button->SetButtonType(TouchDefine_Dialog_Pad_Button::Button_Right);
                else if(((TouchInfo*)item)->iButtonInfoList.at(i)->iButtonName=="a")
                    button->SetButtonType(TouchDefine_Dialog_Pad_Button::Button_A);
                else if(((TouchInfo*)item)->iButtonInfoList.at(i)->iButtonName=="b")
                    button->SetButtonType(TouchDefine_Dialog_Pad_Button::Button_B); 
                
                iButtonAdded_list.append(button);
                
                connect(this, SIGNAL(signalChangeZoom(float)),
                        button, SLOT(slotChangeZoom(float)));
                
                if(iZoom_list->currentIndex()==0)
                    button->iZoom = 1.0;
                else if(iZoom_list->currentIndex()==1)
                    button->iZoom = 0.75;
                else if(iZoom_list->currentIndex()==2)
                    button->iZoom = 0.5;
                
                QPointF pos;
                QSizeF size = this->iModelPad->iScreen->iScreenSize;
                if(iDir==Dir_CW90 || iDir==Dir_ACW90)
                    size.transpose();
                if(button->iOrg==0)
                {
                    pos = button->iButtonPos.toPoint();
                }
                else if(button->iOrg==1)
                {
                    pos.rx() = size.width() - button->iButtonPos.toPoint().x() - button->iButtonSize.width();
                    pos.ry() = button->iButtonPos.toPoint().y();
                }
                else if(button->iOrg==3)
                {
                    pos.rx() = size.width() - button->iButtonPos.toPoint().x() - button->iButtonSize.width();
                    pos.ry() = size.height() - button->iButtonPos.toPoint().y() - button->iButtonSize.height();
                } 
                else if(button->iOrg==2)
                {
                    pos.rx() = button->iButtonPos.toPoint().x();
                    pos.ry() = size.height() - button->iButtonPos.toPoint().y() - button->iButtonSize.height();
                }
                
                button->move((pos*button->iZoom).toPoint());
            }
        }
        else
        {
            iButtonAdded_list.clear();
        }
        
        this->exec();
    }
    

}

void TouchDefine_Dialog::slotChangeModel(int index)
{
    ModelInfo model;
    model.ModelName = iModelList.at(index);
    model.ModelPicName = iModelPic.at(index);
    model.ModelSize = iModelSizeList.at(index);
    model.ModelScreenSize = iModelScreenSizeList.at(index);
    model.ModelScreenOffset = iModelScreenOffsetList.at(index);
    model.ModelZoom  = iModelZoomList.at(index);
    model.eModelDir = iDir;
    
    if(model.ModelZoom<=0.625)
    {
        model.ModelZoom = 0.5;
        iZoom_list->setCurrentIndex(2);
    }
    else if(model.ModelZoom <= 0.875)
    {
        model.ModelZoom = 0.75;
        iZoom_list->setCurrentIndex(1);
    }
    else
    {
        model.ModelZoom = 1.0;
        iZoom_list->setCurrentIndex(0);
    }
    
    QStringList screensize;
    if(iDir==Dir_Normal || iDir==Dir_CW180)
    {
        screensize.append(QString().number(model.ModelScreenSize.width()));
        screensize.append(QString().number(model.ModelScreenSize.height()));
    }
    else
    {
        screensize.append(QString().number(model.ModelScreenSize.height()));
        screensize.append(QString().number(model.ModelScreenSize.width()));
    }
    iScreenSize_input->setText(screensize.join("x"));

    emit signalChangeModel(model);
}

void TouchDefine_Dialog::slotChangeZoom(int index)
{
    float zoom = 0;
    switch(index)
    {
    case 0:
        zoom = 1.0;
        break;
    case 1:
        zoom = 0.75;
        break;
    case 2:
        zoom = 0.5;
        break;
    default:
        zoom = 1.0;
        break;
    }
    
    emit signalChangeZoom(zoom);
}

void TouchDefine_Dialog::slotOKClicked()
{
    if(iParent!=NULL)
    {
        iParent->clear();
        
        for(int i=0; i<iButtonAdded_list.count(); i++)
        {
            TouchButtonInfo* info = new TouchButtonInfo;
            switch(iButtonAdded_list.at(i)->iType)
            {
            case TouchDefine_Dialog_Pad_Button::Button_Up:
                {
                    info->iButtonName = "up";
                }
                break;
            case TouchDefine_Dialog_Pad_Button::Button_Down:
                {
                    info->iButtonName = "down";
                }
                break;
            case TouchDefine_Dialog_Pad_Button::Button_Left:
                {
                    info->iButtonName = "left";
                }
                break;
            case TouchDefine_Dialog_Pad_Button::Button_Right:
                {
                    info->iButtonName = "right";
                }
                break;
            case TouchDefine_Dialog_Pad_Button::Button_A:
                {
                    info->iButtonName = "a";
                }
                break;
            case TouchDefine_Dialog_Pad_Button::Button_B:
                {
                    info->iButtonName = "b";
                }
                break;
            }
            
            info->iOrg = QString().number(iButtonAdded_list.at(i)->iOrg);
            info->iHorPos = QString().number(iButtonAdded_list.at(i)->iButtonPos.x());
            info->iVerPos = QString().number(iButtonAdded_list.at(i)->iButtonPos.y());
            info->iMapto = iButtonAdded_list.at(i)->iButtonMapto;
            if(iButtonAdded_list.at(i)->iLongPressLock==TouchDefine_Dialog_Pad_Button::ButtonLock_ON)
            {
                info->iLongPress = "true";
                info->iLockTime = QString().number(iButtonAdded_list.at(i)->iButtonLockTime);
            }
            
            QString title;
            title += info->iButtonName;
            title += "\t->\t";
            title += info->iMapto;
            info->setText(title);
            iParent->addItem(info);
        }
        
        this->close();
    }
}

void TouchDefine_Dialog::CreateDialog()
{
    CreateModelPic();
    CreateModelList();
    CreateZoomList();
    CreateScreenLED();
    CreateButtonLED();
    CreateButtonNeedAdded();
    CreateButtonBox();
}

void TouchDefine_Dialog::CreateModelPic()
{
    iModelPad = new TouchDefine_Dialog_Pad(this);
    iModelPad->AddButtonListP(&iButtonAdded_list);
    
    connect(this, SIGNAL(signalChangeModel(ModelInfo)),
            iModelPad, SLOT(slotChangeModel(ModelInfo)));
    connect(this, SIGNAL(signalChangeZoom(float)),
            iModelPad, SLOT(slotChangeZoom(float)));
    
    // 排版
    iSubLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    iSubLayout->addWidget(iModelPad);
}

void TouchDefine_Dialog::CreateModelList()
{
    iAllModel = new QLabel(tr("Model"), this);
    iAllModel_list = new QComboBox(this);
    connect(iAllModel_list, SIGNAL(currentIndexChanged(int)), 
            this, SLOT(slotChangeModel(int)));
    
    // 添加到排版中
    iSubLayout1 = new QGridLayout;
    iSubLayout1->addWidget(iAllModel, 0, 0);
    iSubLayout1->addWidget(iAllModel_list, 0, 1);
}

void TouchDefine_Dialog::CreateZoomList()
{
    iZoom = new QLabel(tr("Zoom"), this);
    iZoom_list = new QComboBox(this);
    connect(iZoom_list, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotChangeZoom(int)));
    
    // 添加到排版中
    iSubLayout1->addWidget(iZoom, 1, 0);
    iSubLayout1->addWidget(iZoom_list, 1, 1);
}

void TouchDefine_Dialog::CreateScreenLED()
{
    iScreenSize = new QLabel(tr("ScreenSize"), this);
    iScreenSize_input = new QLineEdit(this);
    iScreenSize_input->setAlignment(Qt::AlignHCenter);
    iScreenSize_input->setReadOnly(true);
    
    // 添加到排版中
    iSubLayout1->addWidget(iScreenSize, 2, 0);
    iSubLayout1->addWidget(iScreenSize_input, 2, 1);
}

void TouchDefine_Dialog::CreateButtonLED()
{
    iCurButtonPos = new QLabel(tr("CurButtonPos"), this);
    iCurButtonPos_input = new QLineEdit(this);
    iCurButtonPos_input->setAlignment(Qt::AlignHCenter);
    iCurButtonPos_input->setReadOnly(true);
    connect(iModelPad, SIGNAL(signalCurButtonPos(QString)),
            iCurButtonPos_input, SLOT(setText(QString)));
    
    // 添加到排版中
    iSubLayout1->addWidget(iCurButtonPos, 3, 0);
    iSubLayout1->addWidget(iCurButtonPos_input, 3, 1);
}

void TouchDefine_Dialog::CreateButtonNeedAdded()
{
    TouchDefine_Dialog_Pad_Button* ButtonUp = new TouchDefine_Dialog_Pad_Button(this);
    ButtonUp->SetButtonType(TouchDefine_Dialog_Pad_Button::Button_Up);
    connect(this, SIGNAL(signalChangeZoom(float)),
            ButtonUp, SLOT(slotChangeZoom(float)));
    iButton_list.append(ButtonUp);
    TouchDefine_Dialog_Pad_Button* ButtonDown = new TouchDefine_Dialog_Pad_Button(this);
    ButtonDown->SetButtonType(TouchDefine_Dialog_Pad_Button::Button_Down);
    connect(this, SIGNAL(signalChangeZoom(float)),
            ButtonDown, SLOT(slotChangeZoom(float)));
    iButton_list.append(ButtonDown);
    TouchDefine_Dialog_Pad_Button* ButtonLeft = new TouchDefine_Dialog_Pad_Button(this);
    ButtonLeft->SetButtonType(TouchDefine_Dialog_Pad_Button::Button_Left);
    connect(this, SIGNAL(signalChangeZoom(float)),
            ButtonLeft, SLOT(slotChangeZoom(float)));
    iButton_list.append(ButtonLeft);
    TouchDefine_Dialog_Pad_Button* ButtonRight = new TouchDefine_Dialog_Pad_Button(this);
    ButtonRight->SetButtonType(TouchDefine_Dialog_Pad_Button::Button_Right);
    connect(this, SIGNAL(signalChangeZoom(float)),
            ButtonRight, SLOT(slotChangeZoom(float)));
    iButton_list.append(ButtonRight);
    TouchDefine_Dialog_Pad_Button* ButtonA = new TouchDefine_Dialog_Pad_Button(this);
    ButtonA->SetButtonType(TouchDefine_Dialog_Pad_Button::Button_A);
    connect(this, SIGNAL(signalChangeZoom(float)),
            ButtonA, SLOT(slotChangeZoom(float)));
    iButton_list.append(ButtonA);
    TouchDefine_Dialog_Pad_Button* ButtonB = new TouchDefine_Dialog_Pad_Button(this);
    ButtonB->SetButtonType(TouchDefine_Dialog_Pad_Button::Button_B);
    connect(this, SIGNAL(signalChangeZoom(float)),
            ButtonB, SLOT(slotChangeZoom(float)));
    iButton_list.append(ButtonB);
                      
    // 排版
    iSubLayout2 = new QGridLayout;
    iSubLayout2->addWidget(ButtonUp, 0, 1);
    iSubLayout2->addWidget(ButtonLeft, 1, 0);
    iSubLayout2->addWidget(ButtonDown, 1, 1);
    iSubLayout2->addWidget(ButtonRight, 1, 2);
    iSubLayout2->addWidget(ButtonA, 2, 0);
    iSubLayout2->addWidget(ButtonB, 2, 2);
    
    iSubLayout1->addLayout(iSubLayout2, 4, 0, 1, 2);
    iSubLayout->addLayout(iSubLayout1);
    iLayout->addLayout(iSubLayout);
}

void TouchDefine_Dialog::CreateButtonBox()
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
    
    // 添加到排版中
    iLayout->addWidget(iButtonBox);
}

void TouchDefine_Dialog::LoadModelList()
{
    // 清空数据
    iModelList.clear();
    iModelSizeList.clear();
    iModelScreenSizeList.clear();
    iModelScreenOffsetList.clear();
    iModelZoomList.clear();
    iModelDirList.clear();
    iModelPic.clear();
    
    // 逐行读取数据，#开头为注释
    QFile file("model.list");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream read(&file);
        while(!read.atEnd())
        {
            QString str = read.readLine();
            if(str.length()==0) // 空行
                continue;
            else if(str.mid(0,1)=="#")  // 注释
                continue;
            else
            {
                QStringList strlist = str.split(";");
                if(strlist.count()!=ModelItemCount)
                    continue;
                else
                {
                    QString modelname = strlist.at(ModelNameItem);
                    QSizeF modelsize;
                    modelsize.setWidth(strlist.at(ModelSizeWidthItem).toFloat());
                    modelsize.setHeight(strlist.at(ModelSizeHeightItem).toFloat());
                    QSizeF modelscreensize;
                    modelscreensize.setWidth(strlist.at(ModelScreenSizeWidthItem).toFloat());
                    modelscreensize.setHeight(strlist.at(ModelScreenSizeHeightItem).toFloat());
                    QSizeF modelscreenoffset;
                    modelscreenoffset.setWidth(strlist.at(ModelScreenOffsetWidthItem).toFloat());
                    modelscreenoffset.setHeight(strlist.at(ModelScreenOffsetHeightItem).toFloat());
                    float modelzoom;
                    modelzoom = strlist.at(ModelZoomItem).toFloat();
                    // 方向
                    ModelDir dir;
                    switch(strlist.at(ModelDirItem).toInt())
                    {
                    case 0:
                        dir = Dir_Normal;
                        break;
                    case 1:
                        dir = Dir_CW90;
                        break;
                    case 2:
                        dir = Dir_CW180;
                        break;
                    case 3:
                        dir = Dir_ACW90;
                        break;
                    default:
                        break;
                    }
                    
                    QString picname = strlist.at(ModelPic);

                    iModelList.append(modelname);
                    iModelPic.append(picname);
                    iModelSizeList.append(modelsize);
                    iModelScreenSizeList.append(modelscreensize);
                    iModelScreenOffsetList.append(modelscreenoffset);
                    iModelZoomList.append(modelzoom);
                    iModelDirList.append(dir);
                }
            }
        } 
    }
    
    // 给ZoomList添加数据
    iZoom_list->addItem(tr("100%"));
    iZoom_list->addItem(tr(" 75%"));
    iZoom_list->addItem(tr(" 50%"));
    // 给ModelList添加数据
    for(int i=0; i<iModelList.count(); i++)
    {
        iAllModel_list->addItem(iModelList.at(i));
    }
    if(iAllModel_list->count()>0)
        iAllModel_list->setCurrentIndex(0);
}
