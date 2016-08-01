# Microsoft Developer Studio Project File - Name="GGClient" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=GGClient - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GGClient.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GGClient.mak" CFG="GGClient - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GGClient - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "GGClient - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GGClient - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\Tmp\Win32\GGClient\Release"
# PROP Intermediate_Dir "..\..\..\Tmp\Win32\GGClient\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /I "..\..\..\Include\Base" /I "..\..\..\Include\Dom" /I "..\..\..\Include\Flash" /I "..\..\..\Resource\Include" /D "__TOUCHPAD__" /D "_ANALY" /D "_WINEMU" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\..\..\Lib\Win32\Release\GGDom.lib ..\..\..\Lib\Win32\Release\GGBase.lib ..\..\..\Lib\Win32\Release\GGFlash.lib ..\..\..\dependency\mp3decoder\lib\mp3decoder.lib ..\..\..\dependency\GDIPlus\lib\GDIPlus.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"..\..\..\Bin\Win32\Release\GGClient.exe"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "GGClient - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\Tmp\Win32\GGClient\Debug"
# PROP Intermediate_Dir "..\..\..\Tmp\Win32\GGClient\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\Include\Dom" /I "..\..\..\Include\Flash" /I "..\..\..\Include\Base" /I "..\..\..\Resource\Include" /D "__TOUCHPAD__" /D "_ANALY" /D "_WINEMU" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "_SOURCECODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\..\..\Lib\Win32\Debug\GGDom.lib ..\..\..\Lib\Win32\Debug\GGBase.lib ..\..\..\Lib\Win32\Debug\GGFlash.lib ..\..\..\dependency\mp3decoder\lib\mp3decoder.lib ..\..\..\dependency\GDIPlus\lib\GDIPlus.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"..\..\..\Bin\Win32\Debug\GGClient.exe" /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "GGClient - Win32 Release"
# Name "GGClient - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AnalyDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorBut.cpp
# End Source File
# Begin Source File

SOURCE=.\Dom.cpp
# End Source File
# Begin Source File

SOURCE=.\GGClient.cpp
# End Source File
# Begin Source File

SOURCE=.\GGClient.rc
# End Source File
# Begin Source File

SOURCE=.\GGClientDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphButton.cpp
# End Source File
# Begin Source File

SOURCE=.\IconBut.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\MobileView.cpp
# End Source File
# Begin Source File

SOURCE=.\SetupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AnalyDialog.h
# End Source File
# Begin Source File

SOURCE=.\ColorBut.h
# End Source File
# Begin Source File

SOURCE=.\Dom.h
# End Source File
# Begin Source File

SOURCE=.\GGClient.h
# End Source File
# Begin Source File

SOURCE=.\GGClientDlg.h
# End Source File
# Begin Source File

SOURCE=.\GraphButton.h
# End Source File
# Begin Source File

SOURCE=.\IconBut.h
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\MobileView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SetupDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\connect.gif
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00003.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor.gif
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\dat1.bin
# End Source File
# Begin Source File

SOURCE=.\res\disk.gif
# End Source File
# Begin Source File

SOURCE=.\res\domlist.bmp
# End Source File
# Begin Source File

SOURCE=.\res\er.gif
# End Source File
# Begin Source File

SOURCE=.\res\failed.gif
# End Source File
# Begin Source File

SOURCE=.\res\file.gif
# End Source File
# Begin Source File

SOURCE=.\res\flash.gif
# End Source File
# Begin Source File

SOURCE=.\res\folder.gif
# End Source File
# Begin Source File

SOURCE=.\res\GGClient.ico
# End Source File
# Begin Source File

SOURCE=.\res\GGClient.rc2
# End Source File
# Begin Source File

SOURCE=.\res\gogo.gif
# End Source File
# Begin Source File

SOURCE=.\res\home.gif
# End Source File
# Begin Source File

SOURCE=.\res\icon.gif
# End Source File
# Begin Source File

SOURCE=.\res\image.gif
# End Source File
# Begin Source File

SOURCE=.\res\keya1.bin
# End Source File
# Begin Source File

SOURCE=.\res\keyb1.bin
# End Source File
# Begin Source File

SOURCE=".\res\loading[1].gif"
# End Source File
# Begin Source File

SOURCE=.\res\media.gif
# End Source File
# Begin Source File

SOURCE=.\res\mobile.gif
# End Source File
# Begin Source File

SOURCE=.\res\pdf.gif
# End Source File
# Begin Source File

SOURCE=.\res\rar.gif
# End Source File
# Begin Source File

SOURCE=.\res\select.gif
# End Source File
# Begin Source File

SOURCE=.\res\text.gif
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\wml.gif
# End Source File
# Begin Source File

SOURCE=.\res\word.gif
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\37hui.png
# End Source File
# Begin Source File

SOURCE=.\res\37lv.png
# End Source File
# Begin Source File

SOURCE=.\res\a_on.png
# End Source File
# Begin Source File

SOURCE=.\res\b_on.png
# End Source File
# Begin Source File

SOURCE=.\res\boff.png
# End Source File
# Begin Source File

SOURCE=.\res\bon.png
# End Source File
# Begin Source File

SOURCE=.\res\config.wml
# End Source File
# Begin Source File

SOURCE=.\res\config.xml
# End Source File
# Begin Source File

SOURCE=.\res\cursor.png
# End Source File
# Begin Source File

SOURCE=.\res\down_on.png
# End Source File
# Begin Source File

SOURCE=.\res\flashcfg.xml
# End Source File
# Begin Source File

SOURCE=.\res\left_on.png
# End Source File
# Begin Source File

SOURCE=.\res\lefts.png
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\right_on.png
# End Source File
# Begin Source File

SOURCE=.\res\rights.png
# End Source File
# Begin Source File

SOURCE=.\res\select.png
# End Source File
# Begin Source File

SOURCE=.\res\sorry.swf
# End Source File
# Begin Source File

SOURCE=.\res\uiconfig.txt
# End Source File
# Begin Source File

SOURCE=.\res\uiconfig.xml
# End Source File
# Begin Source File

SOURCE=.\res\up_on.png
# End Source File
# End Target
# End Project
# Section GGClient : {78414D0F-A143-4741-A074-EE473A67050D}
# 	1:9:IDD_SETUP:104
# 	2:16:Resource Include:resource.h
# 	2:16:CLASS: CSetupDlg:CSetupDlg
# 	2:9:IDD_SETUP:IDD_SETUP
# 	2:10:ENUM: enum:enum
# 	2:12:SetupDlg.cpp:SetupDlg.cpp
# 	2:10:SetupDlg.h:SetupDlg.h
# 	2:19:Application Include:GGClient.h
# End Section
# Section GGClient : {378A6276-623E-4CD6-9E4D-012726BBF249}
# 	2:12:ColorBut.cpp:ColorBut.cpp
# 	2:10:ColorBut.h:ColorBut.h
# 	2:19:Application Include:GGClient.h
# 	2:16:CLASS: CColorBut:CColorBut
# End Section
# Section GGClient : {8582D2E0-E983-4427-A948-D5B6FF7DC9D2}
# 	2:20:TYPEDEF: CONNECTINFO:CONNECTINFO
# 	2:19:CLASS: _CONNECTINFO:_CONNECTINFO
# 	2:10:XConnect.h:XConnect.h
# 	2:19:CLASS: XConnectHttp:XConnectHttp
# 	2:21:TYPEDEF: RESPONSEINFO:RESPONSEINFO
# 	2:10:ENUM: enum:enum
# 	2:19:TYPEDEF: CLIENTINFO:CLIENTINFO
# 	2:18:CLASS: _CLIENTINFO:_CLIENTINFO
# 	2:15:CLASS: XConnect:XConnect
# 	2:19:Application Include:GGClient.h
# 	2:12:XConnect.cpp:XConnect.cpp
# 	2:20:CLASS: _RESPONSEINFO:_RESPONSEINFO
# End Section
# Section GGClient : {BA5ACF50-23EF-4340-A405-4A1338BAE296}
# 	2:15:CLASS: CIconBut:CIconBut
# 	2:19:Application Include:GGClient.h
# 	2:11:IconBut.cpp:IconBut.cpp
# 	2:9:IconBut.h:IconBut.h
# End Section
# Section GGClient : {2EB19769-52DD-413E-802C-858C84806719}
# 	2:10:toolbar1.h:toolbar1.h
# 	2:12:toolbar1.cpp:toolbar1.cpp
# 	2:19:CLASS: CStandardBar:CStandardBar
# 	2:19:Application Include:GGClient.h
# End Section
