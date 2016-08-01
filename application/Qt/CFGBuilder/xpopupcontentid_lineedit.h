#ifndef XPOPUPCONTENTID_LINEEDIT_H
#define XPOPUPCONTENTID_LINEEDIT_H

#include <QLineEdit>

class XPopupContentId_LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    XPopupContentId_LineEdit(QWidget *parent = 0);

signals:
    void ContentChanged(bool b);
    void signalUpdatePath(QString path);

public slots:
    void MidMd5(int position=0, int length=16);
    // 更新路径
    void slotUpdatePath(QString path)
    {
        ipath = path;
        ipath += ".swf";
    }

public:
    void mousePressEvent(QMouseEvent *);
    void contextMenuEvent(QContextMenuEvent * event) {}

private:
    QString ipath;
    QByteArray iMd5;
};

#endif // XPOPUPCONTENTID_LINEEDIT_H
