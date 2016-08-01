#include "xpopuplineedit_dialog.h"
#include <QBoxLayout>
#include <QPushButton>
#include "xpopuplineedit_dialog_label.h"

XPopupLineEdit_Dialog::XPopupLineEdit_Dialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle(tr("MobilePad"));
    iLayout = new QBoxLayout(QBoxLayout::TopToBottom);

    CreateDialog();
    this->setLayout(iLayout);
    emit ClickNumPad();
}

void XPopupLineEdit_Dialog::CreateDialog()
{
    CreateButtonBox();
    CreatePicArea();
}

void XPopupLineEdit_Dialog::CreateButtonBox()
{
    QBoxLayout* sublayout = new QBoxLayout(QBoxLayout::LeftToRight);

    iNumPad = new QPushButton;
    iNumPad->setText(tr("NumPad"));
    QObject::connect(iNumPad, SIGNAL(clicked()),
                     this, SLOT(ClickNumPad()));
    sublayout->addWidget(iNumPad);

    iFullPad = new QPushButton;
    iFullPad->setText(tr("FullPad"));
    QObject::connect(iFullPad, SIGNAL(clicked()),
                     this, SLOT(ClickFullPad()));
    sublayout->addWidget(iFullPad);

    iLayout->addLayout(sublayout);
}

void XPopupLineEdit_Dialog::CreatePicArea()
{
    iPicArea = new XPopupLineEdit_Dialog_Label;
    iPicArea->setMinimumSize(400, 600);
    iPicArea->setMaximumSize(400, 600);
    iPicArea->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QObject::connect(iPicArea, SIGNAL(ClickKey(QString)),
                     this, SLOT(GetKeyName(QString)));

    iLayout->addWidget(iPicArea);
}

void XPopupLineEdit_Dialog::ClickNumPad()
{
    // 设置图片
    QPixmap pic(":/image/logo/N73.png");
    iPicArea->setPixmap(pic);
    iPicArea->ChangePadMode(XPopupLineEdit_Dialog_Label::NumPad);
}

void XPopupLineEdit_Dialog::ClickFullPad()
{
    // 设置图片
    QPixmap pic(":/image/logo/E71.png");
    iPicArea->setPixmap(pic);
    iPicArea->ChangePadMode(XPopupLineEdit_Dialog_Label::FullPad);
}
