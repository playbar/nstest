#include "TouchDefine_Dialog_Pad_Screen.h"
#include <QPixmap>
#include <QPainter>
#include <QMenu>
#include <QFileDialog>
#include <QDebug>
#include <QDragEnterEvent>
#include "TouchDefine_Dialog_Pad_Button.h"

TouchDefine_Dialog_Pad_Screen::TouchDefine_Dialog_Pad_Screen(QWidget *parent) :
    QLabel(parent)
{
    iBGPos = BGPos_Center;
    iHasGameScreen = false;
    iZoom = 1;
    
    this->setAcceptDrops(true);
    
    iButtonAdded_list = NULL;
}


void TouchDefine_Dialog_Pad_Screen::slotChangeScreenSize(QSizeF ScreenSize)
{
    iScreenSize = ScreenSize;
    update();
}

void TouchDefine_Dialog_Pad_Screen::slotChangeZoom(float zoom)
{
    iZoom = zoom;
    emit signalChangeZoom(zoom);
    update();
}

void TouchDefine_Dialog_Pad_Screen::slotChangeBackground()
{
    QString GameScreenPath = QFileDialog::getOpenFileName(this, tr("Open GameScreen"), "./GameScreen",
                                                          "Images (*.png *.jpg *.jpeg)");

    if(GameScreenPath.length()!=0)
    {
        iGameScreen.load(GameScreenPath);
        iHasGameScreen = true;
        update();
    }
    else
        return;
}

void TouchDefine_Dialog_Pad_Screen::slotChangeBackgroundPos_Center()
{
    iBGPos = BGPos_Center;

    update();
}

void TouchDefine_Dialog_Pad_Screen::slotChangeBackgroundPos_FullScreen()
{
    iBGPos = BGPos_FullScreen;

    update();
}

void TouchDefine_Dialog_Pad_Screen::slotCleanBackground()
{
    iHasGameScreen = false;

    update();
}

void TouchDefine_Dialog_Pad_Screen::slotCleanAllButtons()
{
    emit signalCleanAllButtons();
}

void TouchDefine_Dialog_Pad_Screen::dragEnterEvent(QDragEnterEvent *ev)
{
    if(ev->mimeData()->hasFormat("application/x-buttondata"))
        ev->accept();
    else
        ev->ignore();
}

void TouchDefine_Dialog_Pad_Screen::dragMoveEvent(QDragMoveEvent *ev)
{
    QByteArray buttonarray = ev->mimeData()->data("application/x-buttondata");
    QDataStream datastream(&buttonarray, QIODevice::ReadOnly);
    
    QPoint offset;
    datastream >> offset;
    
    QPoint ButtonPos = ev->pos();
    ButtonPos -= offset;
    
    QStringList pos;
    pos.append(QString().number((int)(ButtonPos.x()/iZoom)));
    pos.append(QString().number((int)(ButtonPos.y()/iZoom)));
    
    emit signalCurButtonPos(pos.join("x"));
    
    ev->accept();
}

void TouchDefine_Dialog_Pad_Screen::dragLeaveEvent(QDragLeaveEvent *ev)
{
    emit signalCurButtonPos("Out!!!");
    
    ev->accept();
}

void TouchDefine_Dialog_Pad_Screen::dropEvent(QDropEvent *ev)
{
    QByteArray buttonarray = ev->mimeData()->data("application/x-buttondata");
    QDataStream datastream(&buttonarray, QIODevice::ReadOnly);
    
    QPoint offset;
    int buttontype;
    int status;
    QPixmap pixmap;
    QSizeF buttonsize;
    QPointF buttonpos;
    QString buttonmapto;
    int buttonlocktime;
    float zoom;
    int longpress;
    int org = 0;
    datastream >> offset >> buttontype >> status >> pixmap >> buttonsize >> buttonpos >> buttonmapto >> buttonlocktime >> zoom >> longpress >> org;

    TouchDefine_Dialog_Pad_Button* button = new TouchDefine_Dialog_Pad_Button(this);
    button->iButtonAdded_list = iButtonAdded_list;  // 将list指针传给button
//    button->iOrg = org;
    button->move(ev->pos()-offset); // 不着急移动，往后放
    switch(longpress)
    {
    case 0:
        button->SetButtonLock(TouchDefine_Dialog_Pad_Button::ButtonLock_OFF);
        break;
    case 1:
        button->SetButtonLock(TouchDefine_Dialog_Pad_Button::ButtonLock_ON);
        break;
    }
    switch(buttontype)
    {
    case 0:
        button->SetButtonType(TouchDefine_Dialog_Pad_Button::Button_Up);
        break;
    case 1:
        button->SetButtonType(TouchDefine_Dialog_Pad_Button::Button_Down);
        break;
    case 2:
        button->SetButtonType(TouchDefine_Dialog_Pad_Button::Button_Left);
        break;
    case 3:
        button->SetButtonType(TouchDefine_Dialog_Pad_Button::Button_Right);
        break;
    case 4:
        button->SetButtonType(TouchDefine_Dialog_Pad_Button::Button_A);
        break;
    case 5:
        button->SetButtonType(TouchDefine_Dialog_Pad_Button::Button_B);
        break;
    }
    
    button->SetButtonStatus(TouchDefine_Dialog_Pad_Button::ButtonStatus_Screen);
    button->SetButtonMapto(buttonmapto);
    button->SetButtonLockTime(buttonlocktime);
//    QPointF newpos;
//    switch(button->iOrg)
//    {
//    case 1:
//        {
//            newpos = button->pos()/iZoom;
//        }
//        break;
//    case 2:
//        {
//            newpos.rx() = iScreenSize.width() - button->pos().x()/iZoom - button->iButtonSize.width();
//            newpos.ry() = button->pos().y()/iZoom;
//        }
//        break;
//    case 3:
//        {
//            newpos.rx() = iScreenSize.width() - button->pos().x()/iZoom - button->iButtonSize.width();
//            newpos.ry() = iScreenSize.height() - button->pos().y()/iZoom - button->iButtonSize.height();
//        }
//        break;
//    case 4:
//        {
//            newpos.rx() = button->pos().x()/iZoom;
//            newpos.ry() = iScreenSize.height() - button->pos().y()/iZoom - button->iButtonSize.height();
//        }
//        break;
//    }
//    button->move();
    button->iButtonPos = QPointF(button->pos()/iZoom);
//    button->iButtonPos = newpos;
    button->slotChangeZoom(zoom);
    connect(this, SIGNAL(signalChangeZoom(float)),
            button, SLOT(slotChangeZoom(float)));
    
    if(iButtonAdded_list!=NULL)
        iButtonAdded_list->append(button);
    
    button->show();
    
    ev->accept();
}

void TouchDefine_Dialog_Pad_Screen::updateBackground()
{
    QPixmap pixmap(QSize(iScreenSize.width()*iZoom, iScreenSize.height()*iZoom));
    pixmap.fill(Qt::black);
    this->resize(pixmap.size());
    QPainter painter(this);
    painter.drawPixmap(pixmap.rect(), pixmap, pixmap.rect());
}

void TouchDefine_Dialog_Pad_Screen::updateGameScreen()
{
    if(iHasGameScreen)
    {
        QRectF GameScreenSize;

        switch(iBGPos)
        {
        case BGPos_Center:
            {
                // 1.GameScreen的宽高均小于屏幕的宽高
                if(iGameScreen.width()<iScreenSize.width() && iGameScreen.height()<iScreenSize.height())
                {
                    GameScreenSize.setX(((iScreenSize.width()-iGameScreen.width())/2.0)*iZoom);
                    GameScreenSize.setY(((iScreenSize.height()-iGameScreen.height())/2.0)*iZoom);
                    GameScreenSize.setWidth(iGameScreen.width()*iZoom);
                    GameScreenSize.setHeight(iGameScreen.height()*iZoom);
                }
                // 2.GameScreen的宽大于等于屏幕的宽，而高小与屏幕的高
                else if(iGameScreen.width()>=iScreenSize.width() && iGameScreen.height()<iScreenSize.height())
                {
                    GameScreenSize.setX(0);
                    GameScreenSize.setY(((iScreenSize.height()-(iScreenSize.width()*iGameScreen.height()/iGameScreen.width()))/2.0)*iZoom);
                    GameScreenSize.setWidth(iScreenSize.width()*iZoom);
                    GameScreenSize.setHeight(iScreenSize.width()*iGameScreen.height()*iZoom/iGameScreen.width());
                }
                // 3.GameScreen的宽小于屏幕的宽，而高大于等于屏幕的高
                else if(iGameScreen.width()<iScreenSize.width() && iGameScreen.height()>=iScreenSize.height())
                {
                    GameScreenSize.setX(((iScreenSize.width()-(iScreenSize.height()*iGameScreen.width()/iGameScreen.height()))/2.0)*iZoom);
                    GameScreenSize.setY(0);
                    GameScreenSize.setWidth(iScreenSize.height()*iGameScreen.width()*iZoom/iGameScreen.height());
                    GameScreenSize.setHeight(iScreenSize.height()*iZoom);
                }
                // 4.GameScreen的宽大于等于屏幕的宽，而高大于等于屏幕的高
                else if(iGameScreen.width()>=iScreenSize.width() && iGameScreen.height()>=iScreenSize.height())
                {
                    if((float)(iGameScreen.width()/(float)iScreenSize.width()) < (float)(iGameScreen.height()/(float)iScreenSize.height()))
                    {
                        GameScreenSize.setX(((iScreenSize.width()-(iScreenSize.height()*iGameScreen.width()/iGameScreen.height()))/2.0)*iZoom);
                        GameScreenSize.setY(0);
                        GameScreenSize.setWidth(iScreenSize.height()*iGameScreen.width()*iZoom/iGameScreen.height());
                        GameScreenSize.setHeight(iScreenSize.height()*iZoom);
                    }
                    else
                    {
                        GameScreenSize.setX(0);
                        GameScreenSize.setY(((iScreenSize.height()-(iScreenSize.width()*iGameScreen.height()/iGameScreen.width()))/2.0)*iZoom);
                        GameScreenSize.setWidth(iScreenSize.width()*iZoom);
                        GameScreenSize.setHeight(iScreenSize.width()*iGameScreen.height()*iZoom/iGameScreen.width());
                    }
                }
            }
            break;
        case BGPos_FullScreen:
            {
                GameScreenSize.setX(0);
                GameScreenSize.setY(0);
                GameScreenSize.setWidth(iScreenSize.width()*iZoom);
                GameScreenSize.setHeight(iScreenSize.height()*iZoom);
            }
            break;
        default:
            break;
        }

        QPainter painter(this);
        painter.drawPixmap(GameScreenSize, iGameScreen, iGameScreen.rect());
    }
    else
        return;
}

void TouchDefine_Dialog_Pad_Screen::contextMenuEvent(QContextMenuEvent *ev)
{
    QMenu* menu = new QMenu(this);
    QAction* changebackground = new QAction(tr("Change BG"), menu);
    connect(changebackground, SIGNAL(triggered()),
            this, SLOT(slotChangeBackground()));
    menu->addAction(changebackground);

    QMenu* changebackgroundPosition = new QMenu(tr("Change BG Pos"), menu);
    QAction* changebackgroundPosition_center = new QAction(tr("Center"), changebackgroundPosition);
    changebackgroundPosition_center->setCheckable(true);
    connect(changebackgroundPosition_center, SIGNAL(triggered()),
            this, SLOT(slotChangeBackgroundPos_Center()));
    changebackgroundPosition->addAction(changebackgroundPosition_center);
    QAction* changebackgroundPosition_fullscreen = new QAction(tr("FullScreen"), changebackgroundPosition);
    changebackgroundPosition_fullscreen->setCheckable(true);
    connect(changebackgroundPosition_fullscreen, SIGNAL(triggered()),
            this, SLOT(slotChangeBackgroundPos_FullScreen()));
    changebackgroundPosition->addAction(changebackgroundPosition_fullscreen);

    switch(iBGPos)
    {
    case BGPos_Center:
        {
            changebackgroundPosition_center->setChecked(true);
            changebackgroundPosition_fullscreen->setChecked(false);
        }
        break;
    case BGPos_FullScreen:
        {
            changebackgroundPosition_center->setChecked(false);
            changebackgroundPosition_fullscreen->setChecked(true);
        }
        break;
    default:
        break;
    }
    menu->addMenu(changebackgroundPosition);

    QAction* cleanbackground = new QAction(tr("Clean BG"), menu);
    connect(cleanbackground, SIGNAL(triggered()),
            this, SLOT(slotCleanBackground()));
    menu->addAction(cleanbackground);

//    menu->addSeparator();

//    QAction* cleanallbuttons = new QAction(tr("Clean All Buttons"), menu);
//    connect(cleanallbuttons, SIGNAL(triggered()),
//            this, SLOT(slotCleanAllButtons()));
//    menu->addAction(cleanallbuttons);

    menu->exec(QCursor::pos());
}

void TouchDefine_Dialog_Pad_Screen::paintEvent(QPaintEvent *)
{
    updateBackground();
    updateGameScreen();
}
