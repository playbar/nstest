#include "xpopuplineedit.h"
#include <QMouseEvent>
#include "xpopuplineedit_dialog.h"

XPopupLineEdit::XPopupLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    this->setReadOnly(true);

    iDialog = new XPopupLineEdit_Dialog(this);
    QObject::connect(iDialog, SIGNAL(KeyName(QString)),
                     this, SLOT(SetKeyName(QString)));
}

void XPopupLineEdit::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton)
    {
        iDialog->exec();
    }
}

void XPopupLineEdit::SetKeyName(const QString &KeyName)
{
    iDialog->close();
    this->clear();
    this->setText(KeyName);
}
