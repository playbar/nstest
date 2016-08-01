#include "xpopupkeymapto_lineedit.h"
#include "xpopupkeymapto_dialog.h"
#include <QMouseEvent>

XPopupKeyMapTo_LineEdit::XPopupKeyMapTo_LineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    this->setReadOnly(true);

    iDialog = new XPopupKeyMapTo_Dialog(this);
    QObject::connect(iDialog, SIGNAL(KeyCode(QString)),
                     this, SLOT(SetKeyCode(QString)));
}

void XPopupKeyMapTo_LineEdit::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton)
    {
        iDialog->exec();
    }
}

void XPopupKeyMapTo_LineEdit::SetKeyCode(const QString &KeyCode)
{
    iDialog->close();
    this->clear();
    this->setText(KeyCode);
}
