#ifndef XPOPUPLINEEDIT_DIALOG_LABEL_H
#define XPOPUPLINEEDIT_DIALOG_LABEL_H

#include <QLabel>

class XPopupLineEdit_Dialog_Label : public QLabel
{
    Q_OBJECT
public:
    XPopupLineEdit_Dialog_Label(QWidget *parent = 0);

signals:
    void ClickKey(const QString& keyname);

public slots:

public:
    enum PadMode
    {
        NumPad,
        FullPad
    };

    void ChangePadMode(PadMode amode) {imode=amode;}

public: // event
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

private:
    PadMode imode;

    QRect irect;    // for get key's rect
};

#endif // XPOPUPLINEEDIT_DIALOG_LABEL_H
