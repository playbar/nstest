/*
 * TabSecond中的OperatorBaseInfo Group
 */
#ifndef TABSECOND_GROUPONE_H
#define TABSECOND_GROUPONE_H

// declearation
class QLabel;
class QComboBox;
class QLineEdit;
class QGridLayout;
class QHBoxLayout;
class XEasyXml;
class QSpinBox;
class QGridLayout;

// include
#include <QGroupBox>

class TabSecond_GroupOne : public QGroupBox
{
    Q_OBJECT
    
public:
    TabSecond_GroupOne(QWidget* parent = NULL);

private:
    // 构造Group
    void SetupGroup();

    // 鼠标移动步长
    // 构造
    void SetupMousemovepace();

    // 长按锁定时间
    // 构造
    void SetupLocktime();
    
    // 触点半径
    void SetupTouchSize();

    // ZACC
    void SetupZacc();

    // ZBUTTON
    void SetupZbutton();

    
public:
    // 恢复默认
    void Default();

    // 读取数据
    void ReadData(XEasyXml* xml);

    // 保存数据
    void SaveData(XEasyXml* xml);

private:
    // 鼠标移动步长
    QLabel*     iMousemovepace;
//    QSpinBox*   iMousemovepace_box;
    QLineEdit* iMousemovepace_input;

    // 长按锁定时间
    QLabel*     iLocktime;
//    QSpinBox*   iLocktime_box;
    QLineEdit* iLocktime_input;
    
    // 触点半径
    QLabel* iTouchSize;
//    QSpinBox* iTouchSize_box;
    QLineEdit* iTouchSize_input;

    // zacc
    QLabel* iZacc;
    QComboBox* iZacc_list;

    // zbutton
    QLabel* iZbutton;
    QComboBox* iZbutton_list;

    // 排版
    QGridLayout* iLayout;
};

#endif // TABSECOND_GROUPONE_H
