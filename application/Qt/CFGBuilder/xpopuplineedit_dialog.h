#ifndef XPOPUPLINEEDIT_DIALOG_H
#define XPOPUPLINEEDIT_DIALOG_H

class XPopupLineEdit_Dialog_Label;
class QPushButton;
class QBoxLayout;

#include <QDialog>

class XPopupLineEdit_Dialog : public QDialog
{
    Q_OBJECT
public:
    XPopupLineEdit_Dialog(QWidget *parent = 0);

public:
    void CreateDialog();
    void CreateButtonBox();
    void CreatePicArea();

signals:
    void KeyName(const QString& aKeyName);

public slots:
    void ClickNumPad();
    void ClickFullPad();

    void GetKeyName(const QString& aKeyName) {emit KeyName(aKeyName);}

private:
    // 上方的两个按键
    QPushButton* iNumPad;
    QPushButton* iFullPad;

    // 手机图片的显示区域
    XPopupLineEdit_Dialog_Label* iPicArea;

    // 排版
    QBoxLayout* iLayout;
};

#endif // XPOPUPLINEEDIT_DIALOG_H
