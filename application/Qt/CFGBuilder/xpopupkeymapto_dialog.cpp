#include "xpopupkeymapto_dialog.h"
#include <QVBoxLayout>
#include "xpopupkeymapto_dialog_label.h"

XPopupKeyMapTo_Dialog::XPopupKeyMapTo_Dialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle(tr("VirtualKeyPad"));
    iLayout = new QVBoxLayout;

    CreateDialog();
    this->setLayout(iLayout);
}

void XPopupKeyMapTo_Dialog::CreateDialog()
{
    CreatePicArea();
}

void XPopupKeyMapTo_Dialog::CreatePicArea()
{
    iPicArea = new XPopupKeyMapTo_Dialog_Label;
    iPicArea->setMinimumSize(800, 250);
    iPicArea->setMaximumSize(800, 250);
    iPicArea->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QPixmap pic(":/image/logo/keymap.png");
    iPicArea->setPixmap(pic);
    QObject::connect(iPicArea, SIGNAL(ClickKey(QString)),
                     this, SLOT(GetKeyCode(QString)));

    iLayout->addWidget(iPicArea);
}
