// XTime.cpp: implementation of the XTime class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxggbase.h"
#pragma comment(lib,"psapi.lib")
#include "XSystem.h"
#include "XWindow.h"
#include   "psapi.h "

XU32	XSystem::_nTotalFrames = 0;
XU32	XSystem::_nTotalFrameMemory = 0;
XU32	XSystem::_nTotalTextures = 0;
XU32	XSystem::_nTotalTextureMemory = 0;
XU32	XSystem::_nTotalImages =0;
XU32	XSystem::_nTotalImageMemory =0;
XU32	XSystem::_nTotalBuffers = 0;
XU32	XSystem::_nTotalBufferMemory = 0;
//ADD Api for detail Useragent
bool  XSystem::GetClientDesc(XString8&text)
{
	text="ENU/480x320/2G";
	return true;
}

bool  XSystem::SetClipText(XString16&text)
{
	if(OpenClipboard(NULL))
	{
		
		int l=text.GetLength()*2+2;
		HGLOBAL hMem=::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,l);
		HANDLE p=::GlobalLock(hMem);
		memcpy(p,text.GetData(),l);
		//XU16*pData=new XU16(text.GetLength()+1);
		//memcpy(pData,text.GetData(),text.GetLength()+1);
		HANDLE h=::SetClipboardData(CF_UNICODETEXT,p);

		CloseClipboard();

		::GlobalUnlock(hMem);
		::GlobalFree(hMem);
		return TRUE;
	}
	return false;
}
bool  XSystem::GetClipText(XString16&text)
{
	if(::IsClipboardFormatAvailable(CF_UNICODETEXT))
	{
			//BYTE* hNewDIB = NULL;
		if (OpenClipboard(NULL))
		{
		  HANDLE h=::GetClipboardData(CF_UNICODETEXT);
		  if (h == NULL) return false;
		  DWORD dwLen = ::GlobalSize((HGLOBAL) h);
		  text.SetSize(dwLen+1);
		  void* lp     = ::GlobalLock((HGLOBAL) h);
		  memcpy(text.GetData(), lp, dwLen);		
		  text.GetData()[dwLen]=0;
		  ::GlobalUnlock(h);
		  CloseClipboard();		 
		}
		return true;
	}
	return false;
}

XString16 XSystem::m_strURL;
XString16 XSystem::m_strPostData;
XString16 XSystem::m_strHeaders;
XString8 XSystem::m_strURL8;
XString8 XSystem::m_strPostData8;
XString8 XSystem::m_strHeaders8;

bool  XSystem::Execl(const char* strCmd,const char*args)
{
	return LaunchHTML(strCmd);
	return false;
}
bool  XSystem::LaunchHTML(const char*url)
{
	if(!XWindow::m_pMainWnd)
		return false;
	//m_strURL=url;//.SetString(url,strlen(url));
	SetURL(XString8(url));
	XWindow::m_pMainWnd->PostMsg(XWM_UPDATEMSG,XCMD_UPDATE_WEBVIEW,(XU32)XWindow::m_pMainWnd, XMS_TOPMOST);
	return true;
}
bool  XSystem::LaunchApplication(const char*url)
{
	return LaunchHTML(url);
	return true;
}
bool  XSystem::LaunchContext(const char*url,int nType)
{
	return LaunchHTML(url);
	return true;
}

bool  XSystem::OpenWindow(const char*url,int nArg)
{
	return LaunchHTML(url);
	return false;
}

 bool XSystem::OpenWebView(const char*url,int nType)
 {
	 return LaunchHTML(url);
	 return false;
 }


XU32  XSystem::GetTotalMemory()
{
	MEMORYSTATUS MemStat;
	MemStat.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&MemStat);
	int nNew=MemStat.dwTotalPageFile/ 1024L;
	return nNew;
}
XU32  XSystem::GetFreeMemory()
{
	MEMORYSTATUS MemStat;
	MemStat.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&MemStat);
	int nNew=MemStat.dwAvailPhys/ 1024L;
	return nNew;;
}
XU32  XSystem::GetUsedMemory()
{
	//HANDLE   hProcess=(HANDLE)nProcessID;//AfxGetInstanceHandle();//AfxGetApp()->m_hInstance; 
    PROCESS_MEMORY_COUNTERS   pmc; 
	HANDLE hProcess   =  ::GetCurrentProcess();
		//OpenProcess( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, nProcessID   ); 
    if   (NULL   ==   hProcess) 
               return 0; 
	XU32 nSize=0;
    if(GetProcessMemoryInfo(   hProcess,   &pmc,   sizeof(pmc))   ) 
	 { 
		 return pmc.WorkingSetSize;
	 } 

     //CloseHandle(   hProcess   ); 
	 return nSize;
}

