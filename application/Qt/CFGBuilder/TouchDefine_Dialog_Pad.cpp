#include "TouchDefine_Dialog_Pad.h"
#include <QFile>
#include <QPainter>
#include "TouchDefine_Dialog_Pad_Screen.h"

TouchDefine_Dialog_Pad::TouchDefine_Dialog_Pad(QWidget *parent) :
    QLabel(parent)
{
    iModelPicFlag = false;
    
    iScreen = new TouchDefine_Dialog_Pad_Screen(this);
    connect(this, SIGNAL(signalChangeScreenSize(QSizeF)),
            iScreen, SLOT(slotChangeScreenSize(QSizeF)));
    connect(this, SIGNAL(signalChangeZoom(float)),
            iScreen, SLOT(slotChangeZoom(float)));
    connect(iScreen, SIGNAL(signalCurButtonPos(QString)),
            this, SLOT(slotCurButtonPos(QString)));
    
}

void TouchDefine_Dialog_Pad::AddButtonListP(QList<TouchDefine_Dialog_Pad_Button *> *p)
{
    if(iScreen!=NULL)
        iScreen->iButtonAdded_list = p;
}


void TouchDefine_Dialog_Pad::slotChangeModel(ModelInfo model)
{
    iModelInfo.ModelName = model.ModelName;
    iModelInfo.ModelPicName = model.ModelPicName;
    iModelInfo.ModelSize = model.ModelSize;
    iModelInfo.ModelScreenSize = model.ModelScreenSize;
    iModelInfo.ModelScreenOffset = model.ModelScreenOffset;
    iModelInfo.ModelZoom = model.ModelZoom;
    iModelInfo.eModelDir = model.eModelDir;
    
    QString path = "./ModelPic/";
    path.append(iModelInfo.ModelPicName);
    QFile file(path);
    if(file.exists())
    {
        iModelPic.load(path);
        iModelPicFlag = true;
    }
    else
    {
        iModelPicFlag = false;
    }

    emit signalChangeScreenSize(iModelInfo.ModelScreenSize);
    
    // 刷新
    update();
}

void TouchDefine_Dialog_Pad::slotChangeZoom(float zoom)
{
    iModelInfo.ModelZoom = zoom;
    
    emit signalChangeZoom(zoom);
    
    update();
}

void TouchDefine_Dialog_Pad::slotCurButtonPos(QString pos)
{
    emit signalCurButtonPos(pos);
}

void TouchDefine_Dialog_Pad::paintEvent(QPaintEvent *)
{
    updateBackground();
}

void TouchDefine_Dialog_Pad::updateBackground()
{
    if(iModelPicFlag)
    {
        if(iScreen->isHidden())
            iScreen->show();
        
        // 旋转后的手机图片分辨率
        QSizeF NewModelPicSize = (iModelInfo.eModelDir==Dir_Normal ||iModelInfo.eModelDir==Dir_CW180)?
                                 iModelInfo.ModelSize : QSizeF(iModelInfo.ModelSize.height(), iModelInfo.ModelSize.width());
        // 旋转后的手机屏幕分辨率
        QSizeF NewModelScreenSize = (iModelInfo.eModelDir==Dir_Normal ||iModelInfo.eModelDir==Dir_CW180)?
                                    iModelInfo.ModelScreenSize : QSizeF(iModelInfo.ModelScreenSize.height(), iModelInfo.ModelScreenSize.width());
        // 旋转后的屏幕偏移量
        QSizeF NewModelScreenOffset = (iModelInfo.eModelDir==Dir_Normal ||iModelInfo.eModelDir==Dir_CW180)?
                                      iModelInfo.ModelScreenOffset : QSizeF(iModelInfo.ModelScreenOffset.height(), iModelInfo.ModelScreenOffset.width());

        QPainter painter(this);
        switch(iModelInfo.eModelDir)
        {
            // 无需对原图片进行调整
        case Dir_Normal:
            {
            }
            break;
        case Dir_CW90:
            {
                painter.translate(iModelPic.width()*iModelInfo.ModelZoom, iModelPic.height()*iModelInfo.ModelZoom/2.0);
                painter.rotate(90);
                painter.translate(iModelPic.width()*iModelInfo.ModelZoom, iModelPic.height()*iModelInfo.ModelZoom/2.0);
            }
            break;
        case Dir_CW180:
            {
                painter.translate(iModelPic.width()*iModelInfo.ModelZoom/2.0, iModelPic.height()*iModelInfo.ModelZoom/2.0);
                painter.rotate(180);
                painter.translate(-iModelPic.width()*iModelInfo.ModelZoom/2.0, -iModelPic.height()*iModelInfo.ModelZoom/2.0);
            }
            break;
        case Dir_ACW90:
            {
                painter.translate(iModelPic.width()*iModelInfo.ModelZoom/2.0, iModelPic.width()*iModelInfo.ModelZoom/2.0);
                painter.rotate(-90);
                painter.translate(-iModelPic.width()*iModelInfo.ModelZoom/2.0, -iModelPic.width()*iModelInfo.ModelZoom/2.0);
            }
            break;
        default:
            break;
        }

        painter.drawPixmap(0,0,iModelPic.scaled(iModelPic.width()*iModelInfo.ModelZoom, iModelPic.height()*iModelInfo.ModelZoom));
        this->setFixedSize(QSize(NewModelPicSize.width()*iModelInfo.ModelZoom, NewModelPicSize.height()*iModelInfo.ModelZoom));
        emit signalChangeScreenSize(NewModelScreenSize);
        emit signalChangeZoom(iModelInfo.ModelZoom);
        iScreen->move(QPoint(NewModelScreenOffset.width()*iModelInfo.ModelZoom, NewModelScreenOffset.height()*iModelInfo.ModelZoom));
    }
    else
    {
        // 隐藏手机屏幕
        if(!iScreen->isHidden())
            iScreen->hide();
    }
}
