#ifndef TABSECOND_GROUPFIVE_NEW_H
#define TABSECOND_GROUPFIVE_NEW_H

class TabSecond_GroupFive_Item;

#include <QListWidgetItem>
class TabSecond_GroupFive_New : public QListWidgetItem
{
public:
    explicit TabSecond_GroupFive_New(QListWidget * parent = 0, int type = Type);

public:
    // 方案名称
    QString iItemName;

    QList<TabSecond_GroupFive_Item*> iItemList;
};

#endif // TABSECOND_GROUPFIVE_NEW_H
