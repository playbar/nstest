/*
 * KeyPress的输入框
 */

#ifndef XPOPUPLINEEDIT_H
#define XPOPUPLINEEDIT_H


class XPopupLineEdit_Dialog;

#include <QLineEdit>

class XPopupLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    XPopupLineEdit(QWidget *parent = 0);

signals:

public slots:
    void SetKeyName(const QString& KeyName);

public:
    void mousePressEvent(QMouseEvent *);

private:
    // dialog
    XPopupLineEdit_Dialog* iDialog;
};

#endif // XPOPUPLINEEDIT_H
