// XResource.h: interface for the XResource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XRESOURCE_H__CB41FE79_ADAB_4B4C_95E2_7F7FFBDCD278__INCLUDED_)
#define AFX_XRESOURCE_H__CB41FE79_ADAB_4B4C_95E2_7F7FFBDCD278__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GGType.h"
#include "XString.h"


#ifdef _WINEMU
	#include "win32.rhh"
#elif defined(__APPLE__)
	#ifdef __IPHONE__
        #include "iphone.rhh"
	#elif defined(__IPAD__)
        #include "ipad.rhh"
	#else
		@Error@
	#endif
#elif defined( __ANDROID__ )
	#include "android.rhh"
#else
	@Error@ //Please Define Device Macro
#endif

class XImage;

class _XEXT_CLASS XResource
{
public:
	XResource();
	void Close();
	int Seek(int pos,int type);
	int Read(void*pData,int nLen);
	XBOOL Open(XString8&strPath);

	static XBOOL LoadData(XU8Array&data,XPCTSTR strRes);
	static XBOOL LoadText(XString8&strTxt,XPCTSTR strRes);
	static XBOOL LoadImage(XImage*pImage,XPCTSTR strRes,XBOOL bBitmap=XFALSE);
	static XPCWSTR LoadWString(XU32 nID);

protected:
	XHANDLE hHandle;
};

#endif // !defined(AFX_XRESOURCE_H__CB41FE79_ADAB_4B4C_95E2_7F7FFBDCD278__INCLUDED_)
