# Microsoft Developer Studio Project File - Name="GGFlash" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=GGFlash - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GGFlash.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GGFlash.mak" CFG="GGFlash - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GGFlash - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "GGFlash - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GGFlash - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Tmp\Win32\GGFlash\Release"
# PROP Intermediate_Dir "..\..\..\..\Tmp\Win32\GGFlash\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O1 /I "..\..\..\..\Include\Base" /I "..\..\..\..\Include\Flash" /I "..\..\..\..\Resource\Include" /D "_ANALY" /D "_WINEMU" /D "WIN32" /D "NDEBUG" /D "_UNICODE" /D "_LIB" /FR /YX"stdafxflash.h" /FD /Zm500 /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\Lib\Win32\Release\GGFlash.lib"

!ELSEIF  "$(CFG)" == "GGFlash - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\Tmp\Win32\GGFlash\Debug"
# PROP Intermediate_Dir "..\..\..\..\Tmp\Win32\GGFlash\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\..\..\Include\Base" /I "..\..\..\..\Include\Flash" /I "..\..\..\..\Resource\Include" /D "_ANALY" /D "_WINEMU" /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "_LIB" /FR /YX"stdafxflash.h" /FD /GZ /Zm500 /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\Lib\Win32\Debug\GGFlash.lib"

!ENDIF 

# Begin Target

# Name "GGFlash - Win32 Release"
# Name "GGFlash - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\src\flash\bitbuf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\curve.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\edittext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\flashfixed.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\geom.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\GGFlash.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\global.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\memcop.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\morphinter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\ObjectArgs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\ObjectArray.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\ObjectButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\ObjectClip.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\ObjectCreator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\ObjectFunction.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\ObjectKey.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\ObjectMath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\ObjectString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\ObjectText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\Raster.CPP
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\sbitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\sdisplay.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\sobject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\splay.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\splayer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\sstroker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\StdAfxflash.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XActionScript.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XAVM2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XAVM2Script.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XBitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XConfig.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XDrawDib.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XDrawObj.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XFilter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XFlashConfig.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectAccessibility.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectAsBroadcaster.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectBitmap1.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectCamera.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectColor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectColorTransform.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectContextMenu.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectContextMenuItem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectCustomActions.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectDate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectError.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectExternalInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectFileReference.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectFileReferenceList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectLoadVars.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectLocal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectLocalConnection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectMatrix.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectMicrophone.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectMouse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectMovieLoader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectNetConnection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectNetStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectPoint.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectPrintJob.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectRectangle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectSelection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectShared.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectSound.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectStage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectStyleSheet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectSystem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectTextFormat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectTextRender.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectTextSnapshot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectTransform.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectVideo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectXML.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectXMLNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectXMLSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXObjectXMLUI.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXStack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\flash\XXVar.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\include\flash\Bitbuf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\bitstream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\curve.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\edittext.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\flashfixed.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\flashgeom.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\flashglobal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\flashmemcop.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\genericfonts.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\GGFlash.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\mcontext.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\morphinter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\ObjectArgs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\ObjectArray.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\ObjectButton.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\ObjectClip.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\ObjectCreator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\ObjectFunction.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\ObjectKey.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\ObjectMath.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\ObjectString.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\ObjectText.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\Raster.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\sbitmap.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\sdisplay.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\shape.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\sobject.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\splay.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\splayer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\sstroker.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\stags.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\StdAfxflash.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XActionScript.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XAVM2.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XAVM2Script.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XBitmap.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XConfig.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XDrawDib.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XDrawObj.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XFilter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XFlash.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XFlashConfig.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObject.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectAccessibility.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectAsBroadcaster.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectBitmap1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectCamera.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectColor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectColorTransform.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectContextMenu.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectContextMenuItem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectCustomActions.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectDate.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectError.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectExternalInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectFileReference.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectFileReferenceList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectLoadVars.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectLocal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectLocalConnection.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectMatrix.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectMicrophone.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectMouse.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectMovieLoader.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectNetConnection.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectNetStream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectPoint.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectPrintJob.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectRectangle.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectSelection.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectShared.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectSound.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectStage.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectStyleSheet.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectSystem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectTextFormat.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectTextRender.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectTextSnapshot.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectTransform.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectVideo.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectXML.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectXMLNode.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectXMLSocket.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXObjectXMLUI.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXStack.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\flash\XXVar.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
# Section GGFlash : {550E6C68-79B0-4664-8367-2C7CB80983EA}
# 	2:18:TYPEDEF: XXVARLIST:XXVARLIST
# 	2:10:ENUM: enum:enum
# 	2:15:XXObjectVar.cpp:XXObjectVar.cpp
# 	2:13:XXObjectVar.h:XXObjectVar.h
# 	2:12:CLASS: XXVar:XXVar
# 	2:19:Application Include:GGFlash.h
# End Section
# Section GGFlash : {629F7BA1-056E-49A0-BBFA-859FE3BF1922}
# 	2:22:CLASS: XCTExportAssets:XCTExportAssets
# 	2:28:CLASS: XCTSetBackgroundColor:XCTSetBackgroundColor
# 	2:6:CT.cpp:CT.cpp
# 	2:22:CLASS: XCTPlaceObject2:XCTPlaceObject2
# 	2:22:CLASS: XCTPlaceObject3:XCTPlaceObject3
# 	2:19:CLASS: XCTShowFrame:XCTShowFrame
# 	2:22:CLASS: XCTDoInitAction:XCTDoInitAction
# 	2:4:CT.h:CT.h
# 	2:10:ENUM: enum:enum
# 	2:20:CLASS: XCTFrameLabel:XCTFrameLabel
# 	2:18:CLASS: XCTDoAction:XCTDoAction
# 	2:23:CLASS: XCTRemoveObject2:XCTRemoveObject2
# 	2:22:CLASS: XCTRemoveObject:XCTRemoveObject
# 	2:21:CLASS: XClipActionRec:XClipActionRec
# 	2:17:CLASS: XCTProtect:XCTProtect
# 	2:34:CLASS: XCTDefineSceneAndFrameLabel:XCTDefineSceneAndFrameLabel
# 	2:21:CLASS: XCTStartSound2:XCTStartSound2
# 	2:15:CLASS: XCTDoABC:XCTDoABC
# 	2:19:Application Include:GGFlash.h
# 	2:20:CLASS: XCTStartSound:XCTStartSound
# 	2:21:CLASS: XCTPlaceObject:XCTPlaceObject
# End Section
# Section GGFlash : {A5773942-6DD3-4779-AB4D-AAE46D033DA6}
# 	2:25:CLASS: XDTSoundStreamHead:XDTSoundStreamHead
# 	2:26:CLASS: XDTSoundStreamBlock:XDTSoundStreamBlock
# 	2:10:ENUM: enum:enum
# 	2:21:CLASS: XDTDefineSound:XDTDefineSound
# 	2:11:DTSound.cpp:DTSound.cpp
# 	2:17:CLASS: XSoundInfo:XSoundInfo
# 	2:14:CLASS: XSndEnv:XSndEnv
# 	2:26:CLASS: XDTSoundStreamHead2:XDTSoundStreamHead2
# 	2:9:DTSound.h:DTSound.h
# 	2:19:Application Include:GGFlash.h
# End Section
# Section GGFlash : {B8F50217-1DF7-49A1-832E-6FCAD96DA682}
# 	2:8:Action.h:Action.h
# 	2:20:CLASS: XActionScript:XActionScript
# 	2:19:CLASS: XActionCodes:XActionCodes
# 	2:23:CLASS: XActionCondition:XActionCondition
# 	2:16:TYPEDEF: ACTIONS:ACTIONS
# 	2:10:ENUM: enum:enum
# 	2:10:Action.cpp:Action.cpp
# 	2:19:Application Include:GGFlash.h
# End Section
# Section GGFlash : {22AA1B1D-634E-490B-884D-F6C9811C2022}
# 	2:18:CLASS: XFilterGrow:XFilterGrow
# 	2:9:XFilter.h:XFilter.h
# 	2:24:CLASS: XFilterDropShadow:XFilterDropShadow
# 	2:20:TYPEDEF: XFILTERLIST:XFILTERLIST
# 	2:19:CLASS: XFilterBevel:XFilterBevel
# 	2:18:CLASS: XFilterBlur:XFilterBlur
# 	2:22:CLASS: XFilterGradient:XFilterGradient
# 	2:14:CLASS: XFilter:XFilter
# 	2:25:CLASS: XFilterConvolution:XFilterConvolution
# 	2:19:Application Include:GGFlash.h
# 	2:11:XFilter.cpp:XFilter.cpp
# 	2:25:CLASS: XFilterColorMatrix:XFilterColorMatrix
# End Section
# Section GGFlash : {61264698-E8BD-446D-8D46-CF9E3B19B047}
# 	2:22:CLASS: XShapeRecChange:XShapeRecChange
# 	2:19:TYPEDEF: XFILLARRAY:XFILLARRAY
# 	2:30:CLASS: XMorphFillStyleGradient:XMorphFillStyleGradient
# 	2:22:CLASS: XLineStyleArray:XLineStyleArray
# 	2:26:CLASS: XShapeRecEdgeCurved:XShapeRecEdgeCurved
# 	2:19:CLASS: XSWFSHAPECNT:XSWFSHAPECNT
# 	2:22:CLASS: XFillStyleSolid:XFillStyleSolid
# 	2:18:CLASS: XXSHAPEDATA:XXSHAPEDATA
# 	2:23:CLASS: XMorphGradRecord:XMorphGradRecord
# 	2:16:CLASS: XGradient:XGradient
# 	2:18:CLASS: XGradRecord:XGradRecord
# 	2:25:CLASS: XFillStyleGradient:XFillStyleGradient
# 	2:17:CLASS: XFillStyle:XFillStyle
# 	2:22:CLASS: XDTDefineShape2:XDTDefineShape2
# 	2:19:TYPEDEF: XLINEARRAY:XLINEARRAY
# 	2:15:CLASS: XLINECNT:XLINECNT
# 	2:22:CLASS: XDTDefineShape3:XDTDefineShape3
# 	2:18:CLASS: XLineStyle2:XLineStyle2
# 	2:22:CLASS: XDTDefineShape4:XDTDefineShape4
# 	2:12:DTShapes.cpp:DTShapes.cpp
# 	2:16:CLASS: XShapeRec:XShapeRec
# 	2:17:CLASS: XLineStyle:XLineStyle
# 	2:10:ENUM: enum:enum
# 	2:15:CLASS: XFILLCNT:XFILLCNT
# 	2:26:CLASS: XDTDefineMorphShape:XDTDefineMorphShape
# 	2:28:CLASS: XMorphFillStyleBitmap:XMorphFillStyleBitmap
# 	2:26:CLASS: XFillStyleRGradient:XFillStyleRGradient
# 	2:21:CLASS: XDTDefineShape:XDTDefineShape
# 	2:25:CLASS: XDTDefineDrawShape:XDTDefineDrawShape
# 	2:19:CLASS: XShapeWStyle:XShapeWStyle
# 	2:23:CLASS: XFillStyleBitmap:XFillStyleBitmap
# 	2:17:TYPEDEF: XGRADREC:XGRADREC
# 	2:27:CLASS: XMorphFillStyleSolid:XMorphFillStyleSolid
# 	2:19:Application Include:GGFlash.h
# 	2:23:CLASS: XShapeRecEndFill:XShapeRecEndFill
# 	2:22:CLASS: XMorphLineStyle:XMorphLineStyle
# 	2:14:CLASS: XHITCNT:XHITCNT
# 	2:10:DTShapes.h:DTShapes.h
# 	2:28:CLASS: XShapeRecEdgeStraight:XShapeRecEdgeStraight
# 	2:13:CLASS: XShape:XShape
# 	2:31:CLASS: XMorphFillStyleRGradient:XMorphFillStyleRGradient
# 	2:22:CLASS: XFillStyleArray:XFillStyleArray
# End Section
# Section GGFlash : {F84232F0-ACFB-4014-9282-AC5A936FE7D2}
# 	2:20:CLASS: XXObjectConst:XXObjectConst
# 	2:24:TYPEDEF: _ASFunctionCall:_ASFunctionCall
# 	2:18:TYPEDEF: XXVARLIST:XXVARLIST
# 	2:15:CLASS: XXObject:XXObject
# 	2:20:CLASS: XXObjectStock:XXObjectStock
# 	2:19:CLASS: XXObjectBool:XXObjectBool
# 	2:10:XXObject.h:XXObject1.h
# 	2:17:TYPEDEF: XXTOKENS:XXTOKENS
# 	2:18:CLASS: XXSortToken:XXSortToken
# 	2:10:ENUM: enum:enum
# 	2:21:CLASS: XXObjectNumber:XXObjectNumber
# 	2:20:CLASS: XXObjectEvent:XXObjectEvent
# 	2:14:CLASS: XXTOKEN:XXTOKEN
# 	2:14:CLASS: XXStack:XXStack
# 	2:17:TYPEDEF: XOBJLIST:XOBJLIST
# 	2:12:XXObject.cpp:XXObject1.cpp
# 	2:19:Application Include:GGFlash.h
# End Section
# Section GGFlash : {92EA09AE-7232-489A-9DDC-B7699658372B}
# 	2:20:CLASS: XXObjectConst:XXObjectConst
# 	2:24:TYPEDEF: _ASFunctionCall:_ASFunctionCall
# 	2:18:TYPEDEF: XXVARLIST:XXVARLIST
# 	2:15:CLASS: XXObject:XXObject
# 	2:20:CLASS: XXObjectStock:XXObjectStock
# 	2:19:CLASS: XXObjectBool:XXObjectBool
# 	2:10:XXObject.h:XXObject.h
# 	2:17:TYPEDEF: XXTOKENS:XXTOKENS
# 	2:18:CLASS: XXSortToken:XXSortToken
# 	2:10:ENUM: enum:enum
# 	2:21:CLASS: XXObjectNumber:XXObjectNumber
# 	2:20:CLASS: XXObjectEvent:XXObjectEvent
# 	2:14:CLASS: XXTOKEN:XXTOKEN
# 	2:14:CLASS: XXStack:XXStack
# 	2:17:TYPEDEF: XOBJLIST:XOBJLIST
# 	2:12:XXObject.cpp:XXObject.cpp
# 	2:19:Application Include:GGFlash.h
# End Section
