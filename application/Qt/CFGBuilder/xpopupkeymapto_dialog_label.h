#ifndef XPOPUPKEYMAPTO_DIALOG_LABEL_H
#define XPOPUPKEYMAPTO_DIALOG_LABEL_H

#include <QLabel>

class XPopupKeyMapTo_Dialog_Label : public QLabel
{
    Q_OBJECT
public:
    XPopupKeyMapTo_Dialog_Label(QWidget *parent = 0);

signals:
    void ClickKey(const QString& keycode);

public slots:

public:
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

private:
    QRect irect;

};

#endif // XPOPUPKEYMAPTO_DIALOG_LABEL_H
