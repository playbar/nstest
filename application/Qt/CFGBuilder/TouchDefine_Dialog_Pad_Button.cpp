#include "TouchDefine_Dialog_Pad_Button.h"
#include <QMenu>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QMimeData>
#include <QInputDialog>
#include "TouchDefine_Dialog_Pad_Screen.h"
#include "TouchDefine_Dialog_Pad_Position.h"
#include "xtouchpopup_dialog_mapto.h"
#include "TouchDefine_Dialog_Pad_Mapto.h"

TouchDefine_Dialog_Pad_Button::TouchDefine_Dialog_Pad_Button(QWidget *parent) :
    QLabel(parent)
{
    iType = Button_Up;
    iStatus = ButtonStatus_Wait;
    iLongPressLock = ButtonLock_OFF;
    
    iButtonLockTime = 1000;
    iZoom = 1.0;
    iOrg = 0;
    
    iButtonAdded_list = NULL;
}

void TouchDefine_Dialog_Pad_Button::SetButtonType(ButtonType t)
{
    iType = t;
    
    // 根据类型加载图片
    if(iLongPressLock==ButtonLock_OFF)
    {
        switch(iType)
        {
        case Button_Up:
            {
                iButtonPic.load("./ButtonPic/unlock/up.png");
            }
            break;
        case Button_Down:
            {
                iButtonPic.load("./ButtonPic/unlock/down.png");
            }
            break;
        case Button_Left:
            {
                iButtonPic.load("./ButtonPic/unlock/left.png");
            }
            break;
        case Button_Right:
            {
                iButtonPic.load("./ButtonPic/unlock/right.png");
            }
            break;
        case Button_A:
            {
                iButtonPic.load("./ButtonPic/unlock/a.png");
            }
            break;
        case Button_B:
            {
                iButtonPic.load("./ButtonPic/unlock/b.png");
            }
            break;
        default:
            break;
        }
    }
    else if(iLongPressLock==ButtonLock_ON)
    {
        switch(iType)
        {
        case Button_Up:
            {
                iButtonPic.load("./ButtonPic/lock/up.png");
            }
            break;
        case Button_Down:
            {
                iButtonPic.load("./ButtonPic/lock/down.png");
            }
            break;
        case Button_Left:
            {
                iButtonPic.load("./ButtonPic/lock/left.png");
            }
            break;
        case Button_Right:
            {
                iButtonPic.load("./ButtonPic/lock/right.png");
            }
            break;
        case Button_A:
            {
                iButtonPic.load("./ButtonPic/lock/a.png");
            }
            break;
        case Button_B:
            {
                iButtonPic.load("./ButtonPic/lock/b.png");
            }
            break;
        default:
            break;
        }
    }
    
    iButtonSize = iButtonPic.size();
}

void TouchDefine_Dialog_Pad_Button::SetButtonLock(ButtonLongPressLock l)
{
    iLongPressLock = l;
    
    update();
}

void TouchDefine_Dialog_Pad_Button::SetButtonStatus(ButtonStatus s)
{
    iStatus = s;
}

void TouchDefine_Dialog_Pad_Button::SetButtonMapto(QString mapto)
{
    iButtonMapto = mapto;
}

void TouchDefine_Dialog_Pad_Button::SetButtonLockTime(int time)
{
    iButtonLockTime = time;
}

void TouchDefine_Dialog_Pad_Button::slotButtonMapto()
{
    TouchDefine_Dialog_Pad_Mapto* dialog = new TouchDefine_Dialog_Pad_Mapto(this);
    dialog->InitMapto(this);
    dialog->exec();
}

void TouchDefine_Dialog_Pad_Button::slotChangeButtonPos()
{
    TouchDefine_Dialog_Pad_Position* pos = new TouchDefine_Dialog_Pad_Position(this);
    pos->InitDialog(((TouchDefine_Dialog_Pad_Screen*)(this->parent()))->NativeSize(), iZoom, iOrg, this);
    pos->show();
}

void TouchDefine_Dialog_Pad_Button::slotButtonLockTime()
{
   iButtonLockTime = QInputDialog::getInt(this, tr("Input Lock Time You Want"), tr("LockTime"), iButtonLockTime, 0, 2000, 100); 
}

void TouchDefine_Dialog_Pad_Button::slotCleanButton()
{
    if(iButtonAdded_list!=NULL)
    {
        for(int i=0; i<iButtonAdded_list->count(); i++)
        {
            if(iButtonAdded_list->at(i)==this)
            {
                iButtonAdded_list->takeAt(i);
                this->close();
            }
        }
    }
}

void TouchDefine_Dialog_Pad_Button::slotButtonLongPressLockOff()
{
    iLongPressLock = ButtonLock_OFF;
    this->SetButtonType(iType);

    update();
}

void TouchDefine_Dialog_Pad_Button::slotButtonLongPressLockOn()
{
    iLongPressLock = ButtonLock_ON;
    this->SetButtonType(iType);
    
    update();
}

void TouchDefine_Dialog_Pad_Button::slotChangeZoom(float zoom)
{
    iZoom = zoom;
    
    QPointF pos;
    if(iOrg==0)
    {
        pos = iButtonPos;
    }
    else if(iOrg==1)
    {
        pos.rx() = ((TouchDefine_Dialog_Pad_Screen*)(this->parent()))->NativeSize().width() - iButtonPos.x() - this->iButtonSize.width();
        pos.ry() = iButtonPos.y();
    }
    else if(iOrg==3)
    {
        pos.rx() = ((TouchDefine_Dialog_Pad_Screen*)(this->parent()))->NativeSize().width() - iButtonPos.x() - this->iButtonSize.width();
        pos.ry() = ((TouchDefine_Dialog_Pad_Screen*)(this->parent()))->NativeSize().height() - iButtonPos.y() - this->iButtonSize.height();
    }
    else if(iOrg==2)
    {
        pos.rx() = iButtonPos.x();
        pos.ry() = ((TouchDefine_Dialog_Pad_Screen*)(this->parent()))->NativeSize().height() - iButtonPos.y() - this->iButtonSize.height();
    }
    
    this->move((pos*iZoom).toPoint());
    
    update();
}

void TouchDefine_Dialog_Pad_Button::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << "TouchDefine_Dialog_Pad_Button::mousePressEvent";
    
    if(ev->button()==Qt::LeftButton)
    {
        if(this->iStatus==ButtonStatus_Wait)
        {
            QPixmap pixmap = this->iButtonPic;
            QByteArray ButtonArray;
            QDataStream datastream(&ButtonArray, QIODevice::WriteOnly);
            int type = iType;
            int status = iStatus;
            int longpresslock = iLongPressLock;
            int org = iOrg;
            datastream << ev->pos() << type << status << iButtonPic << iButtonSize << iButtonPos << iButtonMapto << iButtonLockTime << iZoom << longpresslock << org;
            
            QMimeData* mimeData = new QMimeData;
            mimeData->setData("application/x-buttondata", ButtonArray);
            
            QDrag* drag = new QDrag(this);
            drag->setPixmap(pixmap.scaled(pixmap.size()*iZoom));
            drag->setMimeData(mimeData);
            
            drag->setHotSpot(ev->pos());
            
            drag->exec(Qt::MoveAction);
        }
        else if(this->iStatus==ButtonStatus_Screen)
        {
            if(iButtonAdded_list!=NULL)
            {
                for(int i=0; i<iButtonAdded_list->count(); i++)
                {
                    if(iButtonAdded_list->at(i)==this)
                    {
                        iButtonAdded_list->takeAt(i);
                        break;
                    }
                }
            }
            
            QPixmap pixmap = this->iButtonPic;
            QByteArray ButtonArray;
            QDataStream datastream(&ButtonArray, QIODevice::WriteOnly);
            int type = iType;
            int status = iStatus;
            int longpresslock = iLongPressLock;
            datastream << ev->pos() << type << status << iButtonPic << iButtonSize << iButtonPos
                       << iButtonMapto << iButtonLockTime << iZoom << longpresslock << iOrg;
            
            this->close();
            
            QMimeData* mimeData = new QMimeData;
            mimeData->setData("application/x-buttondata", ButtonArray);
            
            QDrag* drag = new QDrag(this);
            drag->setPixmap(pixmap.scaled(pixmap.size()*iZoom));
            drag->setMimeData(mimeData);
            
            drag->setHotSpot(ev->pos());
            
            drag->exec(Qt::MoveAction);
        }
    }
}

void TouchDefine_Dialog_Pad_Button::paintEvent(QPaintEvent *)
{
    updateButton();
}

void TouchDefine_Dialog_Pad_Button::contextMenuEvent(QContextMenuEvent *ev)
{
    if(iStatus==ButtonStatus_Wait)
    {}
    else if(iStatus==ButtonStatus_Screen)
    {
        QMenu* menu = new QMenu(this);
        QAction* buttonmapto = new QAction(tr("MapTo..."), menu);
        connect(buttonmapto, SIGNAL(triggered()),
                this, SLOT(slotButtonMapto()));
        menu->addAction(buttonmapto);
        
        QAction* buttonpos = new QAction(tr("Position"), menu);
        connect(buttonpos, SIGNAL(triggered()),
                this, SLOT(slotChangeButtonPos()));
        menu->addAction(buttonpos);
    
        QMenu* longpresslock = new QMenu(tr("LongPress"), menu);
        QAction* longpresslockoff = new QAction(tr("OFF"), longpresslock);
        connect(longpresslockoff, SIGNAL(triggered()),
                this, SLOT(slotButtonLongPressLockOff()));
        longpresslockoff->setCheckable(true);
        longpresslock->addAction(longpresslockoff);
        QAction* longpresslockon = new QAction(tr("ON"), longpresslock);
        connect(longpresslockon, SIGNAL(triggered()),
                this, SLOT(slotButtonLongPressLockOn()));
        longpresslockon->setCheckable(true);
        longpresslock->addAction(longpresslockon);
        switch(iLongPressLock)
        {
        case ButtonLock_OFF:
            {
                longpresslockoff->setChecked(true);
                longpresslockon->setChecked(false);
            }
            break;
        case ButtonLock_ON:
            {
                longpresslockoff->setChecked(false);
                longpresslockon->setChecked(true);
            }
            break;
        default:
            break;
        }
        menu->addMenu(longpresslock);
    
        QAction* locktime = new QAction(tr("LockTime"), menu);
        connect(locktime, SIGNAL(triggered()),
                this, SLOT(slotButtonLockTime()));
        if(iLongPressLock==ButtonLock_OFF)
        {
            locktime->setEnabled(false);
        }
        menu->addAction(locktime);
    
        menu->addSeparator();
    
        QAction* cleanbutton = new QAction(tr("Delete"), menu);
        connect(cleanbutton, SIGNAL(triggered()),
                this, SLOT(slotCleanButton()));
        menu->addAction(cleanbutton);
    
        menu->exec(QCursor::pos());
    }
}

void TouchDefine_Dialog_Pad_Button::updateButton()
{
    QRectF ButtonSize;
    
    if(iStatus==ButtonStatus_Wait)
    {
        ButtonSize.setX(0);
        ButtonSize.setY(0);
        ButtonSize.setWidth(iButtonSize.width());
        ButtonSize.setHeight(iButtonSize.height());
    }
    else if(iStatus==ButtonStatus_Screen)
    {
        ButtonSize.setX(0);
        ButtonSize.setY(0);
        ButtonSize.setWidth(iButtonSize.width()*iZoom);
        ButtonSize.setHeight(iButtonSize.height()*iZoom);
    }
    
    this->setFixedSize(ButtonSize.size().toSize());
    
    QPainter painter(this);
    painter.drawPixmap(ButtonSize, iButtonPic, QRectF(QPoint(0,0),iButtonSize));
}