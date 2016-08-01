#ifndef XTOUCHPOPUP_DIALOG_MAPTO_H
#define XTOUCHPOPUP_DIALOG_MAPTO_H

class XPopupKeyMapTo_LineEdit;
class QDialogButtonBox;
class QPushButton;
class QBoxLayout;
class QLabel;
class XTouchPopup_Dialog_Button;
class TouchDefine_Dialog_Pad_Button;

#include <QDialog>

class XTouchPopup_Dialog_Mapto : public QDialog
{
    Q_OBJECT
public:
    explicit XTouchPopup_Dialog_Mapto(QWidget *parent = 0);
    void InitMapto(XTouchPopup_Dialog_Button* button);
    void InitMapto(TouchDefine_Dialog_Pad_Button* button);

signals:

public slots:
    void ClickOK();

private:
    void CreateDialog();
    void CreateLineEdit();
    void CreateButtonBox();

    void InitData();
    void InitDataNew();

private:
    QLabel* iLabel;
    XPopupKeyMapTo_LineEdit* imapto;

    QDialogButtonBox* iButtonBox;
    QPushButton* iOk;
    QPushButton* iCancel;

    QBoxLayout* iLayout;

    XTouchPopup_Dialog_Button* iButton;
    
    TouchDefine_Dialog_Pad_Button* iButtonNew;
};

#endif // XTOUCHPOPUP_DIALOG_MAPTO_H
