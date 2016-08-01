# Microsoft Developer Studio Project File - Name="GGBase" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=GGBase - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GGBase.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GGBase.mak" CFG="GGBase - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GGBase - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "GGBase - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GGBase - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Tmp\Win32\GGBase\Release"
# PROP Intermediate_Dir "..\..\..\..\Tmp\Win32\GGBase\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O1 /I "..\..\..\..\Include\Base" /I "..\..\..\..\dependency\mad\include" /I "..\..\..\..\dependency\mp3decoder\include" /I "..\..\..\..\dependency\gdiplus\include" /I "..\..\..\..\Resource\Include" /D "_ANALY" /D "_WINEMU" /D "WIN32" /D "MDEBUG" /D "_UNICODE" /D "_LIB" /D "HAVE_CONFIG_H" /D "ASO_ZEROCHECK" /D "FPM_DEFAULT" /FR /YX"stdafxggbase.h" /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\Lib\Win32\Release\GGBase.lib"

!ELSEIF  "$(CFG)" == "GGBase - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\Tmp\Win32\GGBase\Debug"
# PROP Intermediate_Dir "..\..\..\..\Tmp\Win32\GGBase\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\..\..\Include\Base" /I "..\..\..\..\dependency\mad\include" /I "..\..\..\..\dependency\mp3decoder\include" /I "..\..\..\..\dependency\gdiplus\include" /I "..\..\..\..\Resource\Include" /I "..\..\..\..\dependency\loki-0.1.7\include" /D "_ANALY" /D "_WINEMU" /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "_LIB" /D "HAVE_CONFIG_H" /D "ASO_ZEROCHECK" /D "FPM_DEFAULT" /FR /YX"stdafxggbase.h" /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\Lib\Win32\Debug\GGBase.lib"

!ENDIF 

# Begin Target

# Name "GGBase - Win32 Release"
# Name "GGBase - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\src\bit.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\src\decoder.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\src\fixed.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\src\frame.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\win32\GGBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\src\huffman.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\src\layer12.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\src\layer3.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\StdAfxGGBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\src\stream.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\src\synth.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\src\timer.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\src\version.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\XButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\XData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\XDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\XDib.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\XEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\XFastAlloc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\win32\XFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\win32\XGraphics.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\XHScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\win32\XImage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\XListBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\XMenu.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\XMp3Decomp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\win32\XMutex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\win32\XNativeSoundMix.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\XResource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\XScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\win32\XSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\XSort.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\XStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\win32\XString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\XTabControl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\win32\XTime.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\XVector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\XVScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\XWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\XZLib.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\include\bit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\include\config.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\include\decoder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\include\fixed.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\include\frame.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\GGBase.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\GGType.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\include\global.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\include\huffman.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\include\layer12.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\include\layer3.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\include\mad.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\StdAfxGGBase.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\include\stream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\include\synth.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\include\timer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\dependency\mad\include\version.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XButton.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XData.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XDib.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XFastAlloc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XFile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XGraphics.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XHScrollBar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XImage.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XListBox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XMenu.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XMp3Decomp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XMutex.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XNativeSoundMix.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XResource.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XScrollBar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XSocket.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XSort.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XStream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XString.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XTabControl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XTime.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XVector.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XVScrollBar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XWindow.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XZLib.h
# End Source File
# End Group
# End Target
# End Project
# Section GGBase : {4C76D855-4E36-4099-83E9-32BC35497BC5}
# 	2:24:TYPEDEF: StreamBlockProc:StreamBlockProc
# 	2:17:CLASS: WaveHeader:WaveHeader
# 	2:13:CLASS: CSound:CSound
# 	2:25:CLASS: SoundEnvelopePoint:SoundEnvelopePoint
# 	2:20:CLASS: CSoundChannel:CSoundChannel
# 	2:16:CLASS: CSoundMix:CSoundMix
# 	2:8:sndmix.h:sndmix.h
# 	2:10:ENUM: enum:enum
# 	2:10:sndmix.cpp:sndmix.cpp
# 	2:20:CLASS: SoundEnvelope:SoundEnvelope
# 	2:19:Application Include:GGBase.h
# 	2:18:FUNC: ConvertSound:ConvertSound
# End Section
