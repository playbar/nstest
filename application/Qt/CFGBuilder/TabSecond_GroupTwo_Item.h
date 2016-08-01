#ifndef TABSECOND_GROUPTWO_ITEM_H
#define TABSECOND_GROUPTWO_ITEM_H

#include <QListWidgetItem>

class TabSecond_GroupTwo_Item : public QListWidgetItem
{
public:
    TabSecond_GroupTwo_Item(QListWidget * parent = 0, int type = Type);

public:
    // 虚拟按键
    void SetVirtualKey(const QString& avirtualkey);
    QString VirtualKey() const;

    // 对应鼠标动作
    void SetMouseAct(const QString& amouseact);
    QString MouseAct() const;

private:
    // 虚拟按键
    QString ivirtual_key;

    // 对应鼠标动作
    QString imouse_act;
};

#endif // TABSECOND_GROUPTWO_ITEM_H
