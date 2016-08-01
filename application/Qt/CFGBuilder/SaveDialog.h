#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

class QLabel;
class QLineEdit;
class QDialogButtonBox;
class QPushButton;
class QComboBox;
class QGridLayout;

#include <QDialog>

class SaveDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SaveDialog(QWidget *parent = 0);
    void InitDialog(QString path, QString encode);

signals:
    // 用户选择的文件路径
    void signalFilePath(QString path);
    // 用户选择的编码方式
    void signalEncode(QString encode);
    // 请求保存文件
    void signalSaveFile();
    

public slots:
    // 点击OK
    void slotOKClicked();
    // 点击文件路径按键
    void slotFilePathClicked();
    
private:
    void CreateDialog();
    void CreateFilePath();
    void CreateEncode();
    void CreateButtonBox();
    
private:
    // 文件名
    QLabel* iFilePath;
    QLineEdit* iFilePath_Input;
    QPushButton* iFilePath_Button;
    
    // 编码方式
    QLabel* iEncode;
    QComboBox* iEncode_List;
    
    // 按键组
    QDialogButtonBox* iButtonBox;
    QPushButton* iOk;
    QPushButton* iCancel;
    
    // 排版方式
    QGridLayout* iLayout;
    
    // 缓存下路径，方便提取文件名
    QString iPath;
};

#endif // SAVEDIALOG_H
