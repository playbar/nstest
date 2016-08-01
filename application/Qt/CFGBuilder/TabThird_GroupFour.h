/*
 * TabThird中的TxtInfo Group
 */
#ifndef TABTHIRD_GROUPFOUR_H
#define TABTHIRD_GROUPFOUR_H

// declearation
class QListWidget;
class QTextCodec;
class QVBoxLayout;
class QPushButton;
class QDialogButtonBox;
class QListWidgetItem;
class TabThird_GroupFour_Dialog;
class QTextEdit;
class XEasyXml;
class QPlainTextEdit;

// include
#include <QGroupBox>

class TabThird_GroupFour : public QGroupBox
{
    Q_OBJECT

public:
    TabThird_GroupFour(QWidget* parent = NULL);

private:
    // 构造Group
    void SetupGroup();

public:
    // 恢复默认
    void Default();

    // 读取数据
    void ReadData(XEasyXml* xml);

    // 保存数据
    void SaveData(XEasyXml* xml);

private:
    // 排版
    QVBoxLayout*    iLayout;

    // 内容
    QTextEdit*      itxt_input;
};

#endif // TABTHIRD_GROUPFOUR_H
