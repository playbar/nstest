/*
 * 触屏方案对话框右半部分的按键
 */
#ifndef XTOUCHPOPUP_DIALOG_BUTTON_H
#define XTOUCHPOPUP_DIALOG_BUTTON_H

#include <QLabel>

class XTouchPopup_Dialog_Button : public QLabel
{
    Q_OBJECT
public:
    XTouchPopup_Dialog_Button(QWidget *parent = 0);
    void Copy(XTouchPopup_Dialog_Button* button);

signals:

public slots:
protected:
    void paintEvent(QPaintEvent *ev);


public:
    enum ButtonType
    {
        Button_UP,
        Button_DOWN,
        Button_LEFT,
        Button_RIGHT,
        Button_A,
        Button_B
    };

public:
    ButtonType iType;
    QString iButtonadd;
    QString iButtonmapto;
    QPoint iButtonPoint;
    QString iButtonLongpress;
    QString iButtonLocktime;
};

#endif // XTOUCHPOPUP_DIALOG_BUTTON_H
