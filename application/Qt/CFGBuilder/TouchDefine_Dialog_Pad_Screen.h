/*
 * 此为手机屏幕
 */

#ifndef TOUCHDEFINE_DIALOG_PAD_SCREEN_H
#define TOUCHDEFINE_DIALOG_PAD_SCREEN_H

#include <QLabel>

class TouchDefine_Dialog_Pad_Button;

class TouchDefine_Dialog_Pad_Screen : public QLabel
{
    Q_OBJECT
public:
    explicit TouchDefine_Dialog_Pad_Screen(QWidget *parent = 0);

    // 游戏显示背景
    enum BGPos
    {
        BGPos_Center,               // 居中
        BGPos_FullScreen         // 全屏
    };
    
    QSizeF NativeSize() const { return iScreenSize; }
    
signals:
    // 当前按键坐标
    void signalCurButtonPos(QString pos);
    // 清除所有按键
    void signalCleanAllButtons();
    // 更改缩放比例
    void signalChangeZoom(float zoom);

public slots:
    // 更改屏幕尺寸
    void slotChangeScreenSize(QSizeF ScreenSize);
    // 更改缩放比例
    void slotChangeZoom(float zoom);
    
    // 右键菜单事件
    // 更改游戏背景
    void slotChangeBackground();
    // 更改背景显示位置
    void slotChangeBackgroundPos_Center();
    void slotChangeBackgroundPos_FullScreen();
    // 清除游戏背景
    void slotCleanBackground();
    // 清除已添加按键
    void slotCleanAllButtons();
    
private:
    // 进入
    void dragEnterEvent(QDragEnterEvent *);
    // 移动
    void dragMoveEvent(QDragMoveEvent *);
    // 离开
    void dragLeaveEvent(QDragLeaveEvent *);
    // 拖放
    void dropEvent(QDropEvent *);
    // 刷新屏幕背景
    void updateBackground();
    // 刷新游戏背景
    void updateGameScreen();
    
private:
    // 右键事件
    void contextMenuEvent(QContextMenuEvent *ev);
    // 重绘事件
    void paintEvent(QPaintEvent *);
    
public:
    // 当前游戏背景显示位置
    BGPos iBGPos;
    // 当前屏幕原始尺寸
    QSizeF iScreenSize;
    // 当前选择的缩放比例
    float      iZoom;
    // 当前游戏背景
    bool        iHasGameScreen;
    QPixmap iGameScreen;
    
public:
    // 添加到屏幕上的按键
    QList<TouchDefine_Dialog_Pad_Button*>* iButtonAdded_list;
};

#endif // TOUCHDEFINE_DIALOG_PAD_SCREEN_H
