# Microsoft Developer Studio Project File - Name="SBMakeRes" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SBMakeRes - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SBMakeRes.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SBMakeRes.mak" CFG="SBMakeRes - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SBMakeRes - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SBMakeRes - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SBMakeRes - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\Tmp\Win32\SBMakeRes\Release"
# PROP Intermediate_Dir "..\..\..\Tmp\Win32\SBMakeRes\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"..\..\..\Bin\Win32\Release\SBMakeRes.exe"

!ELSEIF  "$(CFG)" == "SBMakeRes - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\Tmp\Win32\SBMakeRes\Debug"
# PROP Intermediate_Dir "..\..\..\Tmp\Win32\SBMakeRes\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\..\Bin\Win32\Debug\SBMakeRes.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SBMakeRes - Win32 Release"
# Name "SBMakeRes - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DirBrowns.cpp
# End Source File
# Begin Source File

SOURCE=.\DirTree.cpp
# End Source File
# Begin Source File

SOURCE=.\DriverBox.cpp
# End Source File
# Begin Source File

SOURCE=.\FontSample.cpp
# End Source File
# Begin Source File

SOURCE=.\Register.cpp
# End Source File
# Begin Source File

SOURCE=.\SBMakeRes.cpp
# End Source File
# Begin Source File

SOURCE=.\SBMakeRes.rc
# End Source File
# Begin Source File

SOURCE=.\SBMakeResDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\VGFontCreate.cpp
# End Source File
# Begin Source File

SOURCE=.\XMutex.cpp
# End Source File
# Begin Source File

SOURCE=.\XSort.cpp
# End Source File
# Begin Source File

SOURCE=.\XString.cpp
# End Source File
# Begin Source File

SOURCE=.\XVector.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DirBrowns.h
# End Source File
# Begin Source File

SOURCE=.\DirTree.h
# End Source File
# Begin Source File

SOURCE=.\DriverBox.h
# End Source File
# Begin Source File

SOURCE=.\FontSample.h
# End Source File
# Begin Source File

SOURCE=.\Register.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SBMakeRes.h
# End Source File
# Begin Source File

SOURCE=.\SBMakeResDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\VGFontCreate.h
# End Source File
# Begin Source File

SOURCE=.\XMutex.h
# End Source File
# Begin Source File

SOURCE=.\XSort.h
# End Source File
# Begin Source File

SOURCE=.\XString.h
# End Source File
# Begin Source File

SOURCE=.\XVector.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\dirbrown.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SBMakeRes.ico
# End Source File
# Begin Source File

SOURCE=.\res\SBMakeRes.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Vp3000Doc.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section SBMakeRes : {150D71B9-2F7F-45A3-A71A-4FEA5237BBBE}
# 	2:19:CLASS: VGFontSample:VGFontSample
# 	2:12:FontSample.h:FontSample.h
# 	2:14:FontSample.cpp:FontSample.cpp
# 	2:19:Application Include:sbmakeres.h
# End Section
