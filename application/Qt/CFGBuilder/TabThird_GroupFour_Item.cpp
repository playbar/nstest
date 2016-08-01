/*
 *
 */

#include "TabThird_GroupFour_Item.h"

TabThird_GroupFour_Item::TabThird_GroupFour_Item(QListWidget * parent, int type)
    :QListWidgetItem(parent, type)
{
}

void TabThird_GroupFour_Item::SetTxtname(const QString &name)
{
    itxtname = name;
}

QString TabThird_GroupFour_Item::Txtname() const
{
    return itxtname;
}

void TabThird_GroupFour_Item::SetTxtdefine(const QString &define)
{
    itxtdefine = define;
}

QString TabThird_GroupFour_Item::Txtdefine() const
{
    return itxtdefine;
}
