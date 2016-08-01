#ifndef CURDTDVIEW_H
#define CURDTDVIEW_H

class QPushButton;
class QVBoxLayout;
class QTextEdit;

#include <QDialog>

class CurDtdView : public QDialog
{
    Q_OBJECT
public:
    explicit CurDtdView(QWidget *parent = 0);


private:
    void SetupDialog();

signals:

public slots:

private:
    // 显示部分
    QTextEdit*  iShow;

    // 关闭案件
    QPushButton* iCloseDialog;

    // 排版
    QVBoxLayout* iLayout;

};

#endif // CURDTDVIEW_H
