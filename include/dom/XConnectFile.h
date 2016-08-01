// XConnectFile.h: interface for the XConnectFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCONNECTFILE_H__B8DB17C3_35F6_4FF5_BBBF_6DABC81B6E07__INCLUDED_)
#define AFX_XCONNECTFILE_H__B8DB17C3_35F6_4FF5_BBBF_6DABC81B6E07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XConnect.h"
#include "XWAPText.h"

class XConnectFile : public XConnect  
{
public:
	enum {TYPE_GIF=0,
	  TYPE_PNG=1,
	  TYPE_JPG=2,
	  TYPE_BMP=6,
	  TYPE_TXT=7,
	  TYPE_SWF=19};
	enum {XNONE,XFOLDER,XFILE};
	struct FILETYPE
	{
		XPCTSTR strExtern;
		XPCTSTR strIcon;
		XU8		nClass;
		XU8		nType;
	};
public:
	//int ProcHeader(XU8 *pData, XU32 nSize);
	static void GetConfigFile(XString8&strFile,XString8&strConfig);
	static XINT GetFileType(const XString8&strFile);
	XConnectFile(XClient*pClient);
	virtual ~XConnectFile();
	int Connect(XBOOL bRetry);
	virtual bool IsValid(){return m_file.IsValid()!=0;}
	virtual void CreateRequest(){}
	static FILETYPE _fileTypes[];
protected:
	//int		 m_nFileType;
	XWAPText m_strTxt;
	XString8 m_strPath;
	XFileFinder m_find;
	XFile	    m_file;
	enum {XF_NONE,XF_FILE,XF_FOLDER};
	void DisConnect();
	int SendData(XBOOL bRetry);
	int GetData();
	void Begin();
	void AddFile(XWAPText&strBuf,XPCTSTR strFile,XPCTSTR strFileName);
	XBOOL m_bRoot,m_nType;
	void AddPath(XWAPText&strBuf,XPCTSTR strPath,XPCTSTR strPathName,XBOOL bRoot=XFALSE);
	XString8 m_strSubPath;
	XU8 GetFileAttrib(XString8&strPath);
};

#endif // !defined(AFX_XCONNECTFILE_H__B8DB17C3_35F6_4FF5_BBBF_6DABC81B6E07__INCLUDED_)
