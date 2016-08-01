/**/

#include "TabFourth_Preview.h"
#include <QTextEdit>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QTextCodec>
#include <QVBoxLayout>
#include "EasyXml.h"
#include "mainwindow.h"

TabFourth_Preview::TabFourth_Preview(QWidget *parent)
    : QWidget(parent)
{
    iMainWindow = NULL;
    // ÅÅ°æ
    iLayout = new QVBoxLayout;

    // ¹¹ÔìTab
    SetupTab();

    this->setLayout(iLayout);
}

void TabFourth_Preview::SetupTab()
{
    SetupPreview();
    SetupButtonbox();
}

void TabFourth_Preview::SetupPreview()
{
    iPreview = new QTextEdit(this);
    iPreview->setReadOnly(true);
    iPreview->setText(QWidget::tr("Preview Window"));

    iLayout->addWidget(iPreview);
}

void TabFourth_Preview::SetupButtonbox()
{
    iButtonbox = new QDialogButtonBox(this);

    iCreate = new QPushButton(this);
    iCreate->setText(QPushButton::tr("Preview"));
    QObject::connect(iCreate, SIGNAL(clicked()),
                     this, SLOT(SetPreview()));

    iOk = new QPushButton(this);
    iOk->setText(QPushButton::tr("Create"));
    QObject::connect(iOk, SIGNAL(clicked()),
                     this, SLOT(CreateCfg()));

    iButtonbox->addButton(iCreate, QDialogButtonBox::ActionRole);
    iButtonbox->addButton(iOk, QDialogButtonBox::ActionRole);

    iLayout->addWidget(iButtonbox);
}

void TabFourth_Preview::SetPreview()
{
    if(iMainWindow)
    {
        if(iPreview)
        {
            iPreview->clear();
            iPreview->setText(iMainWindow->PreviewContent());
        }
    }
}

void TabFourth_Preview::CreateCfg()
{
    if(iMainWindow)
    {
        iMainWindow->SaveAsForPreview();
    }
}

//void TabFourth_Preview::SetPreview(const QString &str)
//{
//    iPreview->setText(str);
//}

//QString TabFourth_Preview::Preview() const
//{
//    if(iPreview!=NULL)
//    {
//        return iPreview->toPlainText();
//    }
//}

void TabFourth_Preview::Default()
{
    if(iPreview)
        iPreview->clear();
}

void TabFourth_Preview::ReadData(XEasyXml *xml)
{
    if(xml)
    {
        QString value;
        value = xml->GenerateXml();

        if(iPreview)
            iPreview->setText(value);
    }
}
