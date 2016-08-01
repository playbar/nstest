#include "xtouchpopup_dialog_position.h"
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include "xtouchpopup_dialog_button.h"

XTouchPopup_Dialog_Position::XTouchPopup_Dialog_Position(QWidget *parent) :
    QDialog(parent)
{
    iLayout = new QBoxLayout(QBoxLayout::TopToBottom);

    CreateDialog();
    this->setLayout(iLayout);
}

void XTouchPopup_Dialog_Position::InitPosition(QRect screenrect, XTouchPopup_Dialog_Button *button)
{
    iScreenRect = screenrect;
    iButton = button;

    InitX();
    InitY();
}

void XTouchPopup_Dialog_Position::RClicked(bool b)
{
    if(b)
    {
        iXValue->setValue(iScreenRect.width()-(iXValue->value()));
    }
    else
    {
        iXValue->setValue(iScreenRect.width()-(iXValue->value()));
    }
}

void XTouchPopup_Dialog_Position::BClicked(bool b)
{
    iYValue->setValue(iScreenRect.height()-(iYValue->value()));
}

void XTouchPopup_Dialog_Position::OkClicked()
{
    QPoint p;
    if(!iR->isChecked())
    {
        p.setX(iXValue->value()+iScreenRect.topLeft().x());
    }
    else
    {
        p.setX(iScreenRect.topLeft().x()+(iScreenRect.width()-iXValue->value()));
    }

    if(!iB->isChecked())
    {
        p.setY(iYValue->value()+iScreenRect.topLeft().y());
    }
    else
    {
        p.setY(iScreenRect.topLeft().y()+(iScreenRect.height()-iYValue->value()));
    }

    iButton->move(p);
    iButton->iButtonPoint = p-iScreenRect.topLeft();

    this->close();
}

void XTouchPopup_Dialog_Position::CreateDialog()
{
    CreateX();
    CreateY();
    CreateButtonBox();
}

void XTouchPopup_Dialog_Position::CreateX()
{
    // 仅仅构造，为给定Value范围
    iX = new QLabel(this);
    iX->setText(tr("X:"));

    iXValue = new QSpinBox(this);

    iR = new QCheckBox(this);
    iR->setText(tr("R"));

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(iX);
    layout->addWidget(iXValue);
    layout->addWidget(iR);
    // 排版
    iLayout->addLayout(layout);
}

void XTouchPopup_Dialog_Position::CreateY()
{
    // 仅仅构造，未给定Value范围
    iY = new QLabel(this);
    iY->setText(tr("Y:"));

    iYValue = new QSpinBox(this);

    iB = new QCheckBox(this);
    iB->setText(tr("B"));

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(iY);
    layout->addWidget(iYValue);
    layout->addWidget(iB);
    // 排版
    iLayout->addLayout(layout);
}

void XTouchPopup_Dialog_Position::CreateButtonBox()
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

void XTouchPopup_Dialog_Position::InitX()
{
    iXValue->setRange(0, iScreenRect.width());
    iXValue->setValue((iButton->pos()-iScreenRect.topLeft()).x());
    iR->setChecked(false);
    QObject::connect(iR, SIGNAL(clicked(bool)),
                     this, SLOT(RClicked(bool)));
}

void XTouchPopup_Dialog_Position::InitY()
{
    iYValue->setRange(0, iScreenRect.height());
    iYValue->setValue((iButton->pos()-iScreenRect.topLeft()).y());
    iB->setChecked(false);
    QObject::connect(iB, SIGNAL(clicked(bool)),
                     this, SLOT(BClicked(bool)));
}
