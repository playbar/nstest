// XCatch.h: interface for the XCatch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCATCH_H__64568C0E_79FF_4A97_BAB7_11CD50785FF0__INCLUDED_)
#define AFX_XCATCH_H__64568C0E_79FF_4A97_BAB7_11CD50785FF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XURL.h"
#include "XConnect.h"
class XClient;
#ifdef __ANDROID__
#else
class XDialogClear;
#endif

class XCatch : public XConnect  
{
protected:
#ifdef _SYMBIAN	  // modify by hgl
	#pragma pack(1)
	struct XCHeader
	{
		XU8  strMark[3];
		XU8	 nMethod;
		XU32 nErrorCode;
		XU32 nMaxAge;
		XU32 nTime;
		XU8  nClass,nCharset;
		XU16 nType;
		//XU32 nSize;
	};
	#pragma pack()
#else
#pragma pack(1)
	struct XCHeader
	{
		XU8  strMark[3];
		XU8	 nMethod;
		XU32 nErrorCode;
		XU32 nMaxAge;
		XU32 nTime;
		XU8  nClass,nCharset;
		XU16 nType;
		//XU32 nSize;
	};
#pragma pack()
#endif
	XBOOL IsStatic(int nClass,int nType);
	friend class XClient;
public:
	int GetData();
	XBOOL SendData(XBOOL bRetry);
	void Begin();
	virtual bool IsValid(){return m_file.IsValid()!=0;}
	virtual void CreateRequest(){return;}
	virtual void DisConnect();	
	XBOOL GetCatchData();
	void RemoveCatch(REQARRAY&list,XBOOL bForce=XFALSE);
	XBOOL RemoveCatch(CONNECTREQ&req,XBOOL bForce=XFALSE);
	XBOOL RemoveFile(XPCTSTR strFile,XBOOL bForce);
#ifdef __ANDROID__
#else
	int Clear(XDialogClear*pDlg,XU8 bAll=XFALSE);
#endif
	int ClearShared();
#ifdef __APPLE__
	int Clear(XU8 bAll);
#endif
#ifdef __ANDROID__
	int Clear( XU8 bAll );
#endif

	XPCTSTR GetWorkPath(){return m_strWorkPath;}
	XU8 CatchDetect(CONNECTINFO&req,XTime&time,XU32&l,XU8 bForward,bool&bStatic);
	enum {XC_NONE,XC_OK,XC_OUTOFAGE,XC_REMOVE};
	XBOOL Catch(void*pData,int nSize);
	void EndCatch(XU32 nCode,XBOOL bSet=XTRUE);
	XBOOL StartCatch(CONNECTINFO&req,RESPONSEINFO&info);
	void ResetTime();
	void Release();
	XBOOL Create(XPCTSTR strWorkPath);
	XCatch();
	XCatch(XPCTSTR strPath):XConnect(XNULL)
	{
		Create(strPath);
	}
	virtual ~XCatch();

protected:
	XU8 DetectFile(XPCTSTR strFile,XU32&l,XTime&time,XU8 bForward,bool&bStatic);
	//XBOOL GetResponse();
	XBOOL Connect(XBOOL bRetry);
	void MakeFile(CONNECTINFO&req);
	XFile m_file;
	XString8 m_strFile;
	XString8 m_strWorkPath;
	XString8 m_strShared;
};

#endif // !defined(AFX_XCATCH_H__64568C0E_79FF_4A97_BAB7_11CD50785FF0__INCLUDED_)
