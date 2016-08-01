#include "SaveDialog.h"
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QFileDialog>

SaveDialog::SaveDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle(tr("SaveDialog"));
    
    iLayout = new QGridLayout;
    
    CreateDialog();
    this->setLayout(iLayout);
}

void SaveDialog::InitDialog(QString path, QString encode)
{
    iPath = path;
    if(iPath.length()!=0)
    {
        iFilePath_Input->setText(QFileInfo(iPath).fileName());
    }
    for(int i=0; i<iEncode_List->count(); i++)
    {
        if(encode==iEncode_List->itemText(i))
        {
            iEncode_List->setCurrentIndex(i);
            break;
        }
    }
}

// slots
// 点击OK
void SaveDialog::slotOKClicked()
{
    if(iPath.length()!=0)
    {
        emit signalFilePath(iPath);
        emit signalEncode(iEncode_List->currentText());
        emit signalSaveFile();
        this->close();
    }
}

// 点击文件路径按键
void SaveDialog::slotFilePathClicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save File Path"), iPath, "CFG(*.cfg)");
    if(path.length()!=0)
    {
        iPath = path;
        iFilePath_Input->setText(QFileInfo(iPath).fileName());
    }
}

void SaveDialog::CreateDialog()
{
    CreateFilePath();
    CreateEncode();
    CreateButtonBox();
}

void SaveDialog::CreateFilePath()
{
    iFilePath = new QLabel(tr("FilePath"), this);
    iFilePath_Input = new QLineEdit(this);
    iFilePath_Input->setReadOnly(true);
    iFilePath_Button = new QPushButton(tr("..."), this);
    iFilePath_Button->setFixedSize(20,20);
    connect(iFilePath_Button, SIGNAL(clicked()),
            this, SLOT(slotFilePathClicked()));
    
    iLayout->addWidget(iFilePath, 0, 0);
    iLayout->addWidget(iFilePath_Input, 0, 1);
    iLayout->addWidget(iFilePath_Button, 0, 2);
}

void SaveDialog::CreateEncode()
{
    iEncode = new QLabel(tr("Encode"), this);
    iEncode_List = new QComboBox(this);
    iEncode_List->addItem(tr("UTF-8"));
    iEncode_List->addItem(tr("GB2312"));
    
    iLayout->addWidget(iEncode, 1, 0);
    iLayout->addWidget(iEncode_List, 1, 1);
}

void SaveDialog::CreateButtonBox()
{
    iButtonBox = new QDialogButtonBox(this);
    iOk = new QPushButton(tr("OK"), iButtonBox);
    connect(iOk, SIGNAL(clicked()),
            this, SLOT(slotOKClicked()));
    iButtonBox->addButton(iOk, QDialogButtonBox::ActionRole);
    
    iCancel = new QPushButton(tr("Cancel"), iButtonBox);
    connect(iCancel, SIGNAL(clicked()),
            this, SLOT(close()));
    iButtonBox->addButton(iCancel, QDialogButtonBox::ActionRole);
    
    iLayout->addWidget(iButtonBox, 2, 1, 1, 2);
}
