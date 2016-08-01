#ifndef XACTION_H
#define XACTION_H

class XTouchPopup_Dialog_Button;

#include <QAction>

class XAction : public QAction
{
    Q_OBJECT
public:
    explicit XAction(QObject *parent = 0);

signals:
    void ClickAction(XTouchPopup_Dialog_Button* button);

public slots:

};

#endif // XACTION_H
