#ifndef TOUCHDEFINE_DIALOG_PAD_H
#define TOUCHDEFINE_DIALOG_PAD_H

class TouchDefine_Dialog_Pad_Screen;
class TouchDefine_Dialog_Pad_Button;

#include <QLabel>

// 手机方向
enum ModelDir
{
    Dir_Normal,     // 正常放置
    Dir_CW90,       // 顺时针旋转90度
    Dir_CW180,      // 顺时针旋转180度
    Dir_ACW90       // 逆时针旋转90度
};

// 从Dialog传递过来的手机信息
struct ModelInfo
{
    // 手机名称
    QString ModelName;
    // 相对应的手机图片名称
    QString ModelPicName;
    // 手机图片分辨率
    QSizeF ModelSize;
    // 手机屏幕分辨率
    QSizeF ModelScreenSize;
    // 屏幕偏移量
    QSizeF ModelScreenOffset;
    // 默认缩放比例
    float ModelZoom;
    // 需要旋转方向（这里跟Dialog中的方向不一样，此处为通过计算原图片方向及配置文件中的方向后，需要将原图片旋转的方向）
    ModelDir eModelDir;
};

class TouchDefine_Dialog_Pad : public QLabel
{
    Q_OBJECT
public:
    explicit TouchDefine_Dialog_Pad(QWidget *parent = 0);
    
    void AddButtonListP(QList<TouchDefine_Dialog_Pad_Button*>*);

signals:
    // 发送更改屏幕分辨率的信号
    void signalChangeScreenSize(QSizeF ScreenSize);
    // 发送更改缩放比例的信号
    void signalChangeZoom(float zoom);
    // 发送当前按键坐标（屏幕的）
    void signalCurButtonPos(QString pos);

public slots:
    // 更改机型
    void slotChangeModel(ModelInfo model);
    // 更改缩放比例
    void slotChangeZoom(float zoom);
    // 当前按键坐标（屏幕的）
    void slotCurButtonPos(QString pos);
    
private:
    // 重绘
    void paintEvent(QPaintEvent *);
    
private:
    // 刷新手机图片
    void updateBackground();
    
public:
    // 手机屏幕
    TouchDefine_Dialog_Pad_Screen* iScreen;
    
    // 手机图片
    QPixmap iModelPic;
    
    // 手机图片存在信息
    bool iModelPicFlag;
    
    // 手机信息
    ModelInfo iModelInfo;

};

#endif // TOUCHDEFINE_DIALOG_PAD_H
