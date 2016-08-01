#include "xtouchpopup_dialog_mapto.h"
#include <QPushButton>
#include <QDialogButtonBox>
#include <QBoxLayout>
#include "xpopupkeymapto_lineedit.h"
#include "xtouchpopup_dialog_button.h"
#include <QLabel>
#include <QHBoxLayout>

XTouchPopup_Dialog_Mapto::XTouchPopup_Dialog_Mapto(QWidget *parent) :
    QDialog(parent)
{
    iLayout = new QBoxLayout(QBoxLayout::TopToBottom);

    CreateDialog();
    this->setLayout(iLayout);
}

void XTouchPopup_Dialog_Mapto::InitMapto(XTouchPopup_Dialog_Button *button)
{
    iButton =  button;

    InitData();
}

void XTouchPopup_Dialog_Mapto::InitMapto(TouchDefine_Dialog_Pad_Button *button)
{
    iButtonNew = button;
    
    
}

void XTouchPopup_Dialog_Mapto::ClickOK()
{
    if(imapto->text().length()!=0)
    {
        iButton->iButtonmapto = imapto->text();
        this->close();
    }
}

void XTouchPopup_Dialog_Mapto::CreateDialog()
{
    CreateLineEdit();
    CreateButtonBox();
}

void XTouchPopup_Dialog_Mapto::CreateLineEdit()
{
    iLabel = new QLabel;
    iLabel->setText(tr("Mapto"));

    imapto = new XPopupKeyMapTo_LineEdit;
    QHBoxLayout* sublayout = new QHBoxLayout;
    sublayout->addWidget(iLabel);
    sublayout->addWidget(imapto);

    iLayout->addLayout(sublayout);
}

void XTouchPopup_Dialog_Mapto::CreateButtonBox()
{
    iButtonBox = new QDialogButtonBox;
    iOk = new QPushButton;
    iOk->setText(tr("OK"));
    QObject::connect(iOk, SIGNAL(clicked()),
                     this, SLOT(ClickOK()));
    iButtonBox->addButton(iOk, QDialogButtonBox::ActionRole);

    iCancel = new QPushButton;
    iCancel->setText(tr("Cancel"));
    QObject::connect(iCancel, SIGNAL(clicked()),
                     this, SLOT(close()));
    iButtonBox->addButton(iCancel, QDialogButtonBox::ActionRole);

    iLayout->addWidget(iButtonBox);
}

void XTouchPopup_Dialog_Mapto::InitData()
{
    imapto->setText(iButton->iButtonmapto);
}
