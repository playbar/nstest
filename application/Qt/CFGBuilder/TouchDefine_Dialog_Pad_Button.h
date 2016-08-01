#ifndef TOUCHDEFINE_DIALOG_PAD_BUTTON_H
#define TOUCHDEFINE_DIALOG_PAD_BUTTON_H

#include <QLabel>

class TouchDefine_Dialog_Pad_Button : public QLabel
{
    Q_OBJECT
public:
    explicit TouchDefine_Dialog_Pad_Button(QWidget *parent = 0);

    // 按键类型
    enum ButtonType
    {
        Button_Up = 0,
        Button_Down = 1,
        Button_Left = 2,
        Button_Right = 3,
        Button_A = 4,
        Button_B = 5
    };
    
    // 长按锁定
    enum ButtonLongPressLock
    {
        ButtonLock_OFF = 0,
        ButtonLock_ON = 1
    };
    
    // 按键状态(已放置或未放置)
    enum ButtonStatus
    {
        ButtonStatus_Screen = 0,    // 已放置到屏幕上
        ButtonStatus_Wait = 1       // 等待放置状态
    };
    
    
    // 设置按键类型
    void SetButtonType(ButtonType t);
    // 设置长按锁定
    void SetButtonLock(ButtonLongPressLock l);
    // 设置按键状态
    void SetButtonStatus(ButtonStatus s);
    // 设置按键映射
    void SetButtonMapto(QString mapto);
    // 长按时间
    void SetButtonLockTime(int time);
    
signals:

public slots:
    // 映射至PC按键
    void slotButtonMapto();
    // 调整按键坐标
    void slotChangeButtonPos();
    // 长按锁定关
    void slotButtonLongPressLockOff();
    // 长按锁定开
    void slotButtonLongPressLockOn();
    // 长按锁定开启时间
    void slotButtonLockTime();
    // 删除按键
    void slotCleanButton();
    // 调整缩放比例
    void slotChangeZoom(float zoom);
    
private:
    // 点击事件
    void mousePressEvent(QMouseEvent *ev);
    // 重绘事件
    void paintEvent(QPaintEvent *);
    // 右键事件
    void contextMenuEvent(QContextMenuEvent *ev);
    
private:
    // 刷新按键图片
    void updateButton();
    
public:
    // 对齐点
    int iOrg;
    
public:
    // 按键类型
    ButtonType iType;
    // 按键状态
    ButtonStatus iStatus;
    // 按键图片
    QPixmap iButtonPic;
    // 按键尺寸
    QSizeF  iButtonSize;
    // 按键坐标
    QPointF iButtonPos;
    // 对应PC按键
    QString iButtonMapto;
    // 长按时间
    int iButtonLockTime;
    // 当前缩放比例
    float iZoom;
    // 长按锁定
    ButtonLongPressLock iLongPressLock;
    
    // 添加到屏幕上的按键组
    QList<TouchDefine_Dialog_Pad_Button*>* iButtonAdded_list;
};

#endif // TOUCHDEFINE_DIALOG_PAD_BUTTON_H
