// XEntryXML.h: interface for the XEntryXML class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XENTRYXML_H__377C9E3A_C75F_4F07_9770_A1DB16EC7E79__INCLUDED_)
#define AFX_XENTRYXML_H__377C9E3A_C75F_4F07_9770_A1DB16EC7E79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class XEntryXML  
{
public:
#ifdef _SYMBIAN	  // modify by hgl
#ifdef _DEBUG
	#pragma pack(1)
#endif
	typedef struct _DOMENTRY
	{
		_CREATEDOM Create;
		XPCTSTR	   strEntry;
		XU16	   nCode;
	};
#ifdef _DEBUG
	#pragma pack()
#endif
#else
	#pragma pack(1)
	typedef struct _DOMENTRY
	{
		_CREATEDOM Create;
		XPCTSTR	   strEntry;
		XU16	   nCode;
	};
	#pragma pack()
#endif
public:
	
	virtual ~XEntryXML();
protected:
	XEntryXML();
	static XPCTSTR GetToken(XU16 ID);
	static _DOMENTRY _tagToken[];
};

#endif // !defined(AFX_XENTRYXML_H__377C9E3A_C75F_4F07_9770_A1DB16EC7E79__INCLUDED_)
