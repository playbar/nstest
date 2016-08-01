#include "CurDtdView.h"
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>
#include <QPushButton>

CurDtdView::CurDtdView(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle(tr("Current DTD"));
    this->setMinimumSize(640, 480);

    iLayout = new QVBoxLayout;

    SetupDialog();

    this->setLayout(iLayout);
}

void CurDtdView::SetupDialog()
{
    iShow = new QTextEdit(this);
    iShow->setReadOnly(true);
    iLayout->addWidget(iShow);

    // load
    QString content;
    QString path = "config_20111009.dtd";

    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        content = stream.readAll();

        file.close();
    }
    else
    {
        content += path;
        content += " not find";
    }
    // load

    iShow->setText(content);

    iCloseDialog = new QPushButton(tr("Close"), this);
    iLayout->addWidget(iCloseDialog);
    connect(iCloseDialog, SIGNAL(clicked()),
            this, SLOT(close()));
}

