/*
 * TabThird中TxtInfo Group 中的 Item
 */
#ifndef TABTHIRD_GROUPFOUR_ITEM_H
#define TABTHIRD_GROUPFOUR_ITEM_H

//

//
#include <QListWidgetItem>

class TabThird_GroupFour_Item : public QListWidgetItem
{
public:
    TabThird_GroupFour_Item(QListWidget * parent = 0, int type = Type);

public:
    // 文本名称
    void SetTxtname(const QString& name);
    QString Txtname() const;

    // 文本内容
    void SetTxtdefine(const QString& define);
    QString Txtdefine() const;

private:
    // 文本名称
    QString     itxtname;

    // 文本内容
    QString     itxtdefine;
};

#endif // TABTHIRD_GROUPFOUR_ITEM_H
