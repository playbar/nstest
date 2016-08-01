#include "xtouchpopup_dialog_button.h"
#include <QPainter>

XTouchPopup_Dialog_Button::XTouchPopup_Dialog_Button(QWidget *parent) :
    QLabel(parent)
{
    this->setFocusPolicy(Qt::ClickFocus);
}

void XTouchPopup_Dialog_Button::Copy(XTouchPopup_Dialog_Button *button)
{
    button->iType = iType;
    button->iButtonadd = iButtonadd;
    button->iButtonmapto = iButtonmapto;
    button->iButtonLongpress = iButtonLongpress;
    button->iButtonLocktime = iButtonLocktime;
}

void XTouchPopup_Dialog_Button::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    painter.setPen(Qt::SolidLine);
    painter.setPen(QPen(QColor(0, 255, 0)));
    painter.drawRoundedRect(QRect(this->x(), this->y(),this->width(),this->height()), 20, 15);

}


