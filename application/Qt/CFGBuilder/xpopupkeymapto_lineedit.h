#ifndef XPOPUPKEYMAPTO_LINEEDIT_H
#define XPOPUPKEYMAPTO_LINEEDIT_H

class XPopupKeyMapTo_Dialog;

#include <QLineEdit>

class XPopupKeyMapTo_LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    XPopupKeyMapTo_LineEdit(QWidget *parent = 0);

signals:

public slots:
    void SetKeyCode(const QString& KeyCode);

public:
    void mousePressEvent(QMouseEvent *);

private:
    // dialog
    XPopupKeyMapTo_Dialog* iDialog;
};

#endif // XPOPUPKEYMAPTO_LINEEDIT_H
