/*
 * TabFirst中的MouseBaseInfo Group
 */
#ifndef TABFIRST_GROUPTWO_H
#define TABFIRST_GROUPTWO_H

// declearation
class QLabel;
class QLineEdit;
class QComboBox;
class QGridLayout;
class XEasyXml;

// include
#include <QGroupBox>

class TabFirst_GroupTwo : public QGroupBox
{
public:
    TabFirst_GroupTwo(QWidget* parent = NULL);

private:
    // 构造Group
    void SetupGroup();

    // 鼠标操作模式
    void SetupMousemode();

    // 模式切换热键
    void SetupMousemodechange();

public:
    // 恢复默认
    void Default();

    // 读取数据
    void ReadData(XEasyXml* xml);

    // 保存数据
    void SaveData(XEasyXml* xml);

private:
    // 鼠标操作模式
    QLabel*     iMousemode;
    QLineEdit*  iMousemode_input;

    // 模式切换热键
    QLabel*     iMousemodechange;
    QLineEdit*  iMousemodechange_input;

    // 排版(4, 1)
    QGridLayout*    iLayout;
};

#endif // TABFIRST_GROUPTWO_H
