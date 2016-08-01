/**/

#include "TabSecond_GroupFour_Item.h"

TabSecond_GroupFour_Item::TabSecond_GroupFour_Item(QListWidget *parent, int type)
    : QListWidgetItem(parent, type)
{
}

void TabSecond_GroupFour_Item::SetKeypress(const QString &Keypress)
{
    iKeypress = Keypress;
}

QString TabSecond_GroupFour_Item::Keypress() const
{
    return iKeypress;
}

void TabSecond_GroupFour_Item::SetKeymapto(const QString &Keymapto)
{
    iKeymapto = Keymapto;
}

QString TabSecond_GroupFour_Item::Keymapto() const
{
    return iKeymapto;
}

void TabSecond_GroupFour_Item::SetLongpress(const QString &Longpress)
{
    iLongpress = Longpress;
}

QString TabSecond_GroupFour_Item::Longpress() const
{
    return iLongpress;
}

void TabSecond_GroupFour_Item::SetLocktime(const QString &Locktime)
{
    iLocktime = Locktime;
}

QString TabSecond_GroupFour_Item::Locktime() const
{
    return iLocktime;
}
