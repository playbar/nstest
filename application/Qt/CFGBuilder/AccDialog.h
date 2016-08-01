/*
 * 重力感应配置对话框
 */
#ifndef ACCDIALOG_H
#define ACCDIALOG_H

class QLabel;
class QSpinBox;
class QComboBox;
class QGridLayout;
class QPushButton;
class QDialogButtonBox;
class QRadioButton;
class QListWidget;
class QListWidgetItem;
class QLineEdit;
class XPopupKeyMapTo_LineEdit;

#include <QDialog>

class AccDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AccDialog(QWidget *parent = 0);

    
    // 修改ITEM
    void ModifyItem_Item(QListWidget* parent, QListWidgetItem* item = NULL);
    
signals:

public slots:
    // Mapto Check slot
    void slotMaptoChecked(bool);
    // MouseAct Check slot
    void slotMouseActChecked(bool);
    // 点击OK
    void slotOKClicked();
    
private:
    void CreateDialog();
    void CreateDirection();
    void CreateAccSpeed();
    void CreateStep();
    void CreateMapto();
    void CreateMouseAct();
    void CreateOnlyOne();
    void CreateButtonBox();
    
private:
    // 方向
    QLabel* iDirection;
    QComboBox* iDirection_List;
    
    // accspeed
    QLabel* iAccSpeed;
    QLineEdit* iAccSpeed_input;
    
    // step
    QLabel* iStep;
    QLineEdit* iStep_input;
    
    // 映射至
    QRadioButton* iMapto;
    XPopupKeyMapTo_LineEdit* iMapto_Input;
    
    // 鼠标动作
    QRadioButton* iMouseAct;
    QComboBox* iMouseAct_List;
    
    // OnlyOne
    QLabel* iOnlyOne;
    QComboBox* iOnlyOne_List;
    
    // 按键组
    QDialogButtonBox* iButtonBox;
    QPushButton* iOk;
    QPushButton* iCancel;
    
    // 排版
    QGridLayout* iLayout;
    
    // list points
    QListWidget* iParent;
    
    // item points
    QListWidgetItem* iItem;
};

#endif // ACCDIALOG_H
