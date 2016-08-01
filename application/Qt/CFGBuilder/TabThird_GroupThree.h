/*
 * TabThird中的NetworkInfo Group
 */
#ifndef TABTHIRD_GROUPTHREE_H
#define TABTHIRD_GROUPTHREE_H

// declearation
class QLabel;
class QLineEdit;
class QComboBox;
class QTextCodec;
class QGridLayout;
class QTextEdit;
class XEasyXml;

// include
#include <QGroupBox>

class TabThird_GroupThree : public QGroupBox
{
   Q_OBJECT
    
public:
    TabThird_GroupThree(QWidget* parent = NULL);

private:
    // 构造Group
    void SetupGroup();

    // 积分类型
    void SetupScoretype();

    // 上传的积分变量名
    void SetupScorevarname();

    // 上传积分的url
    void SetupSubmiturl();
    
    // autotag
    void SetupAutoTag();

public slots:
    void lock(int index);
    
public:
    // 恢复默认
    void Default();

    // 读取数据
    void ReadData(XEasyXml* xml);

    // 保存数据
    void SaveData(XEasyXml* xml);

private:
    // 积分类型
    QLabel*     iscoretype;
    QComboBox*  iscoretype_list;

    // 上传的积分变量名
    QLabel*     iscorevarname;
    QLineEdit*  iscorevarname_input;

    // 上传积分的url
    QLabel*     isubmiturl;
//    QLineEdit*  isubmiturl_input;
    QTextEdit*  isubmiturl_input;
    
    // autotag
    QLabel* iAutoTag;
    QLineEdit* iAutoTag_input;

    // 排版
    QGridLayout*    iLayout;
};

#endif // TABTHIRD_GROUPTHREE_H
