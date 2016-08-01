// XTime.cpp: implementation of the XTime class.
//
//////////////////////////////////////////////////////////////////////

//#include "StdAfxGGBase.h"
#include "XSystem.h"

#ifdef __ANDROID__
#include "jni.h"
#include "GlobalFun.h"
#endif

XU32	XSystem::_nTotalFrames = 0;
XU32	XSystem::_nTotalFrameMemory = 0;
XU32	XSystem::_nTotalTextures = 0;
XU32	XSystem::_nTotalTextureMemory = 0;
XU32	XSystem::_nTotalImages =0;
XU32	XSystem::_nTotalImageMemory =0;
XU32	XSystem::_nTotalBuffers = 0;
XU32	XSystem::_nTotalBufferMemory = 0;

XString16 XSystem::m_strURL;
XString16 XSystem::m_strPostData;
XString16 XSystem::m_strHeaders;
XString8 XSystem::m_strURL8;
XString8 XSystem::m_strPostData8;
XString8 XSystem::m_strHeaders8;

//ADD Api for detail Useragent
bool  XSystem::GetClientDesc(XString8&text)
{
	//LOGWHERE();
	char * pDesc = GFunGetClientDesc();
	text = pDesc;
	//text += "/480x320/2G";
	//text="ENU/480x320/CPU:1.2GHz,RAM:800MB"
	text += "/";
	text += gWidth;
	text += "*";
	text += gHeight;
//	text += "/2G";
	text += "/CPU:";
	char * pCpu = GFunGetCpu();
	text += pCpu;
	text += ",RAM:";
	char * pRam = GFunGetRam();
	text += pRam;
	text += ",DPI:";
	char * pDpi = GFunGetDpi();
	text += pDpi;
	free( pDesc );
	free( pCpu );
	free( pRam );
	free( pDpi );
	pDesc = NULL;
	//text="ENU/480x320/2G";
	//LOGWHEREMSG( text.GetData() );
	return true;
}

bool  XSystem::SetClipText(XString16&text)
{
	return false;
}
bool  XSystem::GetClipText(XString16&text)
{
	return false;
}
bool  XSystem::Execl(const char* strCmd,const char*args)
{
	return false;
}
//娴�瑙����
bool  XSystem::LaunchHTML(const char*url)
{
	jstring jstr = c2js( url );
	GFunOpenViewUrl( jstr );
	return true;
}
//瀹�瑁�
bool  XSystem::LaunchApplication(const char*url)
{
	jstring jstr = c2js( url );
	GFunOpenViewUrl( jstr );
	return true;
}
//
bool  XSystem::LaunchContext(const char*url,int nType)
{
	jstring jstr = c2js( url );
	GFunOpenViewUrl( jstr );
	return true;
}

bool  XSystem::OpenWindow(const char*url,int nArg)
{
	return false;
}
bool XSystem::OpenWebView(const char*url,int nArg)
{
	LOGWHEREMSG( url );
	jstring jstr = c2js( url );
	GFunOpenWebViewUrl( jstr, nArg );
	return true;
}
XU32  XSystem::GetTotalMemory()
{
//	MEMORYSTATUS MemStat;
//	MemStat.dwLength = sizeof(MEMORYSTATUS);
//	GlobalMemoryStatus(&MemStat);
//	int nNew=MemStat.dwTotalPageFile/ 1024L;
//	return nNew;
	return 0;
}
XU32  XSystem::GetFreeMemory()
{
//	MEMORYSTATUS MemStat;
//	MemStat.dwLength = sizeof(MEMORYSTATUS);
//	GlobalMemoryStatus(&MemStat);
//	int nNew=MemStat.dwAvailPhys/ 1024L;
//	return nNew;
	return 0;
}
XU32  XSystem::GetUsedMemory()
{
//	//HANDLE   hProcess=(HANDLE)nProcessID;//AfxGetInstanceHandle();//AfxGetApp()->m_hInstance;
//    PROCESS_MEMORY_COUNTERS   pmc;
//	HANDLE hProcess   =  ::GetCurrentProcess();
//		//OpenProcess( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, nProcessID   );
//    if   (NULL   ==   hProcess)
//               return 0;
//	XU32 nSize=0;
//    if(GetProcessMemoryInfo(   hProcess,   &pmc,   sizeof(pmc))   )
//	 {
//		 return pmc.WorkingSetSize;
//	 }
//
//     //CloseHandle(   hProcess   );
//	 return nSize;
	return 0;
}
