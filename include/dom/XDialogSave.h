// XDialogSave.h: interface for the XDialogSave class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDIALOGSAVE_H__80A0309C_1718_4A87_BFD1_255E93725631__INCLUDED_)
#define AFX_XDIALOGSAVE_H__80A0309C_1718_4A87_BFD1_255E93725631__INCLUDED_

#include "XClientApply.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _SYMBIAN
#define KDownSis	1
#endif

#ifdef __APPLE__
class XDialogSave : public XDialog
#elif defined _SYMBIAN
class XDialogSave : public XDialog
#elif defined( __ANDROID__ )
class XDialogSave : public XDialog
#else
class XDialogSave : public XMessageBox  
#endif
{
public:	
	static void GetPrompt(XString8&url,XString16&strTxt);
	void Release();
	XU32 DoModal(XWindow*pHandle);
	XDialogSave(XClientApply*pApply,XString8&strURL,XPCWSTR strPrompt,XU8Array&config);
#ifdef _SYMBIAN
	XDialogSave(XClientApply*pApply,XString8&strURL,XPCWSTR strPrompt);
#endif
	virtual ~XDialogSave();
	
#ifdef __APPLE__
	void _setParent(XWindow* window);
#endif

#ifdef _SYMBIAN
	unsigned char GetDownStatus(){return m_nDownStatus;}
#endif
	

#ifdef __ANDROID__
	void _setParent(XWindow* window);
#endif
protected:
	XFile m_file;
	int m_nTimes;
	void OnFinished(XClient*pClient,XBOOL bOK=XTRUE);
	void OnRecive(XClient*pClient);
	void OnStarted(XClient*pClient);
	void OnConnect(XClient*pClient);
	XString8 m_strFile;
	XBOOL Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XClient* m_pClient;
	int m_nPecent,m_nCode;
	XBOOL OnPaint(XGraphics&g);
	XClientApply *m_pApply;
	XString8 m_strName;
	XPCTSTR m_strURL;
#ifdef _SYMBIAN
	unsigned char m_nDownStatus;			//????
#endif
};

#endif // !defined(AFX_XDIALOGSAVE_H__80A0309C_1718_4A87_BFD1_255E93725631__INCLUDED_)
