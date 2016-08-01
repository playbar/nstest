/*
 * TabSecond中KeyInfo Group List Item的详细信息
 */
#ifndef TABSECOND_GROUPFOUR_ITEM_H
#define TABSECOND_GROUPFOUR_ITEM_H

// declearation

// include
#include <QListWidgetItem>

class TabSecond_GroupFour_Item : public QListWidgetItem
{
public:
    TabSecond_GroupFour_Item(QListWidget* parent = 0, int type = Type);

public:
    // 被按下的手机按键
    void SetKeypress(const QString& Keypress);
    QString Keypress() const;

    // 映射按键
    void SetKeymapto(const QString& Keymapto);
    QString Keymapto() const;

    // 是否长按支持
    void SetLongpress(const QString& Longpress);
    QString Longpress() const;

    // 长按锁定时间
    void SetLocktime(const QString& Locktime);
    QString Locktime() const;

private:
    // 被按下的手机按键
    QString     iKeypress;

    // 映射按键
    QString     iKeymapto;

    // 是否长按支持
    QString     iLongpress;

    // 长按锁定时间
    QString     iLocktime;
};

#endif // TABSECOND_GROUPFOUR_ITEM_H
