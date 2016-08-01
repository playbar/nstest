#include "TouchDefine_Dialog_Pad_Mapto.h"
#include <QPushButton>
#include <QDialogButtonBox>
#include <QBoxLayout>
#include "xpopupkeymapto_lineedit.h"
#include "xtouchpopup_dialog_button.h"
#include <QLabel>
#include <QHBoxLayout>
#include "TouchDefine_Dialog_Pad_Button.h"

TouchDefine_Dialog_Pad_Mapto::TouchDefine_Dialog_Pad_Mapto(QWidget *parent) :
    QDialog(parent)
{
    iLayout = new QBoxLayout(QBoxLayout::TopToBottom);

    CreateDialog();
    this->setLayout(iLayout);
}

void TouchDefine_Dialog_Pad_Mapto::InitMapto(TouchDefine_Dialog_Pad_Button *button)
{
    iButton =  button;

    InitData();
}

void TouchDefine_Dialog_Pad_Mapto::ClickOK()
{
    if(imapto->text().length()!=0)
    {
        iButton->iButtonMapto = imapto->text();

        this->close();
    }
}

void TouchDefine_Dialog_Pad_Mapto::CreateDialog()
{
    CreateLineEdit();
    CreateButtonBox();
}

void TouchDefine_Dialog_Pad_Mapto::CreateLineEdit()
{
    iLabel = new QLabel;
    iLabel->setText(tr("Mapto"));

    imapto = new XPopupKeyMapTo_LineEdit;
    QHBoxLayout* sublayout = new QHBoxLayout;
    sublayout->addWidget(iLabel);
    sublayout->addWidget(imapto);

    iLayout->addLayout(sublayout);
}

void TouchDefine_Dialog_Pad_Mapto::CreateButtonBox()
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

void TouchDefine_Dialog_Pad_Mapto::InitData()
{
    imapto->setText(iButton->iButtonMapto);
}