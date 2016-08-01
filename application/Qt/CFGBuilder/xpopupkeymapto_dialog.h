#ifndef XPOPUPKEYMAPTO_DIALOG_H
#define XPOPUPKEYMAPTO_DIALOG_H

class XPopupKeyMapTo_Dialog_Label;
class QVBoxLayout;

#include <QDialog>

class XPopupKeyMapTo_Dialog : public QDialog
{
    Q_OBJECT
public:
    XPopupKeyMapTo_Dialog(QWidget *parent = 0);

public:
    void CreateDialog();
    void CreatePicArea();

signals:
    void KeyCode(const QString& aKeyCode);

public slots:
    void GetKeyCode(const QString& aKeyCode) {emit KeyCode(aKeyCode);}

private:
    // 图片显示位置
    XPopupKeyMapTo_Dialog_Label* iPicArea;

    // 排版
    QVBoxLayout* iLayout;
};

#endif // XPOPUPKEYMAPTO_DIALOG_H
