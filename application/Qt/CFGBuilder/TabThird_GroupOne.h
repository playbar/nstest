/*
 * TabThird中的DisplayInfo Group
 */
#ifndef TABTHIRD_GROUPONE_H
#define TABTHIRD_GROUPONE_H

// declearation
class QLabel;
class QComboBox;
class QGridLayout;
class QTextCodec;
class QLineEdit;
class XEasyXml;
#include "TouchDefine_Dialog_Pad.h"

// include
#include <QGroupBox>

class TabThird_GroupOne : public QGroupBox
{
    Q_OBJECT
    
public:
    TabThird_GroupOne(QWidget* parent = NULL);

private:
    // 构造Group
    void SetupGroup();

    // 屏幕显示方向
    void SetupScreendir();

    // 显示缩放比例
    void SetupZoomsize();
    
    // 显示质量
    void SetupQuality();

    // 广告位置
    void SetupAdPos();

    // 广告url
    void SetupAdUrl();

public:
    // 恢复默认
    void Default();

    // 读取数据
    void ReadData(XEasyXml* xml);

    // 保存数据
    void SaveData(XEasyXml* xml);
    
public slots:
    void slotDirChange(int a)
    {
        if(a==0)
            emit signalDirChange(Dir_Normal);
        else if(a==1)
            emit signalDirChange(Dir_ACW90);
    }
    
signals:
    void signalDirChange(ModelDir);

private:
    // 屏幕显示方向
    QLabel*     iScreendir;
    QComboBox*  iScreendir_list;

    // 显示缩放比例
    QLabel*     iZoomsize;
    QComboBox*  iZoomsize_list;
    
    // 显示质量
    QLabel* iQuality;
    QComboBox* iQuality_list;

    // 广告位置
    QLabel* iAdPos;
    QLineEdit* iAdPos_input;

    // 广告地址
    QLabel* iAdUrl;
    QLineEdit* iAdUrl_input;

    QGridLayout*    iLayout;
};

#endif // TABTHIRD_GROUPONE_H
