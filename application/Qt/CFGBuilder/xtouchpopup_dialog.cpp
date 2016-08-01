#include "xtouchpopup_dialog.h"
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>
#include "xtouchpopup_dialog_pad.h"
#include <QMouseEvent>
#include <QGridLayout>
#include "xtouchpopup_dialog_button.h"
#include <QPixmap>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QPainter>
#include <QDialogButtonBox>
#include <QPushButton>
#include "xaction.h"
#include "xtouchpopup_dialog_position.h"
#include "xtouchpopup_dialog_longpress.h"
#include "tabsecond_groupfive_new.h"
#include "TabSecond_GroupFive_Item.h"
#include "xtouchpopup_dialog_mapto.h"

XTouchPopup_Dialog::XTouchPopup_Dialog(QWidget *parent) :
    QDialog(parent)
{
    iParent = NULL;
    iItem = NULL;

    iLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    iSubLayout4 = new QBoxLayout(QBoxLayout::LeftToRight);
    this->setAcceptDrops(true);

    CreateDialog();
    this->setLayout(iLayout);
}

void XTouchPopup_Dialog::ModifyItem_Item(QListWidget *parent, QListWidgetItem *item)
{
    iName_input->clear();
    for(int j=0; j<iButtonAdd.count(); j++)
    {
        iButtonAdd.at(j)->close();
    }
    iButtonAdd.clear();

    iParent = parent;
    iItem = item;

    if(iItem!=NULL)
    {
        iName_input->setText(((TabSecond_GroupFive_New*)iItem)->iItemName);
        for(int i=0; i<((TabSecond_GroupFive_New*)iItem)->iItemList.count(); i++)
        {
            XTouchPopup_Dialog_Button* button = new XTouchPopup_Dialog_Button(this);
            button->iButtonadd = ((TabSecond_GroupFive_New*)iItem)->iItemList.at(i)->Buttonadd();
            button->iButtonmapto =  ((TabSecond_GroupFive_New*)iItem)->iItemList.at(i)->Buttonmapto();
            button->iButtonLongpress =  ((TabSecond_GroupFive_New*)iItem)->iItemList.at(i)->ButtonLongpress();
            button->iButtonLocktime =  ((TabSecond_GroupFive_New*)iItem)->iItemList.at(i)->ButtonLocktime();
            button->iButtonPoint.setX( ((TabSecond_GroupFive_New*)iItem)->iItemList.at(i)->Buttonhorpos().toInt());
            button->iButtonPoint.setY( ((TabSecond_GroupFive_New*)iItem)->iItemList.at(i)->Buttonverpos().toInt());
            QString pixmap;
            if(button->iButtonLongpress=="true")
                pixmap += ":/image/logo/longlock/";
            else
                pixmap += ":/image/logo/unlonglock/";

            pixmap += button->iButtonadd;
            pixmap += ".png";
            button->setPixmap(QPixmap(pixmap));
            button->move(button->iButtonPoint+iModelScreenRect.at(iModelNum).topLeft());
            button->show();
            iButtonAdd.append(button);
        }
    }
    else
    {
        iName_input->clear();
    }

    this->exec();
}

void XTouchPopup_Dialog::ListenZoomChange(const QString& zoom)
{
    if(zoom=="100%")
        iZoomCount = 1.0;
    else if(zoom==" 75%")
        iZoomCount = 0.75;
    else if(zoom==" 50%")
        iZoomCount = 0.5;
    else
        iZoomCount = 1.0;
}

void XTouchPopup_Dialog::ClickChange()
{}

void XTouchPopup_Dialog::ClickClean()
{}

void XTouchPopup_Dialog::ClickCleanButtons()
{
    for(int i=0; i<iButtonAdd.count();i++)
    {
        iButtonAdd.at(i)->close();
    }
    iButtonAdd.clear();
}

void XTouchPopup_Dialog::ClickMapto()
{
    XTouchPopup_Dialog_Mapto* mapto = new XTouchPopup_Dialog_Mapto(this);
    mapto->InitMapto(iButtonAdd.at(iButtonat));
    mapto->exec();
}

void XTouchPopup_Dialog::ClickPosition()
{
    XTouchPopup_Dialog_Position* button = new XTouchPopup_Dialog_Position(this);
    button->InitPosition(iModelScreenRect.at(iModelNum), iButtonAdd.at(iButtonat));
    button->exec();
}

void XTouchPopup_Dialog::ClickLongPress()
{
    XTouchPopup_Dialog_LongPress* longpress = new XTouchPopup_Dialog_LongPress(this);
    longpress->Init(iButtonAdd.at(iButtonat));
    longpress->exec();
}

void XTouchPopup_Dialog::ClickDelete()
{
    if(iButtonat!=-1)
    {
        XTouchPopup_Dialog_Button* button = iButtonAdd.takeAt(iButtonat);
        button->close();
    }
}

void XTouchPopup_Dialog::OKClicked()
{
    if(iParent!=NULL)
    {
        if(iItem!=NULL)
        {
            if(iName_input->text().length()==0)
                return;

            ((TabSecond_GroupFive_New*)iItem)->iItemName = iName_input->text();
            ((TabSecond_GroupFive_New*)iItem)->iItemList.clear();

            for(int i=0; i<iButtonAdd.count(); i++)
            {
                TabSecond_GroupFive_Item* item = new TabSecond_GroupFive_Item;
                item->SetButtonadd(iButtonAdd.at(i)->iButtonadd);
                item->SetButtonmapto(iButtonAdd.at(i)->iButtonmapto);
                item->SetButtonhorpos(QString::number(iButtonAdd.at(i)->iButtonPoint.x()));
                item->SetButtonverpos(QString::number(iButtonAdd.at(i)->iButtonPoint.y()));
                item->SetButtonLongpress(iButtonAdd.at(i)->iButtonLongpress);
                item->SetButtonLocktime(iButtonAdd.at(i)->iButtonLocktime);
                ((TabSecond_GroupFive_New*)iItem)->iItemList.append(item);
            }

            iItem->setText(iName_input->text());
        }
        else
        {
            if(iName_input->text().length()==0)
                return;

            iItem = new TabSecond_GroupFive_New;
            ((TabSecond_GroupFive_New*)iItem)->iItemName = iName_input->text();
            for(int i=0; i<iButtonAdd.count(); i++)
            {
                TabSecond_GroupFive_Item* childitem = new TabSecond_GroupFive_Item;
                childitem->SetButtonadd(iButtonAdd.at(i)->iButtonadd);
                childitem->SetButtonmapto(iButtonAdd.at(i)->iButtonmapto);
                childitem->SetButtonhorpos(QString::number(iButtonAdd.at(i)->iButtonPoint.x()));
                childitem->SetButtonverpos(QString::number(iButtonAdd.at(i)->iButtonPoint.y()));
                if(iButtonAdd.at(i)->iButtonLongpress=="true")
                {
                    childitem->SetButtonLongpress(iButtonAdd.at(i)->iButtonLongpress);
                    childitem->SetButtonLocktime(iButtonAdd.at(i)->iButtonLocktime);
                }
                else
                {
                    childitem->SetButtonLongpress(iButtonAdd.at(i)->iButtonLongpress);
                }
                ((TabSecond_GroupFive_New*)iItem)->iItemList.append(childitem);
            }

            iItem->setText(iName_input->text());

            iParent->addItem(iItem);
        }

        this->close();
    }
}

void XTouchPopup_Dialog::CreateDialog()
{
    InitData();
    iSubLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    CreateName();
    CreateModel();
    CreateZoom();

    CreatePad();

    CreateSpinButton();
    CreateButton();
    CreateButtonBox();

    InitModel();
    InitZoom();
}

void XTouchPopup_Dialog::CreateName()
{
    iSubLayout1 = new QHBoxLayout;

    iName = new QLabel(this);
    iName->setText(tr("Name"));

    iName_input = new QLineEdit(this);
    iName_input->setFixedWidth(300);

    iSubLayout1->addWidget(iName);
    iSubLayout1->addWidget(iName_input);
}

void XTouchPopup_Dialog::CreateModel()
{
    iModel = new QLabel(this);
    iModel->setText(tr("Model"));

    iModel_list = new QComboBox(this);
    iModel_list->setEnabled(false);
    QObject::connect(iModel_list, SIGNAL(currentIndexChanged(QString)),
                     this, SLOT(ListenModelChange(QString)));
    QObject::connect(iModel_list, SIGNAL(currentIndexChanged(int)),
                     this, SLOT(ListenModelChange(int)));

    iSubLayout1->addWidget(iModel);
    iSubLayout1->addWidget(iModel_list);
}

void XTouchPopup_Dialog::CreateZoom()
{
    iZoom = new QLabel(this);
    iZoom->setText(tr("Zoom"));

    iZoom_list = new QComboBox(this);
    iZoom_list->setEnabled(false);
    QObject::connect(iZoom_list, SIGNAL(currentIndexChanged(QString)),
                     this, SLOT(ListenZoomChange(QString)));

    iSubLayout1->addWidget(iZoom);
    iSubLayout1->addWidget(iZoom_list);

    iSubLayout->addLayout(iSubLayout1);
}

void XTouchPopup_Dialog::InitModel()
{
    // 读取Model.txt文件，并添加到iModel_list中
    QFile file("Model.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream readstream(&file);
        while(!readstream.atEnd())
        {
            QString model = readstream.readLine();
            if(model.length()!=0)
            {
                iModel_list->addItem(model);
            }
        }
        if(iModel_list->count()>0)
        {
            iModel_list->setCurrentIndex(0);
        }

        file.close();
    }
    else
    {
        iModel_list->addItem(tr("NoModel"));
        iModel_list->setEnabled(false);
    }
}

void XTouchPopup_Dialog::InitZoom()
{
    iZoom_list->addItem(tr("100%"));
    iZoom_list->addItem(tr(" 75%"));
    iZoom_list->addItem(tr(" 50%"));
    // 通知其他widget缩放比例改变
    emit ZoomChanged(1.0);
}

void XTouchPopup_Dialog::CreatePad()
{
    iPad = new XTouchPopup_Dialog_Pad(this);
    iPad->resize(QSize(100, 100));
    QObject::connect(this, SIGNAL(ModelChanged(QString)),
                     iPad, SLOT(LoadModelPic(QString)));

    iSubLayout->addWidget(iPad);
}

void XTouchPopup_Dialog::CreateSpinButton()
{
    iSubLayout2 = new QHBoxLayout;

    iClockwiseButton = new QLabel(this);
    iClockwiseButton->setPixmap(QPixmap(":/Spin/logo/Clockwise.png"));
    iClockwiseButton->resize(iClockwiseButton->pixmap()->size());

    iEasternButton = new QLabel(this);
    iEasternButton->setPixmap(QPixmap(":/Spin/logo/Eastern.png"));
    iEasternButton->resize(iEasternButton->pixmap()->size());

    iSubLayout2->addWidget(iClockwiseButton);
    iSubLayout2->itemAt(0)->setAlignment(Qt::AlignLeft);
    iSubLayout2->addWidget(iEasternButton);
    iSubLayout2->itemAt(1)->setAlignment(Qt::AlignRight);

    iSubLayout->addLayout(iSubLayout2);

    iSubLayout4->addLayout(iSubLayout);
//    iLayout->addLayout(iSubLayout);
}

void XTouchPopup_Dialog::CreateButton()
{
    iSubLayout3 = new QGridLayout;
    iSubLayout3->setColumnStretch(2, 3);

    XTouchPopup_Dialog_Button* buttonup = new XTouchPopup_Dialog_Button;
    buttonup->setPixmap(QPixmap(":/image/logo/unlonglock/up.png"));
    buttonup->resize(buttonup->pixmap()->size());
    buttonup->setMinimumSize(buttonup->pixmap()->size());
    buttonup->setMaximumSize(buttonup->pixmap()->size());
    buttonup->iType = XTouchPopup_Dialog_Button::Button_UP;
    buttonup->iButtonadd = "up";
    iButtonBox.append(buttonup);
    iSubLayout3->addWidget(buttonup, 0, 0);

    XTouchPopup_Dialog_Button* buttondown = new XTouchPopup_Dialog_Button;
    buttondown->setPixmap(QPixmap(":/image/logo/unlonglock/down.png"));
    buttondown->resize(buttondown->pixmap()->size());
    buttondown->setMinimumSize(buttondown->pixmap()->size());
    buttondown->setMaximumSize(buttondown->pixmap()->size());
    buttondown->iType = XTouchPopup_Dialog_Button::Button_DOWN;
    buttondown->iButtonadd = "down";
    iButtonBox.append(buttondown);
    iSubLayout3->addWidget(buttondown, 0, 1);

    XTouchPopup_Dialog_Button* buttonleft = new XTouchPopup_Dialog_Button;
    buttonleft->setPixmap(QPixmap(":/image/logo/unlonglock/left.png"));
//    buttonleft->setPixmap(QPixmap(":/image/logo/xiaologo.gif"));
    buttonleft->resize(buttonleft->pixmap()->size());
    buttonleft->setMinimumSize(buttonleft->pixmap()->size());
    buttonleft->setMaximumSize(buttonleft->pixmap()->size());
    buttonleft->iType = XTouchPopup_Dialog_Button::Button_LEFT;
    buttonleft->iButtonadd = "left";
    iButtonBox.append(buttonleft);
    iSubLayout3->addWidget(buttonleft, 1, 0);

    XTouchPopup_Dialog_Button* buttonright = new XTouchPopup_Dialog_Button;
    buttonright->setPixmap(QPixmap(":/image/logo/unlonglock/right.png"));
    buttonright->resize(buttonright->pixmap()->size());
    buttonright->setMinimumSize(buttonright->pixmap()->size());
    buttonright->setMaximumSize(buttonright->pixmap()->size());
    buttonright->iType = XTouchPopup_Dialog_Button::Button_RIGHT;
    buttonright->iButtonadd = "right";
    iButtonBox.append(buttonright);
    iSubLayout3->addWidget(buttonright, 1, 1);

    XTouchPopup_Dialog_Button* buttona = new XTouchPopup_Dialog_Button;
    buttona->setPixmap(QPixmap(":/image/logo/unlonglock/a.png"));
    buttona->resize(buttona->pixmap()->size());
    buttona->setMinimumSize(buttona->pixmap()->size());
    buttona->setMaximumSize(buttona->pixmap()->size());
    buttona->iType = XTouchPopup_Dialog_Button::Button_A;
    buttona->iButtonadd = "a";
    iButtonBox.append(buttona);
    iSubLayout3->addWidget(buttona, 2, 0);

    XTouchPopup_Dialog_Button* buttonb = new XTouchPopup_Dialog_Button;
    buttonb->setPixmap(QPixmap(":/image/logo/unlonglock/b.png"));
    buttonb->resize(buttonb->pixmap()->size());
    buttonb->setMinimumSize(buttonb->pixmap()->size());
    buttonb->setMaximumSize(buttonb->pixmap()->size());
    buttonb->iType = XTouchPopup_Dialog_Button::Button_B;
    buttonb->iButtonadd = "b";
    iButtonBox.append(buttonb);
    iSubLayout3->addWidget(buttonb, 2, 1);

    iSubLayout4->addLayout(iSubLayout3);
//    iLayout->addLayout(iSubLayout3);
    iLayout->addLayout(iSubLayout4);
}

void XTouchPopup_Dialog::CreateButtonBox()
{
    iButton = new QDialogButtonBox;
    iOk = new QPushButton;
    iOk->setText(tr("OK"));
    QObject::connect(iOk, SIGNAL(clicked()),
                     this, SLOT(OKClicked()));
    iButton->addButton(iOk, QDialogButtonBox::ActionRole);

    iCancel =new QPushButton;
    iCancel->setText(tr("Cancel"));
    QObject::connect(iCancel, SIGNAL(clicked()),
                     this, SLOT(close()));
    iButton->addButton(iCancel, QDialogButtonBox::ActionRole);

    iLayout->addWidget(iButton);

}

void XTouchPopup_Dialog::InitData()
{
    iModelNum = -1;

    // 机型名称
    QFile file("Model.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream read(&file);
        while(!read.atEnd())
        {
            QString name = read.readLine();
            iModelName.append(name);
        }
        file.close();
    }

    // 手机的尺寸
    file.setFileName("ModelPicSize.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream read(&file);
        while(!read.atEnd())
        {
            QString sizestr = read.readLine();
            QStringList sizelist = sizestr.split(",");
            QSize size;
            size.setWidth(sizelist.at(0).toInt());
            size.setHeight(sizelist.at(1).toInt());
            iModelPicSize.append(size);
        }

        file.close();
    }

     // 屏幕的偏移
    file.setFileName("ModelOffsetSize.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream read(&file);
        while(!read.atEnd())
        {
            QString sizestr = read.readLine();
            QStringList sizelist = sizestr.split(",");
            QSize size;
            size.setWidth(sizelist.at(0).toInt());
            size.setHeight(sizelist.at(1).toInt());
            iModelOffsetSize.append(size);
        }

        file.close();
    }

    // 屏幕的尺寸及位置
    for(int i=0;i<iModelName.count();i++)
    {
        QRect rect;
        QPoint point;
        point.setX(iModelOffsetSize.at(i).width());
        point.setY(iModelOffsetSize.at(i).height());
        int width = iModelPicSize.at(i).width()-2*iModelOffsetSize.at(i).width();
        int height = iModelPicSize.at(i).height()-2*iModelOffsetSize.at(i).height();
        rect.setTopLeft(point);
        rect.setWidth(width);
        rect.setHeight(height);
        iModelScreenRect.append(rect);
    }
}

void XTouchPopup_Dialog::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton)
    {
        XTouchPopup_Dialog_Button* button = static_cast<XTouchPopup_Dialog_Button*>(childAt(ev->pos()));
        if(iButtonBox.contains(button))
        {
            QPixmap pixmap = *button->pixmap();
            QByteArray itemArray;
            QDataStream dataStream(&itemArray, QIODevice::WriteOnly);
            dataStream << pixmap << QPoint(ev->pos()-button->pos()) << button->iButtonadd << button->iButtonmapto
                    << button->iButtonLongpress << button->iButtonLocktime;

            QMimeData* mimeData = new QMimeData;
            mimeData->setData("application/x-buttondata", itemArray);

            QDrag* drag = new QDrag(this);
            drag->setMimeData(mimeData);
            drag->setPixmap(pixmap);
            drag->setHotSpot(ev->pos()-button->pos());

            drag->exec(Qt::MoveAction);
        }
        else if(iButtonAdd.contains(button))
        {
            QPixmap pixmap = *button->pixmap();
            QByteArray itemArray;
            QDataStream dataStream(&itemArray, QIODevice::WriteOnly);
            dataStream << pixmap << QPoint(ev->pos()-button->pos()) << button->iButtonadd << button->iButtonmapto
                    << button->iButtonLongpress <<  button->iButtonLocktime;

            QMimeData* mimeData = new QMimeData;
            mimeData->setData("application/x-buttondata", itemArray);

            QDrag* drag = new QDrag(this);
            drag->setMimeData(mimeData);
            drag->setPixmap(pixmap);
            drag->setHotSpot(ev->pos()-button->pos());

            drag->exec(Qt::MoveAction);
            int at = iButtonAdd.indexOf(button);
            if(at!=-1)
                iButtonAdd.takeAt(at);
            button->close();
        }
    }
}

void XTouchPopup_Dialog::mouseDoubleClickEvent(QMouseEvent *ev)
{
//    QString msg;
//    msg += QString::number(ev->pos().x());
//    msg += ",";
//    msg += QString::number(ev->pos().y());
//    QMessageBox::information(this, tr("Position"), msg);
}

void XTouchPopup_Dialog::contextMenuEvent(QContextMenuEvent *ev)
{
    QRect rect;
    rect.setTopLeft(iModelScreenRect.at(iModelNum).topLeft()+iPad->pos());
    rect.setWidth(iModelScreenRect.at(iModelNum).width());
    rect.setHeight(iModelScreenRect.at(iModelNum).height());

    if(rect.contains(ev->pos()))
    {
        XTouchPopup_Dialog_Button* button = static_cast<XTouchPopup_Dialog_Button*>(childAt(ev->pos()));
        if(iButtonAdd.contains(button))
        {
            iButtonat = iButtonAdd.indexOf(button);
            QMenu* menu = new QMenu;
            QAction* Mapto = new QAction(menu);
            Mapto->setText(tr("Mapto"));
            QObject::connect(Mapto, SIGNAL(triggered()),
                             this, SLOT(ClickMapto()));
            menu->addAction(Mapto);

            QAction* Position = new QAction(menu);
//            Position->setEnabled(false);
            Position->setText(tr("Position"));
            QObject::connect(Position, SIGNAL(triggered()),
                             this, SLOT(ClickPosition()));
            menu->addAction(Position);

            QAction* LongPress = new QAction(menu);
            LongPress->setText(tr("LongPress"));
            QObject::connect(LongPress, SIGNAL(triggered()),
                             this, SLOT(ClickLongPress()));
            menu->addAction(LongPress);

            XAction* Delete = new XAction(menu);
            Delete->setText(tr("Delete"));
            QObject::connect(Delete, SIGNAL(triggered()),
                             this, SLOT(ClickDelete()));
            menu->addAction(Delete);

            menu->exec(QCursor::pos());
        }
        else
        {
            QMenu* menu = new QMenu;
            QAction* ChangeGameBackground = new QAction(menu);
            ChangeGameBackground->setText(tr("Change"));
            ChangeGameBackground->setEnabled(false);
            QObject::connect(ChangeGameBackground, SIGNAL(triggered()),
                             this, SLOT(ClickChange()));
            menu->addAction(ChangeGameBackground);

            QAction* CleanGameBackground = new QAction(menu);
            CleanGameBackground->setText(tr("Clean"));
            CleanGameBackground->setEnabled(false);
            QObject::connect(CleanGameBackground, SIGNAL(triggered()),
                             this, SLOT(ClickClean()));
            menu->addAction(CleanGameBackground);

            QAction* CleanAllButtons = new QAction(menu);
            CleanAllButtons->setText(tr("Clean Buttons"));
            QObject::connect(CleanAllButtons, SIGNAL(triggered()),
                             this, SLOT(ClickCleanButtons()));
            menu->addAction(CleanAllButtons);

            menu->exec(QCursor::pos());
        }
    }
}

void XTouchPopup_Dialog::dragEnterEvent(QDragEnterEvent *ev)
{
    if(ev->mimeData()->hasFormat("application/x-buttondata"))
        ev->accept();
    else
        ev->ignore();
}

//void XTouchPopup_Dialog::dragMoveEvent(QDragMoveEvent *ev)
//{
////    qDebug() << "Drag Move";
//    qDebug() << ev->pos();
//}

//void XTouchPopup_Dialog::dragLeaveEvent(QDragLeaveEvent *ev)
//{
//    qDebug() << "Drag Level";
//}

void XTouchPopup_Dialog::dropEvent(QDropEvent *ev)
{
    if(ev->mimeData()->hasFormat("application/x-buttondata"))
    {
        // 获取手机屏幕的位置及大小
        QRect rect;
        rect.setTopLeft(iModelScreenRect.at(iModelNum).topLeft()+iPad->pos());
        rect.setWidth(iModelScreenRect.at(iModelNum).width());
        rect.setHeight(iModelScreenRect.at(iModelNum).height());
        //

        QByteArray itemArray = ev->mimeData()->data("application/x-buttondata");
        QDataStream dataStream(&itemArray, QIODevice::ReadOnly);

        QPixmap pixmap;
        QPoint offset;
        QPoint buttonTLPos;
        QPoint buttonBRPos;
        QString buttonadd;
        QString buttonmapto;
        QString longpress;
        QString locktime;
        dataStream >> pixmap >> offset >> buttonadd >> buttonmapto >> longpress >> locktime;
        buttonTLPos = ev->pos() - offset;
        buttonBRPos = buttonTLPos + QPoint(pixmap.size().width(), pixmap.size().height());

        // button是否在屏幕的显示范围之内
        if(rect.contains(buttonTLPos) && rect.contains(buttonBRPos))
        {
            XTouchPopup_Dialog_Button* button = new XTouchPopup_Dialog_Button(this);
            button->iButtonadd = buttonadd;
            button->iButtonmapto = buttonmapto;
            button->iButtonLongpress = longpress;
            button->iButtonLocktime = locktime;
            button->setPixmap(pixmap);
            // 应该有方法可以固定Size
            button->setMinimumSize(button->pixmap()->size());
            button->setMaximumSize(button->pixmap()->size());
            //
            button->move(ev->pos()-offset);
            button->iButtonPoint =button->pos()-iModelScreenRect.at(iModelNum).topLeft();
            button->show();
            button->setAttribute(Qt::WA_DeleteOnClose);
            iButtonAdd.append(button);

            ev->accept();
        }
    }
}
