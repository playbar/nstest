/*
 * Builder的整体框架
 */
#include "mainwindow.h"
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QTextStream>
#include <QIODevice>
#include <QFile>
#include <QMessageBox>
#include <QPushButton>
#include "TabFrame.h"
#include "EasyXml.h"
#include "SaveDialog.h"
#include <QTextCodec>
#include "CurDtdView.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    iEngine = new XEasyXml;
    iEncode = "UTF-8";

    this->setWindowTitle(QMainWindow::tr("CFGBuilder"));
    this->setMinimumSize(640, 480);
    this->setMaximumSize(640, 480);
    this->setWindowIcon(QIcon(":image/logo/xiaologo.gif"));

    CreateMenu();
    CreateFrame();
}

MainWindow::~MainWindow()
{
    SAFE_DEL(iEngine);
}

void MainWindow::CreateMenu()
{
    CreateMenu_File();
    CreateMenu_Set();
    CreateMenu_Help();
}

void MainWindow::CreateMenu_File()
{
    iFileMenu = this->menuBar()->addMenu(tr("File"));

    iFileCreate = new QAction(tr("New"), this);
//    iFileCreate->setShortcut(tr("Ctrl+N"));
    iFileMenu->addAction(iFileCreate);
    this->connect(iFileCreate, SIGNAL(triggered()),
                  this, SLOT(CreateFile()));

    iFileMenu->addSeparator();

    iFileOpen = new QAction(tr("Open"), this);
    iFileOpen->setShortcut(tr("Ctrl+O"));
    iFileMenu->addAction(iFileOpen);
    this->connect(iFileOpen, SIGNAL(triggered()),
                  this, SLOT(OpenFile()));

    iFileSave = new QAction(tr("Save"), this);
    iFileSave->setShortcut(tr("Ctrl+S"));
    iFileMenu->addAction(iFileSave);
    this->connect(iFileSave, SIGNAL(triggered()),
                  this, SLOT(SaveFile()));

    iFileSaveAs = new QAction(tr("SaveAs"), this);
    iFileSaveAs->setShortcut(tr("Ctrl+A"));
    iFileMenu->addAction(iFileSaveAs);
    this->connect(iFileSaveAs, SIGNAL(triggered()),
                  this, SLOT(SaveAs()));

    iFileMenu->addSeparator();

    iClose = new QAction(tr("Close"), this);
//    iClose->setShortcut(tr("Ctrl+C"));
    iFileMenu->addAction(iClose);
    this->connect(iClose, SIGNAL(triggered()),
                  this, SLOT(CloseFile()));


    iFileMenu->addSeparator();

    iExit = new QAction(tr("Quit"), this);
//    iExit->setShortcut(tr("Ctrl+Q"));
    iFileMenu->addAction(iExit);
    this->connect(iExit, SIGNAL(triggered()),
                  this, SLOT(close()));
}

void MainWindow::CreateMenu_Set()
{
    iSetMenu = this->menuBar()->addMenu(tr("Options"));

    // 后续添加内容，需要添加生成器的一些配置设置
}

void MainWindow::CreateMenu_Help()
{
    iHelpMenu = this->menuBar()->addMenu(tr("Help"));

    iHelpDoc = new QAction(tr("Index"), this);
    iHelpMenu->addAction(iHelpDoc);
    // 后续添加内容，需要打开一个帮助文档

    iCurDtd = new QAction(tr("DTD"), this);
    iHelpMenu->addAction(iCurDtd);
    this->connect(iCurDtd, SIGNAL(triggered()),
                  this, SLOT(CurDTD()));

    iHelpMenu->addSeparator();

    iAbout = new QAction(tr("About"), this);
    iHelpMenu->addAction(iAbout);
    this->connect(iAbout, SIGNAL(triggered()),
                  this, SLOT(About()));
    // 后续添加内容，需要打开一个关于生成器的自我介绍
}

void MainWindow::CreateFrame()
{
    iTabFrame = new TabFrame(this);
    
    connect(iTabFrame, SIGNAL(signalUpdatePathItoO(QString)),
            this, SLOT(slotUpdatePathItoO(QString)));
    
    connect(this, SIGNAL(signalUpdatePathOtoI(QString)),
            iTabFrame, SLOT(slotUpdatePathOtoI(QString)));
//    connect(this, SIGNAL(signalUpdatePath(QString)),
//            iTabFrame, SLOT(slotUpdatePath(QString)));
    
//    connect(iTabFrame, SIGNAL(signalUpdatePath(QString)),
//            this, SLOT(slotUpdatePath(QString)));
//    iTabFrame->SaveData(iEngine);

    this->setCentralWidget(iTabFrame);
    iTabFrame->SetMainWindowForPreview(this);
}

void MainWindow::CreateFile()
{
    iPath.clear();

    emit signalUpdatePathOtoI(iPath);
    
    this->setWindowTitle(QMainWindow::tr("CFGBuilder"));

    if(iEngine)
        SAFE_DEL(iEngine);
    iEngine = new XEasyXml;
    iTabFrame->SaveData(iEngine);

    if(iTabFrame)
        iTabFrame->Default();
}

void MainWindow::OpenFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("OpenFile"),
                                                iPath, tr("CFG(*.cfg)"));
    if(path.length()!=0)
    {
        iPath = path;
        QString tmp;
        tmp = iPath;
        tmp.mid(0, tmp.lastIndexOf("."));
        
        emit signalUpdatePathOtoI(tmp);
        
        QString title = tr("CFGBuilder");
        title += " (";
        title += iPath;
        title += ")";
        this->setWindowTitle(title);

        if(iEngine)
            SAFE_DEL(iEngine);

        iEngine = new XEasyXml;
        if(iEngine)
        {
            iEngine->OpenXml(iPath);
            iEncode = iEngine->Encoding();
//            iEncodeOld = iEncode;
        }

        if(iTabFrame)
        {
            iTabFrame->Default();
            iTabFrame->ReadData(iEngine);
        }
    }
}

void MainWindow::SaveFile()
{
    if(iPath.length()==0)
    {
//        iPath = QFileDialog::getSaveFileName(this, tr("Save"),
//                                             iPath, tr("CFG(*.cfg)"));
        SaveDialog* sd = new SaveDialog(this);
        sd->InitDialog(iPath, iEncode);
        connect(sd, SIGNAL(signalFilePath(QString)),
                this, SLOT(slotChangeFilePath(QString)));
        connect(sd, SIGNAL(signalEncode(QString)),
                this, SLOT(slotChangeEncode(QString)));
        connect(sd, SIGNAL(signalSaveFile()),
                this, SLOT(slotSaveFile()));
        sd->show();
    }
    if(iPath.length()!=0)
    {
        QString title = tr("CFGBuilder");
        title += " (";
        title += iPath;
        title += ")";
        this->setWindowTitle(title);

        QFile file(iPath);
        if(file.exists())
            file.remove();

        if(file.open(QIODevice::WriteOnly))
        {
            // 文件成功打开
            QTextStream writer(&file);
            QString str;
            if(iEngine!=NULL)
            {
                // 应该保存参数
                if(iTabFrame)
                    iTabFrame->SaveData(iEngine);
                
                iEngine->SetEncoding(iEncode);
                if(iEncode=="GB2312")
                {
                    writer.setCodec("GB2312");
                }
                else
                {
                    writer.setCodec("UTF-8");
                    QByteArray by1;
                    by1.resize(3);
                    by1[0] = 0xEF;
                    by1[1] = 0xBB;
                    by1[2] = 0xBF;
                    file.write(by1);
                }
                str = iEngine->GenerateXml();
            }
            
            writer.operator <<(str);

            file.close();
        }
    }
}

void MainWindow::SaveAs()
{
    SaveDialog* sd = new SaveDialog(this);
    sd->InitDialog(iPath, iEncode);
    connect(sd, SIGNAL(signalFilePath(QString)),
            this, SLOT(slotChangeFilePath(QString)));
    connect(sd, SIGNAL(signalEncode(QString)),
            this, SLOT(slotChangeEncode(QString)));
    connect(sd, SIGNAL(signalSaveFile()),
            this, SLOT(slotSaveFile()));
    sd->show();
}

void MainWindow::CloseFile()
{
    iPath.clear();
    
    emit signalUpdatePathOtoI(iPath);

    this->setWindowTitle(tr("CFGBuilder"));

    if(iEngine)
        SAFE_DEL(iEngine);
    iEngine = new XEasyXml;

    if(iTabFrame)
        iTabFrame->Default();
}

void MainWindow::About()
{
    QMessageBox msg;

    msg.setText(QMessageBox::tr("CFGBuilder Version 0.91\nDTD Version 20111009\nBuild Time 2011-10-17"));

    msg.exec();
}

void MainWindow::CurDTD()
{
    CurDtdView* view = new CurDtdView(this);
    view->exec();
}

void MainWindow::slotUpdatePathItoO(QString path)
{
    iPath = path;
    iPath += ".cfg";
    QString title = tr("CFGBuilder");
    title += " (";
    title += iPath;
    title += ")";
    this->setWindowTitle(title);
}

QString MainWindow::PreviewContent() const
{
    if(iTabFrame)
    {
        if(iEngine)
        {
            iTabFrame->SaveData(iEngine);
            return iEngine->GenerateXml();
        }
    }
}
