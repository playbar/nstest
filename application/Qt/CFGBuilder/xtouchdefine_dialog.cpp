//#include "xtouchdefine_dialog.h"
//#include <QBoxLayout>
//#include <QLabel>
//#include <QLineEdit>
//#include <QComboBox>
//#include <QHBoxLayout>

//XTouchDefine_Dialog::XTouchDefine_Dialog(QWidget *parent) :
//    QDialog(parent)
//{
//    iLayout = new QBoxLayout(QBoxLayout::TopToBottom);

//    CreateDialog();
//    this->setLayout(iLayout);
//}

//void XTouchDefine_Dialog::CreateDialog()
//{
//    CreateTouchName();
//    CreateTouchMode();
//    CreateAreaZoom();
//    CreateTouchArea();
//    CreateTouchButton();
//    CreateButtonBox();
//}

//void XTouchDefine_Dialog::CreateTouchName()
//{
//    iTouchName = new QLabel;
//    iTouchName->setText(tr("Name"));

//    iTouchName_input = new QLineEdit;

//    iSubLayout = new QHBoxLayout;
//    iSubLayout->addWidget(iTouchName);
//    iSubLayout->addWidget(iTouchName_input);
//}

//void XTouchDefine_Dialog::CreateTouchMode()
//{
//    iTouchMode = new QLabel;
//    iTouchMode->setText(tr("Mode"));

//    iTouchMode_list = new QComboBox;
//    iTouchMode_list->addItem(tr("NOKIA 5800"));

//    iTouchMode_list->setCurrentIndex(0);

//    iSubLayout->addWidget(iTouchMode);
//    iSubLayout->addWidget(iTouchMode_list);

////    iLayout->addLayout(iSubLayout);
//}

//void XTouchDefine_Dialog::CreateAreaZoom()
//{
//    iAreaZoom = new QLabel;
//    iAreaZoom->setText(tr("Zoom"));

//    iAreaZoom_list = new QComboBox;
//    iAreaZoom_list->addItem(tr("100%"));
//    iAreaZoom_list->addItem(tr(" 75%"));
//    iAreaZoom_list->addItem(tr(" 50%"));
//    iAreaZoom_list->setCurrentIndex(0);

//    iSubLayout->addWidget(iAreaZoom);
//    iSubLayout->addWidget(iAreaZoom_list);

//    iLayout->addLayout(iSubLayout);
//}

//void XTouchDefine_Dialog::CreateTouchArea()
//{}

//void XTouchDefine_Dialog::CreateTouchButton()
//{}

//void XTouchDefine_Dialog::CreateButtonBox()
//{}
