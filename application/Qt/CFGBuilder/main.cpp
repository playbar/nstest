#include <QtGui/QApplication>
#include "mainwindow.h"
#include "TabThird_GroupFour_Dialog.h"

#include "EasyXml.h"
#include <QTranslator>
#include "xpopupcontentid_lineedit.h"
#include "xtouchdefine_dialog.h"
#include "xtouchpopup_dialog.h"
#include "xtouchpopup_dialog_position.h"

#include <QToolButton>
#include <QPushButton>
#include "SaveDialog.h"
#include "AccDialog.h"
#include "AccGroup.h"
#include <TabSecond_GroupFive.h>
#include "TouchDefine_Dialog.h"
#include "xtouchpopup_dialog_mapto.h"
#include "xpopupkeymapto_dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator( 0 );
    translator.load( "CFGBuilder_zh.qm", "." );
    a.installTranslator( &translator );

    MainWindow* window = new MainWindow;
    window->show();
    
//    TouchDefine_Dialog* dialog = new TouchDefine_Dialog;
//    dialog->show();
    
//    XTouchPopup_Dialog_Mapto* dialog = new XTouchPopup_Dialog_Mapto;
//    dialog->show();
    
//    XPopupKeyMapTo_Dialog dialog;
//    dialog.show();

    return a.exec();
}
