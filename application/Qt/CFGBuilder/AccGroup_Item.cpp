#include "AccGroup_Item.h"

AccGroup_Item::AccGroup_Item(QListWidget *parent, int type) :
    QListWidgetItem(parent, type)
{
    BMapto = false;
    BMouseAct = false;
}
