/*
 * 添加触屏按键方案时弹出的对话框
 */

#ifndef XTOUCHPOPUP_DIALOG_H
#define XTOUCHPOPUP_DIALOG_H

class QLabel;
class QLineEdit;
class QComboBox;
class QHBoxLayout;
class QBoxLayout;
class QVBoxLayout;
class XTouchPopup_Dialog_Pad;
class QGridLayout;
class XTouchPopup_Dialog_Button;
class QPushButton;
class QDialogButtonBox;
class QListWidget;
class QListWidgetItem;

#include <QDialog>
#include <QDebug>

class XTouchPopup_Dialog : public QDialog
{
    Q_OBJECT
public:
    XTouchPopup_Dialog(QWidget *parent = 0);

public:
    void ModifyItem_Item(QListWidget* parent, QListWidgetItem* item = NULL);

signals:
    void ModelChanged(const QString& newmodel);
    void ZoomChanged(double zoom);

    void Clockwise();
    void Eastern();

    void ClickAction(XTouchPopup_Dialog_Button* button);
public slots:
    void ListenModelChange(int index) {iModelNum=index;}
    void ListenModelChange(const QString& newmodel) { emit ModelChanged(newmodel); }
//    void ListenSizeChange();
    void ListenZoomChange(const QString& zoom);

    void ClickChange();
    void ClickClean();
    void ClickCleanButtons();

    void ClickMapto();
    void ClickPosition();
    void ClickLongPress();
    void ClickDelete();

    void OKClicked();
private:
    void CreateDialog();
    void CreateName();
    void CreateModel();
    void CreateZoom();

    void InitModel();
    void InitZoom();

    void CreatePad();

    void CreateSpinButton();

    void CreateButton();

    void CreateButtonBox();

    void InitData();

public:
    void mousePressEvent(QMouseEvent *ev);
    void mouseDoubleClickEvent(QMouseEvent *ev);
    void contextMenuEvent(QContextMenuEvent *ev);
    void dragEnterEvent(QDragEnterEvent *ev);
//    void dragMoveEvent(QDragMoveEvent *ev);
//    void dragLeaveEvent(QDragLeaveEvent *ev);
    void dropEvent(QDropEvent *ev);

private:
    // 方案名称
    QLabel* iName;
    QLineEdit*  iName_input;

    // 机型选择列表
    QLabel* iModel;
    QComboBox*  iModel_list;

    // 缩放比例
    QLabel* iZoom;
    QComboBox*  iZoom_list;
    // 子排版
    QHBoxLayout*    iSubLayout1;

    // 手机面板
    XTouchPopup_Dialog_Pad* iPad;

    // 底端的旋转按钮
    QLabel* iClockwiseButton;
    QLabel* iEasternButton;

    // 子排版
    QHBoxLayout*    iSubLayout2;

    // 左半部分的子排版
    QBoxLayout*    iSubLayout;

    // 可添加的按键组
    QList<XTouchPopup_Dialog_Button*> iButtonBox;

    // 右半部分的子排版
    QGridLayout*    iSubLayout3;

    // 按键组之上的排版
    QBoxLayout* iSubLayout4;

    // Dialog的排版
    QBoxLayout* iLayout;

    // 添加到屏幕上的按键
    QList<XTouchPopup_Dialog_Button*> iButtonAdd;

    // 显示屏的位置
    QRect iScreenRect;

    // 缩放比例
    float iZoomCount;

    // 机型名称
    QList<QString>  iModelName;
    // 手机的尺寸
    QList<QSize>    iModelPicSize;
    // 屏幕的偏移
    QList<QSize>    iModelOffsetSize;
    // 屏幕的尺寸及位置
    QList<QRect>    iModelScreenRect;
    // 机型序号
    int iModelNum;

    // button at
    int iButtonat;

    QDialogButtonBox* iButton;
    QPushButton* iOk;
    QPushButton* iCancel;

    QListWidget* iParent;

    QListWidgetItem* iItem;
};

#endif // XTOUCHPOPUP_DIALOG_H
