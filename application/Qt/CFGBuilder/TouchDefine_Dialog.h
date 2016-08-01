#ifndef TOUCHDEFINE_DIALOG_H
#define TOUCHDEFINE_DIALOG_H

class QListWidget;
class QListWidgetItem;
class QPushButton;
class QDialogButtonBox;
class QLineEdit;
class QLabel;
class QComboBox;
class QBoxLayout;
class QGridLayout;
class TouchDefine_Dialog_Pad;
class TouchDefine_Dialog_Pad_Button;

#include <QDialog>
#include "TouchDefine_Dialog_Pad.h"

class TouchDefine_Dialog : public QDialog
{
    Q_OBJECT
public:
    TouchDefine_Dialog(QWidget *parent=0, ModelDir dir=Dir_Normal);
    
    // 外部设置手机屏幕方向
    ModelDir iDir;
public:
    void ModifyItem_Item(QListWidget* parent, QListWidgetItem* item = NULL);
    void Load();

signals:
    // 更改型号信号
    void signalChangeModel(ModelInfo model);
    
    // 更改缩放信号
    void signalChangeZoom(float zoom);

public slots:
    // 更改型号
    void slotChangeModel(int index);
    // 更改缩放
    void slotChangeZoom(int index);
    // 点击OK
    void slotOKClicked();
    

    
private:
    void CreateDialog();
    void CreateModelList();
    void CreateZoomList();
    void CreateScreenLED();
    void CreateButtonLED();
    void CreateModelPic();
    void CreateButtonNeedAdded();
    void CreateButtonBox();
    
    // 读取Model.list
    void LoadModelList();
    
private:
    // 机型列表
    QLabel* iAllModel;
    QComboBox* iAllModel_list;
    
    // 缩放比例
    QLabel* iZoom;
    QComboBox* iZoom_list;
    
    // 当前屏幕分辨率
    QLabel* iScreenSize;
    QLineEdit* iScreenSize_input;
    
    // 当前按键坐标
    QLabel* iCurButtonPos;
    QLineEdit* iCurButtonPos_input;
    
    // 手机屏幕
    TouchDefine_Dialog_Pad* iModelPad;
    
    // 可以添加的按键
    QList<TouchDefine_Dialog_Pad_Button*> iButton_list;
    
    // 已添加到屏幕上的按键
    QList<TouchDefine_Dialog_Pad_Button*> iButtonAdded_list;
    
    
    // 确定取消
    QDialogButtonBox* iButtonBox;
    QPushButton* iOk;
    QPushButton* iCancel;
    
    // 手机机型列表
    QList<QString> iModelList;
    // 手机图片分辨率列表
    QList<QSizeF> iModelSizeList;
    // 手机屏幕分辨率列表
    QList<QSizeF> iModelScreenSizeList;
    // 正常放置时的偏移量
    QList<QSizeF> iModelScreenOffsetList;
    // 默认缩放比例
    QList<float> iModelZoomList;
    // 手机图片方向
    QList<ModelDir> iModelDirList;
    // 手机图片名称
    QList<QString> iModelPic;
    
    // parent
    QListWidget* iParent;
    
    // item
    QListWidgetItem* iItem;
    
    // 手机屏幕跟右半边的排版
    QBoxLayout* iSubLayout;
    
    // 手机屏幕右半部分排版
    QGridLayout* iSubLayout1;
    
    // 需要添加按键部分的排版
    QGridLayout* iSubLayout2;
    
    // 整体排版
    QBoxLayout* iLayout;
    

};

#endif // TOUCHDEFINE_DIALOG_H
