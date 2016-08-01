# Microsoft Developer Studio Project File - Name="AS3AutoMap" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=AS3AutoMap - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AS3AutoMap.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AS3AutoMap.mak" CFG="AS3AutoMap - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AS3AutoMap - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "AS3AutoMap - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AS3AutoMap - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "AS3AutoMap - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
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
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "AS3AutoMap - Win32 Release"
# Name "AS3AutoMap - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AS3AutoMap.cpp
# End Source File
# Begin Source File

SOURCE=.\AS3AutoMap.rc
# End Source File
# Begin Source File

SOURCE=.\AS3AutoMapDlg.cpp
# End Source File
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

SOURCE=.\Register.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TextDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AS3AutoMap.h
# End Source File
# Begin Source File

SOURCE=.\AS3AutoMapDlg.h
# End Source File
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

SOURCE=.\Register.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TextDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\AS3AutoMap.ico
# End Source File
# Begin Source File

SOURCE=.\res\AS3AutoMap.rc2
# End Source File
# Begin Source File

SOURCE=.\res\dirbrown.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icons.bmp
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
# Section AS3AutoMap : {FADA66D6-4620-4A87-B388-48FBD62827DD}
# 	2:13:DirBrowns.cpp:DirBrowns.cpp
# 	2:17:CLASS: CDirBrowns:CDirBrowns
# 	2:19:Application Include:AS3AutoMap.h
# 	2:11:DirBrowns.h:DirBrowns.h
# End Section
# Section AS3AutoMap : {B6617E5A-166E-4841-BE06-1DD5628165ED}
# 	2:13:DriverBox.cpp:DriverBox.cpp
# 	2:11:DriverBox.h:DriverBox.h
# 	2:17:CLASS: CDriverBox:CDriverBox
# 	2:19:Application Include:AS3AutoMap.h
# End Section
# Section AS3AutoMap : {000889DA-0523-4901-875F-2520D103EAAC}
# 	2:12:Register.cpp:Register.cpp
# 	2:16:CLASS: CRegister:CRegister
# 	2:19:Application Include:AS3AutoMap.h
# 	2:10:Register.h:Register.h
# End Section
# Section AS3AutoMap : {54E6D2C2-4091-4199-A593-589E5CCCE5C1}
# 	2:15:CLASS: CDirTree:CDirTree
# 	2:11:DirTree.cpp:DirTree.cpp
# 	2:9:DirTree.h:DirTree.h
# 	2:19:Application Include:AS3AutoMap.h
# End Section
