#include "xtouchpopup_dialog_longpress.h"
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include "xtouchpopup_dialog_button.h"
#include <QHBoxLayout>

XTouchPopup_Dialog_LongPress::XTouchPopup_Dialog_LongPress(QWidget *parent) :
    QDialog(parent)
{
    iLayout = new QBoxLayout(QBoxLayout::TopToBottom);

    CreateDialog();

    this->setLayout(iLayout);
}

void XTouchPopup_Dialog_LongPress::Init(XTouchPopup_Dialog_Button *button)
{
    iButton = button;

    InitLongPress();
    InitLongPressTime();
}

void XTouchPopup_Dialog_LongPress::OkClicked()
{
    if(!iLongPress->isChecked())
    {
        iButton->iButtonLongpress = "false";
        iButton->iButtonLocktime.clear();
    }
    else
    {
        iButton->iButtonLongpress = "true";
        iButton->iButtonLocktime = QString::number(iLongPressTime_box->value());
    }

    QString pixmap;
    if(iButton->iButtonLongpress=="true")
        pixmap += ":/image/logo/longlock/";
    else
        pixmap += ":/image/logo/unlonglock/";
    pixmap += iButton->iButtonadd;
    pixmap += ".png";
    iButton->setPixmap(QPixmap(pixmap));

    this->close();
}

void XTouchPopup_Dialog_LongPress::InitLongPress()
{
    if(iButton->iButtonLongpress=="true")
    {
        iLongPress->setChecked(true);
        iLongPressTime_box->setValue(iButton->iButtonLocktime.toInt());
    }
    else
    {
        iLongPress->setChecked(false);
    }
}

void XTouchPopup_Dialog_LongPress::InitLongPressTime()
{
    if(iLongPress->isChecked())
    {
        iLongPressTime_box->setEnabled(true);
        iLongPressTime_box->setValue(iButton->iButtonLocktime.toInt());
    }
    else
        iLongPressTime_box->setEnabled(false);
}

void XTouchPopup_Dialog_LongPress::CreateDialog()
{
    CreateLongPress();
    CreateLongPressTime();
    CreateButtonBox();
}

void XTouchPopup_Dialog_LongPress::CreateLongPress()
{
    iLongPress = new QCheckBox;
    iLongPress->setText(tr("LongPress"));

    isubLayout = new QHBoxLayout;
    isubLayout->addWidget(iLongPress);
}

void XTouchPopup_Dialog_LongPress::CreateLongPressTime()
{
    iLongPressTime = new QLabel;
    iLongPressTime->setText(tr("LockTime"));

    iLongPressTime_box = new QSpinBox;
    iLongPressTime_box->setEnabled(false);
    iLongPressTime_box->setMaximum(1000);
//    iLongPressTime_box->setRange(800, 1000);
    QObject::connect(iLongPress, SIGNAL(clicked(bool)),
                     iLongPressTime_box, SLOT(setEnabled(bool)));

    isubLayout->addWidget(iLongPressTime);
    isubLayout->addWidget(iLongPressTime_box);

    iLayout->addLayout(isubLayout);
}

void XTouchPopup_Dialog_LongPress::CreateButtonBox()
{
    iButtonBox = new QDialogButtonBox;
    iOk = new QPushButton;
    iOk->setText(tr("OK"));
    QObject::connect(iOk, SIGNAL(clicked()),
                     this, SLOT(OkClicked()));
    iButtonBox->addButton(iOk, QDialogButtonBox::ActionRole);

    iCancel = new QPushButton;
    iCancel->setText(tr("Cancel"));
    QObject::connect(iCancel, SIGNAL(clicked()),
                     this, SLOT(close()));

    iButtonBox->addButton(iCancel, QDialogButtonBox::ActionRole);
    iLayout->addWidget(iButtonBox);
}
