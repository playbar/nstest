/*
 * TabFirst中的ConfigBaseInfo Group
 */
#ifndef TABFIRST_GROUPONE_H
#define TABFIRST_GROUPONE_H

// declearation
class QLabel;
class QLineEdit;
class QComboBox;
class QTextCodec;
class QGridLayout;
class QSpinBox;
class QCheckBox;
class XPopupContentId_LineEdit;

// declearation
class XEasyXml;

// include
#include <QGroupBox>

class TabFirst_GroupOne : public QGroupBox
{
    Q_OBJECT

public:
    TabFirst_GroupOne(QWidget *parent = NULL);

    // 恢复默认
    void Default();

    // 读取数据
    void ReadData(XEasyXml* xml);

    // 保存数据
    void SaveData(XEasyXml* xml);

private:
    // 构造Group
    void SetupGroup();

    // FLASH内容ID操作
    void SetupContentid();

    // FLASH内容等级操作
    void SetupContentlevel();

    // 手机操作类型操作
    void SetupOperatortype();

    // Content_type
    void SetupContentType();

public slots:
    void ClickCheckBox(bool b);
    
    void slotUpdatePathOtoI(QString path)
    {
        emit signalUpdatePathOtoI(path);
    }
    
    void slotUpdatePathItoO(QString path)
    {
        emit signalUpdatePathItoO(path);
    }

signals:
    void ChangeMD5(int p, int l);
    //
    void signalUpdatePathOtoI(QString);
    void signalUpdatePathItoO(QString);

private:
    // FLASH内容ID
    QLabel*     icontent_id;
//    QSpinBox*   icontent_id_box;
    XPopupContentId_LineEdit* icontent_id_input;

    // 取后16位
    QCheckBox*  iChangBox;

    // FLASH内容等级
    QLabel*     icontent_level;
//    QSpinBox*   icontent_level_box;
    QLineEdit* icontent_level_input;

    // 手机操作类型
    QLabel*     ioperator_type;
    QComboBox*  ioperator_type_list;

    // content_type
    QLabel*     icontent_type;
    QComboBox*  icontent_type_list;

    // 排版(5, 2)
    QGridLayout*    iLayout;
};

#endif // TABFIRST_GROUPONE_H
