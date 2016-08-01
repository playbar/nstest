///*
// * 定义点击Touch_Define的添加时弹出的对话框
// */

//#ifndef XTOUCHDEFINE_DIALOG_H
//#define XTOUCHDEFINE_DIALOG_H

//class QPushButton;
//class QDialogButtonBox;
//class QBoxLayout;
//class QLabel;
//class QLineEdit;
//class QComboBox;
//class QHBoxLayout;
//class XTouchDefine_Dialog_Label;

//#include <QDialog>

//class XTouchDefine_Dialog : public QDialog
//{
//    Q_OBJECT
//public:
//    XTouchDefine_Dialog(QWidget *parent = 0);

//signals:

//public slots:
//    void SetTouchMode();

//private:
//    void CreateDialog();
//    void CreateTouchName();
//    void CreateTouchMode();
//    void CreateAreaZoom();
//    void CreateTouchArea();
//    void CreateTouchButton();
//    void CreateButtonBox();

//private:
//    // 触屏方案名称
//    QLabel* iTouchName;
//    QLineEdit* iTouchName_input;

//    // 手机样式选择
//    QLabel* iTouchMode;
//    QComboBox* iTouchMode_list;

//    // 缩放比例
//    QLabel* iAreaZoom;
//    QComboBox* iAreaZoom_list;

//    // 手机屏幕
//    XTouchDefine_Dialog_Label* iPhoneSkin;

//    // 添加按键
//    //

//    // 按键组
//    QPushButton*    iOK;
//    QPushButton*    iCancel;
//    QDialogButtonBox*   iButtonBox;

//    // 排版
//    QBoxLayout* iLayout;
//    QHBoxLayout* iSubLayout;    // for Name and Mode
//};

//#endif // XTOUCHDEFINE_DIALOG_H
