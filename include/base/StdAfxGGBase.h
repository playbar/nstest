// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__2E1479B9_9FB8_4DE7_9539_3813EEA2FE8C__INCLUDED_)
#define AFX_STDAFX_H__2E1479B9_9FB8_4DE7_9539_3813EEA2FE8C__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000


#ifdef _SYMBIAN
	#include <e32base.h>
	#include <string.h>
	#include <ctype.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <f32file.h>
	#include <charconv.h>
	#include <e32des8.h>
	#include <EscapeUtils.h>
	#include <GDI.H>
	#include <W32STD.H>
	//#include <wchar.h>
#elif defined(_WIN32)

	//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
	#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

	#include <afxwin.h>         // MFC core and standard components
	#include <afxext.h>         // MFC extensions
	#include <afxdisp.h>        // MFC Automation classes
	#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls

	#include <stdlib.h>
	#include <stdio.h>
	#include <winsock2.h>
	#include <windows.h>
	#include <objbase.h>
	#define Gdiplus
	#ifndef ULONG_PTR
		#define ULONG_PTR ULONG
	#endif
#ifndef __field_ecount_opt
	#define __field_ecount_opt(size)
	#define __out_ecount(size)
	#define __out_bcount(size)
#endif
	#include "gdiplus.h"

	#pragma comment(lib, "ws2_32.lib")
	#pragma comment(lib,"winmm.lib")
#endif

//#define SOUND
//#define EDITTEXT


// handle warning
#pragma warning(disable:4996)
#pragma warning(disable:4819)
#pragma warning(disable:4100)
#pragma warning(disable:4244)

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__2E1479B9_9FB8_4DE7_9539_3813EEA2FE8C__INCLUDED_)
