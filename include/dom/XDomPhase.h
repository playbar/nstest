// XDomPhase.h: interface for the XDomPhase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMPHASE_H__C5A7DADF_195C_47E2_AE19_81F7683D79AF__INCLUDED_)
#define AFX_XDOMPHASE_H__C5A7DADF_195C_47E2_AE19_81F7683D79AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"
#include "XDomBR.h"	// Added by ClassView

class XDom;

enum {XPS_START=0,
	  XPS_TEXT=0x1,
	  XPS_TAG=0x2,
	  XPS_ATTNAME=0x3,
	  XPS_ATTVAL=0x4,
	  XPS_STRING=0x5,
	  XPS_CHAR=0x6,
	  XPS_DATA=0x7,
	  XPS_SKIP=0x8,
	  XPS_ECHO=0x9,
	  XPS_SCRIPT=0xA};//,XPS_CHARSET=0xB};


class XDomPhase  
{
public:
	XBOOL NeedConvert();
	virtual XBOOL PhaseData(XTCHAR* pData,int nSize,bool bFlash,bool bCondenseWhite=false){return XTRUE;}
//#if (__CORE_VERSION__>=0x02073000)
	virtual XBOOL EndParse(XBOOL bFlash){return XFALSE;}
//#else
//	virtual XBOOL EndParse(){return XFALSE;}
//#endif
	XDomPhase(XDom*pDom);
	virtual ~XDomPhase();
protected:
	
	XDom*	m_pDom;
}; 



class XDomPhaseXML : public XDomPhase
{
public:

#ifdef _SYMBIAN	  // modify by hgl
#ifdef _DEBUG
	#pragma pack(1)
#endif
	typedef struct
	{
		_CREATEDOM Create;
		XPCTSTR	   strEntry;
		XU8		   bMustEnd;
		XU16*	   nChilds;
		XU16*	   nNChilds;
		XU8		   bStringParam;
		//XPCWSTR	   nChilds;
		//XU16	   nCode;
	}_DOMENTRY;
#ifdef _DEBUG
	#pragma pack()
#endif
#else
	#pragma pack(1)
	typedef struct
	{
		_CREATEDOM Create;
		XPCTSTR	   strEntry;
		XU8		   bMustEnd;
		XU16*	   nChilds;
		XU16*	   nNChilds;
		XU8		   bStringParam;
		//XPCWSTR	   nChilds;
		//XU16	   nCode;
	}_DOMENTRY;
	#pragma pack()
#endif

public:
	static XU8 MustEnd(XU16 ID);
	static XU8 StringParam(XU16 ID);
	static XINT IndexOf(XPCTSTR strKey,XU8 nType=XSS_NOCASE);
	XDomPhaseXML(XDom*pDom);
	~XDomPhaseXML();
	XBOOL PhaseData(XTCHAR* pData,int nSize,bool bFlash,bool bCondenseWhite=false);
//#if (__CORE_VERSION__>=0x02073000)
	virtual XBOOL EndParse(XBOOL bFlash);
//#else
//	virtual XBOOL EndParse();
//#endif
	static XPCTSTR GetToken(XU16 ID);
	static XU16*   GetTokenChilds(XU16 ID);
	static XU16*   GetTokenNChilds(XU16 ID);
	static _DOMENTRY _tagToken[];
protected:
	XBOOL TransCharset(XU8 bSet=XFALSE);
	XU8 m_bURL;
	int PhaseScript(XTCHAR*pData,int nSize);
	int PhaseKeyData(XTCHAR*pData,int nSize,XBOOL bSkip);
	int PhaseEcho(XTCHAR*pData,int nSize);
	int PhaseChar(XTCHAR*pData,int nSize);
	int PhaseString(XTCHAR*pData,int nSize);
	int PhaseValue(XTCHAR*pData,int nSize);
	int PhaseName(XTCHAR*pData,int nSize);
	int PhaseTag(XTCHAR*pData,int nSize);
	int PhaseText(XTCHAR*pData,int nSize,bool bFlash,bool bCondenseWhite=false);
	XString8 m_strChar;
//	XPhaseText* m_pText;
	XDomItem* CreateDomItem();
	XBOOL AddTag(XU8 bEnd);
	void SetStatus(XU8 nStatus);
	XString8 m_strKey,m_strValue,m_strTxt;//Value;
//	XPhase* m_pPhase;
	XU32 m_nLength;
	XU8 m_nStatus,m_nSpaceCount,m_nKeyCount,m_nStrChar,m_bMustEnd,m_bFirst;
	XStack<XU8> m_stack;
};

#endif // !defined(AFX_XDOMPHASE_H__C5A7DADF_195C_47E2_AE19_81F7683D79AF__INCLUDED_)
