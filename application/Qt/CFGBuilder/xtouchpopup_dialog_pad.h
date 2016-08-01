/*
 * 触屏方案对话框中的主要部分，显示手机
 */
#ifndef XTOUCHPOPUP_DIALOG_PAD_H
#define XTOUCHPOPUP_DIALOG_PAD_H

#include <QLabel>

class XTouchPopup_Dialog_Pad : public QLabel
{
    Q_OBJECT
public:
    XTouchPopup_Dialog_Pad(QWidget *parent = 0);

public slots:
    void LoadModelPic(const QString& modelpic);

public:
    void contextMenuEvent(QContextMenuEvent *ev);
    void focusInEvent(QFocusEvent *ev);
    void focusOutEvent(QFocusEvent *ev);

};

#endif // XTOUCHPOPUP_DIALOG_PAD_H
