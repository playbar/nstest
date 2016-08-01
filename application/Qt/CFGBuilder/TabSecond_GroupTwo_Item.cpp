#include "TabSecond_GroupTwo_Item.h"

TabSecond_GroupTwo_Item::TabSecond_GroupTwo_Item(QListWidget *parent, int type)
    :QListWidgetItem(parent, type)
{
}

void TabSecond_GroupTwo_Item::SetVirtualKey(const QString &avirtualkey)
{
    ivirtual_key = avirtualkey;
}

QString TabSecond_GroupTwo_Item::VirtualKey() const
{
    return ivirtual_key;
}

void TabSecond_GroupTwo_Item::SetMouseAct(const QString& amouseact)
{
    imouse_act = amouseact;
}

QString TabSecond_GroupTwo_Item::MouseAct() const
{
    return imouse_act;
}
