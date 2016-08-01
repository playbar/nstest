#include "TabSecond_GroupThree_Item.h"

TabSecond_GroupThree_Item::TabSecond_GroupThree_Item(QListWidget *parent, int type)
    :QListWidgetItem(parent, type)
{
}

void TabSecond_GroupThree_Item::SetVirtualKey(const QString &avirtualkey)
{
    ivirtual_key = avirtualkey;
}

QString TabSecond_GroupThree_Item::VirtualKey() const
{
    return ivirtual_key;
}

void TabSecond_GroupThree_Item::SetMouseAct(const QString& amouseact)
{
    imouse_act = amouseact;
}

QString TabSecond_GroupThree_Item::MouseAct() const
{
    return imouse_act;
}
