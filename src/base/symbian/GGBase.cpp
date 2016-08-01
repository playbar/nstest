/*
 ============================================================================
 Name		: GGBase.cpp
 Author	  : xie peifu
 Copyright   : Copyright @jqbar
 Description : Static lib source file
 ============================================================================
 */
//  Include Files  

#include "GGBase.h"
static XPCTSTR _strIcons[XMAX_ICONS]={("LOADING"),("FAILED"),("CONNECT"),
	("HOME"),("ANI"),("MAINICON"),"GREENBLOCK","BLUEBLOCK","GRAYBLOCK",XNULL};

//  Local Functions
#ifdef _SYMBIAN

#define MAX_INIT_WCHAR_STRING 0x1000

static XU16* memList[MAX_INIT_WCHAR_STRING];
static XU16 numAllocedString = 0;

const XU16* _convertWchar_toU16(const wchar_t* str)
{
	//assert(str);
	if (!str) return 0;
	//assert(numAllocedString < MAX_INIT_WCHAR_STRING);
	XU32 len = 0;
	for (const wchar_t* p = str; *p; ++p)
		len++;
	memList[numAllocedString] = new XU16[len + 1];
	XU16* _ret = memList[numAllocedString];
	_ret[len] = 0;
	for (XU32 i = 0; i < len; ++i)
	{
		_ret[i] = str[i];
	}
	numAllocedString++;

	return _ret;
}

void release_wchar_mem()
{
	for (XU16 i = 0; i < numAllocedString; ++i)
	{
		delete[] memList[numAllocedString];
	}
	numAllocedString = 0;
}

//XPCWSTR wcschr(XPCWSTR strBuf,XWCHAR C)
//	{
//		while(*strBuf)
//			{
//				if(*strBuf==C) return strBuf;
//				strBuf++;
//			}
//		return XNULL;
//	}

TInt DummyFunction()
	{
	//
	// add your code here
	//
	return 0;
	}

XBOOL _stdcall XInitGGBase() 
{
	/*int err;
	WORD wVersion;
	WSADATA WSAData;
	wVersion=MAKEWORD(2,0);
	err=WSAStartup(wVersion,&WSAData);

	GdiplusStartupInput gdiplusStartupInput(NULL,FALSE,FALSE);
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);*/
	//XBitmap bmp(1,1);
	XGraphics g(1,1);//bmp);
	XSize sz;
	//XString16 strTxt(_strTestString);
	g.MeasureString(XNULL,sz,0);
	XWindow::m_nTextHeight=(XU8)sz.cy;
	XImageThread::GetInstance();
	for(int i=0;i<XMAX_ICONS;i++)
	{
		if(_strIcons[i]==XNULL) break;
		XWindow::GetIcon(i)->CreateFromResource(_strIcons[i],XFALSE);
	}
	return XTRUE;
}

void  _stdcall XReleaseGGBase()
{
	for(int i=0;i<XMAX_ICONS;i++)
		XWindow::GetIcon(i)->Release();
	XImageThread* imageThread = XImageThread::GetInstance();
	delete imageThread;
	release_wchar_mem();
	
}

#elif defined(_WIN32)

XBOOL _stdcall XInitGGBase() 
{
	int err;
	WORD wVersion;
	WSADATA WSAData;
	wVersion=MAKEWORD(2,0);
	err=WSAStartup(wVersion,&WSAData);

	GdiplusStartupInput gdiplusStartupInput(NULL,FALSE,FALSE);
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	//XBitmap bmp(1,1);
	XGraphics g(1,1);//bmp);
	XSize sz;
	//XString16 strTxt(_strTestString);
	g.SetFontSize(XWindow::m_nFontSize);	
	g.MeasureString(XNULL,sz,0);
	XWindow::m_nTextHeight=(XU8)sz.cy;

	for(int i=0;i<XMAX_ICONS;i++)
	{
		if(_strIcons[i]==XNULL) break;
		XWindow::GetIcon(i)->CreateFromResource(_strIcons[i]);
	}

	return XTRUE;
}

void  _stdcall XReleaseGGBase()
{
	for(int i=0;i<XMAX_ICONS;i++)
		XWindow::GetIcon(i)->Release();



	::WSACleanup();
	GdiplusShutdown(m_gdiplusToken);
}

#endif
