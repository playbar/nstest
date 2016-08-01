#include "xpopupkeymapto_dialog_label.h"
#include <QMouseEvent>
#include <QMessageBox>
#include <QApplication>
#include <QClipboard>
#include "XVirtualKeyData.h"


XPopupKeyMapTo_Dialog_Label::XPopupKeyMapTo_Dialog_Label(QWidget *parent) :
    QLabel(parent)
{
    this->setMouseTracking(true);
}

void XPopupKeyMapTo_Dialog_Label::mouseMoveEvent(QMouseEvent *ev)
{
    for(int i=0; i<Key_Count; i++)
    {
        if(Key_Pos[i].contains(ev->pos()))
        {
            this->setCursor(QCursor(Qt::PointingHandCursor));
            break;
        }
        else
            this->unsetCursor();
    }
}

void XPopupKeyMapTo_Dialog_Label::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton)
    {
        for(int i=0; i<Key_Count; i++)
        {
            if(Key_Pos[i].contains(ev->pos()))
            {
                emit ClickKey(Key_Name[i]);
                break;
            }
        }
    }
    else if(ev->button()==Qt::RightButton)
    {
        irect.setTopLeft(ev->pos());
    }
}

void XPopupKeyMapTo_Dialog_Label::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::RightButton)
    {
        irect.setBottomRight(ev->pos());

        // 弹出提示信息，显示区域范围
        QMessageBox* msg = new QMessageBox(this);
        QString copy;
        QString value;
        value += "(";
        QString value1;
        value1.setNum(irect.topLeft().x());
        value += value1;
        copy += value1;
        value += ",";
        copy += ",";
        value1.setNum(irect.topLeft().y());
        value += value1;
        copy += value1;
        value += ",";
        copy += ",";
        value1.setNum(irect.bottomRight().x()-irect.topLeft().x());
        value += value1;
        copy += value1;
        value += ",";
        copy += ",";
        value1.setNum(irect.bottomRight().y()-irect.topLeft().y());
        value += value1;
        copy += value1;
        value += ")";
        msg->setText(value);

        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(copy);

        msg->exec();
    }
}
