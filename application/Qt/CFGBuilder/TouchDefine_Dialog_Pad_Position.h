#ifndef TOUCHDEFINE_DIALOG_PAD_POSITION_H
#define TOUCHDEFINE_DIALOG_PAD_POSITION_H


class QDialogButtonBox;
class QLabel;
class QSpinBox;
class QCheckBox;
class QBoxLayout;
class TouchDefine_Dialog_Pad_Button;

#include <QDialog>

class TouchDefine_Dialog_Pad_Position : public QDialog
{
    Q_OBJECT
public:
    explicit TouchDefine_Dialog_Pad_Position(QWidget *parent = 0);
    void InitDialog(QSizeF screensize, float zoom, int org, TouchDefine_Dialog_Pad_Button* button);
    
private:
    void CreateDialog();
    void CreateX();
    void CreateY();
    void CreateButtonBox();
    
    
signals:

public slots:
    // R
    void slotRChecked(bool );
    // B
    void slotBChecked(bool );
    // OK
    void slotClickOK();
    
private:
    // X坐标
    QLabel* iXLabel;
    QSpinBox* iXValue;
    // R
    QCheckBox* iR;

    // Y坐标
    QLabel* iYLabel;
    QSpinBox* iYValue;
    // B
    QCheckBox* iB;
    
    // 按键
    QDialogButtonBox*   iButtonBox;
    QPushButton* iOk;
    QPushButton* iCancel;
    
    // 排版
    QBoxLayout* iLayout;
    
    // 当前屏幕的原始分辨率
    QSizeF iScreenSize;
    // 当前缩放比例
    float iZoom;
    // 相对应的按键
    TouchDefine_Dialog_Pad_Button* iButton;

    // 对齐点
    int iOrg;
};

#endif // TOUCHDEFINE_DIALOG_PAD_POSITION_H
