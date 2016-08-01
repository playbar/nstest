#ifndef XTOUCHPOPUP_DIALOG_POSITION_H
#define XTOUCHPOPUP_DIALOG_POSITION_H

class QLabel;
class QSpinBox;
class QCheckBox;
class QPushButton;
class QDialogButtonBox;
class QBoxLayout;
class XTouchPopup_Dialog_Button;

#include <QDialog>

class XTouchPopup_Dialog_Position : public QDialog
{
    Q_OBJECT
public:
    explicit XTouchPopup_Dialog_Position(QWidget *parent = 0);
    void InitPosition(QRect screenrect, XTouchPopup_Dialog_Button* button);

signals:

public slots:
    void RClicked(bool b);
    void BClicked(bool b);
    void OkClicked();

private:
    void CreateDialog();
    void CreateX();
    void CreateY();
    void CreateButtonBox();

    void InitX();
    void InitY();

private:
    // X坐标
    QLabel* iX;
    QSpinBox*   iXValue;
    // R
    QCheckBox*  iR;

    // Y坐标
    QLabel* iY;
    QSpinBox* iYValue;
    // B
    QCheckBox* iB;

    // 按键
    QDialogButtonBox* iButtonBox;
    QPushButton*        iOk;
    QPushButton*        iCancel;

    QBoxLayout* iLayout;

    QRect iScreenRect;
    XTouchPopup_Dialog_Button* iButton;
};

#endif // XTOUCHPOPUP_DIALOG_POSITION_H
