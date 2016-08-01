// XTime.h: interface for the XTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__XSYSTEM__)
#define __XSYSTEM__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XString.h"

class XSystem
{
public:
	static XU32 _nTotalFrames;
	static XU32	_nTotalFrameMemory;
	static XU32	_nTotalTextures;
	static XU32 _nTotalTextureMemory;
	static XU32 _nTotalImages;
	static XU32 _nTotalImageMemory;
	static XU32 _nTotalBuffers;
	static XU32 _nTotalBufferMemory;
public:
	//static XU32  nProcessID;
	//static void  SetProcessID(XU32 id);
	static bool  SetClipText(XString16&text);
	static bool  GetClipText(XString16&text);
	//API for client UA
	static bool  GetClientDesc(XString8&text);
	static bool  Execl(const char* strCmd,const char*args);
	static bool  LaunchHTML(const char*url);
	static bool  LaunchApplication(const char*url);
	static bool  LaunchContext(const char*url,int nType);
	static bool  OpenWindow(const char*url,int nArg);
    static bool  OpenWebView(const char*url,int nType);
	static XU32  GetTotalMemory();
	static XU32  GetFreeMemory();
	static XU32  GetUsedMemory();
public:
	static XString16 m_strURL;
	static XString16 m_strPostData;
	static XString16 m_strHeaders;
	static XString8 m_strURL8;
	static XString8 m_strPostData8;
	static XString8 m_strHeaders8;
	static void ResetHTMLData()
	{
		m_strPostData.Empty();
		m_strHeaders.Empty();
		m_strPostData8.Empty();
		m_strHeaders8.Empty();
	}
	static void SetHeaders(XString8&str)
	{
		m_strHeaders8=str;
		m_strHeaders=m_strHeaders8;
		
	}
	static void SetURL(XString8&strURL)
	{
		m_strURL8 = strURL;
		if(m_strURL8[0]=='w'||m_strURL8[0]=='W')
		{
			m_strURL8[0]='h';
			m_strURL8[1]='t';
			m_strURL8[2]='t';
			m_strURL8[3]='p';
		}
		m_strURL = m_strURL8;
		
	}
	static void SetPostData(XString8&strData)
	{
		m_strPostData = strData;
		m_strPostData8 = strData;
	}

};

/*class XCamera
{
	static XCamera* GetCamera(const char*name);
	//static bool FreeCamera(XCamera
};
class XMicrophone
{
};*/

#endif // !defined(AFX_XTIME_H__4C0E387A_381D_42C1_A320_F4C3AF63502A__INCLUDED_)
