; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSWFEditDlg
LastTemplate=CButton
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SWFEdit.h"

ClassCount=4
Class1=CSWFEditApp
Class2=CSWFEditDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CSWFShow
Resource3=IDD_SWFEDIT_DIALOG

[CLS:CSWFEditApp]
Type=0
HeaderFile=SWFEdit.h
ImplementationFile=SWFEdit.cpp
Filter=N

[CLS:CSWFEditDlg]
Type=0
HeaderFile=SWFEditDlg.h
ImplementationFile=SWFEditDlg.cpp
Filter=D
LastObject=IDC_ETREE
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=SWFEditDlg.h
ImplementationFile=SWFEditDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SWFEDIT_DIALOG]
Type=1
Class=CSWFEditDlg
ControlCount=17
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EFILE,edit,1350633600
Control3=IDC_EGET,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_RFILE,edit,1350633600
Control6=IDC_RGET,button,1342242816
Control7=IDC_STATIC,static,1342308352
Control8=IDC_ETREE,SysTreeView32,1350631447
Control9=IDC_RTREE,SysTreeView32,1350631447
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_ESHOW,button,1342242827
Control13=IDC_STATIC,static,1342308352
Control14=IDC_RSHOW,button,1342242827
Control15=IDC_REPLACE,button,1342242816
Control16=IDC_RESTORE,button,1342242816
Control17=IDC_PACKAGE,button,1342242816

[CLS:CSWFShow]
Type=0
HeaderFile=SWFShow.h
ImplementationFile=SWFShow.cpp
BaseClass=CButton
Filter=W
VirtualFilter=BWC

