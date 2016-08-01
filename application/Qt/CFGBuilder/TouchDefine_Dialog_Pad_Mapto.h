#ifndef TOUCHDEFINE_DIALOG_PAD_MAPTO_H
#define TOUCHDEFINE_DIALOG_PAD_MAPTO_H

class QLabel;
class QDialogButtonBox;
class XPopupKeyMapTo_LineEdit;
class QPushButton;
class QBoxLayout;
class TouchDefine_Dialog_Pad_Button;

#include <QDialog>

class TouchDefine_Dialog_Pad_Mapto : public QDialog
{
    Q_OBJECT
public:
    explicit TouchDefine_Dialog_Pad_Mapto(QWidget *parent = 0);
    void InitMapto(TouchDefine_Dialog_Pad_Button* button);
signals:

public slots:
    void ClickOK();
    
private:
    void CreateDialog();
    void CreateLineEdit();
    void CreateButtonBox();

    void InitData();
    
private:
    QLabel* iLabel;
    XPopupKeyMapTo_LineEdit* imapto;

    QDialogButtonBox* iButtonBox;
    QPushButton* iOk;
    QPushButton* iCancel;

    QBoxLayout* iLayout;
    
    TouchDefine_Dialog_Pad_Button* iButton;

};

#endif // TOUCHDEFINE_DIALOG_PAD_MAPTO_H
