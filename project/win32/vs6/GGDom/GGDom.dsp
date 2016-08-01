# Microsoft Developer Studio Project File - Name="GGDom" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=GGDom - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GGDom.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GGDom.mak" CFG="GGDom - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GGDom - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "GGDom - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GGDom - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Tmp\Win32\GGDom\Release"
# PROP Intermediate_Dir "..\..\..\..\Tmp\Win32\GGDom\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O1 /I "..\..\..\..\Include\Base" /I "..\..\..\..\Include\Dom" /I "..\..\..\..\Include\Flash" /I "..\..\..\..\Resource\Include" /D "__TOUCHPAD__" /D "_ANALY" /D "_WINEMU" /D "WIN32" /D "NDEBUG" /D "_UNICODE" /D "_LIB" /FR /YX"stdafxggdom.h" /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\Lib\Win32\Release\GGDom.lib"

!ELSEIF  "$(CFG)" == "GGDom - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\Tmp\Win32\GGDom\Debug"
# PROP Intermediate_Dir "..\..\..\..\Tmp\Win32\GGDom\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\..\..\Include\Base" /I "..\..\..\..\Include\Dom" /I "..\..\..\..\Include\Flash" /I "..\..\..\..\Resource\Include" /D "__TOUCHPAD__" /D "_ANALY" /D "_WINEMU" /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "_LIB" /D "_SOURCECODE" /FR /YX"stdafxggdom.h" /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\Lib\Win32\Debug\GGDom.lib"

!ENDIF 

# Begin Target

# Name "GGDom - Win32 Release"
# Name "GGDom - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\src\dom\GGDom.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\StdAfxggdom.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XCatch.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XClient.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XClientApply.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XConnect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XConnectFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XConnectRes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XContentType.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XCSS.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDialogClear.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDialogSave.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDom.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomAccess.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomAnchor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomBlock.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomBR.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomCard.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomDo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomDoc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomEcho.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomFieldset.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomFont.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomGo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomImg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomInput.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomItem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomMeta.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomOnEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomOptGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomOption.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomP.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomPhase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomPhaseImage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomPostfield.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomPre.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomPrev.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomRefresh.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomSelect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomSetvar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomTemplate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomTimer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XDomViewExt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XEntryXML.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XEnumData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XFlashView.cpp

!IF  "$(CFG)" == "GGDom - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "GGDom - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XHomeView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XHTMLA.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XHTMLBody.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XHTMLButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XHTMLCode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XHTMLForm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XHTMLFrame.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XHTMLInput.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XHTMLLi.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XHTMLLink.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XHTMLMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XHTMLObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XHTMLScript.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XHTMLStyle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XHTMLSup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XHTMLTextarea.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XHTMLTH.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XLayout.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XMainWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XSelection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XToolBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XURL.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XVar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\dom\XWAPText.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\include\flash\Bitbuf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\GGDom.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\StdAfxggdom.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XCatch.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XClient.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XClientApply.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XConnect.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XConnectFile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XConnectRes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XContentType.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XCSS.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\base\XData.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDialogClear.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDialogSave.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDom.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomAccess.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomAnchor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomBlock.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomBR.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomCard.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomDo.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomDoc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomEcho.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomFieldset.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomFont.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomGo.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomImg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomInput.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomItem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomMeta.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomOnEvent.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomOptGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomOption.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomP.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomPhase.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomPhaseImage.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomPostfield.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomPre.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomPrev.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomRefresh.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomSelect.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomSetvar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomTable.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomTemplate.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomText.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomTimer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomView.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XDomViewExt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XEntryXML.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XEnumData.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XFlashView.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XHomeView.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XHTMLA.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XHTMLBody.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XHTMLButton.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XHTMLCode.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XHTMLForm.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XHTMLFrame.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XHTMLInput.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XHTMLLi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XHTMLLink.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XHTMLMap.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XHTMLObject.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XHTMLScript.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XHTMLStyle.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XHTMLSup.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XHTMLTextarea.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XHTMLTH.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XLayout.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XMainWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XSelection.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XStatusBar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XToolBar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XURL.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XVar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\dom\XWAPText.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\include\flash\geom.h
# End Source File
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
# Section GGDom : {8582D2E0-E983-4427-A948-D5B6FF7DC9D2}
# 	2:20:TYPEDEF: CONNECTINFO:CONNECTINFO
# 	2:19:CLASS: _CONNECTINFO:_CONNECTINFO
# 	2:10:XConnect.h:XConnect.h
# 	2:19:CLASS: XConnectHttp:XConnectHttp
# 	2:21:TYPEDEF: RESPONSEINFO:RESPONSEINFO
# 	2:10:ENUM: enum:enum
# 	2:19:TYPEDEF: CLIENTINFO:CLIENTINFO
# 	2:18:CLASS: _CLIENTINFO:_CLIENTINFO
# 	2:15:CLASS: XConnect:XConnect
# 	2:19:Application Include:GGDom.h
# 	2:12:XConnect.cpp:XConnect.cpp
# 	2:20:CLASS: _RESPONSEINFO:_RESPONSEINFO
# End Section
# Section GGDom : {C11186DF-3461-4071-97AC-7826C54C0F83}
# 	2:6:XDom.h:XDom.h
# 	2:8:XDom.cpp:XDom.cpp
# 	2:11:CLASS: XDom:XDom
# 	2:19:Application Include:GGDom.h
# End Section
# Section GGDom : {FB1DA6CB-BD71-4BDE-98C7-A00D5A0F593D}
# 	2:8:XVar.cpp:XVar.cpp
# 	2:6:XVar.h:XVar.h
# 	2:17:TYPEDEF: XVarList:XVarList
# 	2:10:ENUM: enum:enum
# 	2:11:CLASS: XVar:XVar
# 	2:19:Application Include:GGDom.h
# End Section
# Section GGDom : {A45E36D9-9D08-479A-BB67-A4A0DD55E765}
# 	2:21:TYPEDEF: PDRAWCONTEXT:PDRAWCONTEXT
# 	2:15:CLASS: XDomLeaf:XDomLeaf
# 	2:17:TYPEDEF: CELLDATA:CELLDATA
# 	2:19:TYPEDEF: XIMAGEDATA:XIMAGEDATA
# 	2:18:CLASS: _XIMAGEDATA:_XIMAGEDATA
# 	2:18:TYPEDEF: PCELLDATA:PCELLDATA
# 	2:15:CLASS: XDomNode:XDomNode
# 	2:15:CLASS: XDomItem:XDomItem
# 	2:20:TYPEDEF: DRAWCONTEXT:DRAWCONTEXT
# 	2:19:TYPEDEF: XIMAGELIST:XIMAGELIST
# 	2:19:CLASS: _DRAWCONTEXT:_DRAWCONTEXT
# 	2:10:ENUM: enum:enum
# 	2:17:TYPEDEF: XDomList:XDomList
# 	2:10:XDomItem.h:XDomItem.h
# 	2:19:TYPEDEF: SELECTINFO:SELECTINFO
# 	2:18:CLASS: _SELECTINFO:_SELECTINFO
# 	2:12:XDomItem.cpp:XDomItem.cpp
# 	2:19:TYPEDEF: PSELEFINFO:PSELEFINFO
# 	2:19:TYPEDEF: _CREATEDOM:_CREATEDOM
# 	2:19:Application Include:GGDom.h
# 	2:19:TYPEDEF: XEVENTDATA:XEVENTDATA
# 	2:18:CLASS: _XEVENTDATA:_XEVENTDATA
# 	2:16:CLASS: _CELLDATA:_CELLDATA
# 	2:15:CLASS: XDomData:XDomData
# End Section
# Section GGDom : {1179F5CC-C79A-44EC-A5C7-2856F4192552}
# 	2:18:CLASS: XEnumMethod:XEnumMethod
# 	2:16:CLASS: XEnumType:XEnumType
# 	2:21:CLASS: XEnumInputType:XEnumInputType
# 	2:19:CLASS: XEnumAttribs:XEnumAttribs
# 	2:19:CLASS: XEnumCharset:XEnumCharset
# 	2:17:CLASS: XEnumAlias:XEnumAlias
# 	2:10:ENUM: enum:enum
# 	2:22:CLASS: XEnumHttpStatus:XEnumHttpStatus
# 	2:18:CLASS: XEnumHeader:XEnumHeader
# 	2:15:CLASS: XEnumArg:XEnumArg
# 	2:10:XXMLData.h:XXMLData.h
# 	2:15:CLASS: XEnumLan:XEnumLan
# 	2:12:XXMLData.cpp:XXMLData.cpp
# 	2:17:CLASS: XEnumAlign:XEnumAlign
# 	2:19:Application Include:GGDom.h
# End Section
# Section GGDom : {90864AB3-6E2D-43CA-9CB4-741A40D2D62E}
# 	2:14:CLASS: XClient:XClient
# 	2:9:XClient.h:XClient.h
# 	2:11:XClient.cpp:XClient.cpp
# 	2:17:TYPEDEF: REQARRAY:REQARRAY
# 	2:10:ENUM: enum:enum
# 	2:19:TYPEDEF: CONNECTREQ:CONNECTREQ
# 	2:18:CLASS: _CONNECTREQ:_CONNECTREQ
# 	2:19:Application Include:GGDom.h
# End Section
# Section GGDom : {66709933-74BB-44B2-9DB9-0A5B01AC1631}
# 	2:6:XDom.h:XDom.h
# 	2:8:XDom.cpp:XDom.cpp
# 	2:11:CLASS: XDom:XDom
# 	2:19:Application Include:GGDom.h
# End Section
# Section GGDom : {0D3A0387-9F4C-41B8-A7C2-80FB5B912C9D}
# 	2:16:XContentType.cpp:XContentType.cpp
# 	2:10:ENUM: enum:enum
# 	2:14:XContentType.h:XContentType.h
# 	2:19:Application Include:GGDom.h
# 	2:19:CLASS: XContentType:XContentType
# End Section
# Section GGDom : {6414A8CC-04AF-40BB-BEC3-05AA545FB07C}
# 	2:14:XConnectFile.h:XConnectFile.h
# 	2:16:XConnectFile.cpp:XConnectFile.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:CLASS: XConnectFile:XConnectFile
# 	2:19:Application Include:GGDom.h
# End Section
