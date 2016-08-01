#ifndef XTOUCHPOPUP_DIALOG_LONGPRESS_H
#define XTOUCHPOPUP_DIALOG_LONGPRESS_H

class QCheckBox;
class QSpinBox;
class QLabel;
class QBoxLayout;
class QDialogButtonBox;
class QPushButton;
class XTouchPopup_Dialog_Button;
class QHBoxLayout;

#include <QDialog>

class XTouchPopup_Dialog_LongPress : public QDialog
{
    Q_OBJECT
public:
    explicit XTouchPopup_Dialog_LongPress(QWidget *parent = 0);
    void Init(XTouchPopup_Dialog_Button* button);
signals:

public slots:
    void OkClicked();

private:
    void CreateDialog();
    void CreateLongPress();
    void CreateLongPressTime();
    void CreateButtonBox();
    void InitLongPress();
    void InitLongPressTime();

private:
    // LongPress
    QCheckBox* iLongPress;

    // LongPress time
    QLabel* iLongPressTime;
    QSpinBox*   iLongPressTime_box;

    QDialogButtonBox* iButtonBox;
    QPushButton* iOk;
    QPushButton* iCancel;
    QHBoxLayout* isubLayout;

    QBoxLayout* iLayout;

    XTouchPopup_Dialog_Button* iButton;
};

#endif // XTOUCHPOPUP_DIALOG_LONGPRESS_H
