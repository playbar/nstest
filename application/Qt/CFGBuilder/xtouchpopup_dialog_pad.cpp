#include "xtouchpopup_dialog_pad.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QDebug>

XTouchPopup_Dialog_Pad::XTouchPopup_Dialog_Pad(QWidget *parent) :
    QLabel(parent)
{
    this->setFocusPolicy(Qt::ClickFocus);
}

void XTouchPopup_Dialog_Pad::LoadModelPic(const QString &modelpic)
{
    // 读取modelpic的png文件，如果不存在，读取NoModel.png文件
    QString path("ModelPic/");
    path += modelpic;
    path += ".png";
    QFile file(path);
    if(!file.exists())
        path = "ModelPic/NoModel.png";
    this->setPixmap(QPixmap(path));
    this->resize(this->pixmap()->size());
}

void XTouchPopup_Dialog_Pad::contextMenuEvent(QContextMenuEvent *ev)
{
//    if(QRect(QPoint(0,0), QPoint(500,500)).contains(ev->pos()))
//    {
//        QMenu* menu = new QMenu;
//        QAction* ChangeGameBackground = new QAction(menu);
//        ChangeGameBackground->setText(tr("Change"));
//        menu->addAction(ChangeGameBackground);

//        QAction* CleanGameBackground = new QAction(menu);
//        CleanGameBackground->setText(tr("Clean"));
//        menu->addAction(CleanGameBackground);

//        QAction* CleanAllButtons = new QAction(menu);
//        CleanAllButtons->setText(tr("Clean Buttons"));
//        menu->addAction(CleanAllButtons);

//        menu->exec(QCursor::pos());
//    }
    ev->ignore();
}

void XTouchPopup_Dialog_Pad::focusInEvent(QFocusEvent *ev)
{
    qDebug() << "In Event";
    ev->accept();
}

void XTouchPopup_Dialog_Pad::focusOutEvent(QFocusEvent *ev)
{
    qDebug() << "Out Event";
    ev->accept();
}
