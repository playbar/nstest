// XTime.cpp: implementation of the XTime class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxggbase.h"
#ifdef _SYMBIAN
#include "XSystem.h"
#include <hal.h>
#include <apgcli.h>
#include <apgtask.h>
#include <eikenv.h>
#include "ZCConvert.h"
#else
#pragma comment(lib,"psapi.lib")
#include "XSystem.h"
#include   "psapi.h "
#endif


	//API for client UA
bool  XSystem::GetClientDesc(XString8&text)
{
	TInt value = 0;
	TInt ret = HAL::Get(HALData::EModel, value);
	if(ret == KErrNone)
		{
		XString8 model;
		model.FromInt(value);
		text = model;
		return true;
		}
	return false;
}
bool  XSystem::SetClipText(XString16&text)
{
#ifdef _SYMBIAN
	return false;
#else
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
#endif
}
bool  XSystem::GetClipText(XString16&text)
{
#ifdef _SYMBIAN
	return false;
#else
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
#endif
}
bool  XSystem::Execl(const char* strCmd,const char*args)
{
	return false;
}
bool  XSystem::LaunchHTML(const char*url)
{
	return false;
}
bool  XSystem::LaunchApplication(const char*url)
{
	return false;
}
bool  XSystem::LaunchContext(const char*url,int nType)
{
	if(url == NULL)
		return false;
	TPtrC8 url_8;
	url_8.Set((unsigned char*)url);
	if(url_8.Length() <= 0)
		return false;
	const TInt KBrowserUid1 = 0x1020724D;
	const TInt KBrowserUid2 = 0x10008D39;
	
	RApaLsSession appArcSession;
	User::LeaveIfError(appArcSession.Connect()); // connect to AppArc server
	// Wap Browser's constants UId
	TUid browserId( TUid::Uid( KBrowserUid1 ) );
	
	TApaAppInfo appInfo;
	if( appArcSession.GetAppInfo( appInfo, browserId ) != KErrNone )
		{
		browserId = TUid::Uid( KBrowserUid2 );
		}
	
	TApaTaskList taskList( CEikonEnv::Static()->WsSession() );
	TApaTask task = taskList.FindApp( browserId );
	
	if(task.Exists())
		{
		task.SendMessage(TUid::Uid(0),url_8);
		}
	else
		{
		unsigned short* wurl = ZCConvert::ConvertToUnicode((unsigned char*)url);
		TPtrC url_16;
		url_16.Set(wurl);
		TThreadId id;
		appArcSession.StartDocument( url_16, browserId, id );
		delete wurl;
		}
	appArcSession.Close();
	return true;
}

XU32  XSystem::GetTotalMemory()
{
#ifdef _SYMBIAN
	int memory;
	HAL::Get(HAL::EMemoryRAM,memory);
	return memory;
#else
	MEMORYSTATUS MemStat;
	MemStat.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&MemStat);
	int nNew=MemStat.dwTotalPageFile/ 1024L;
	return nNew;
#endif
}
XU32  XSystem::GetFreeMemory()
{
#ifdef _SYMBIAN
	int freeMemory;
	HAL::Get(HAL::EMemoryRAMFree,freeMemory);
	return freeMemory;
#else
	MEMORYSTATUS MemStat;
	MemStat.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&MemStat);
	int nNew=MemStat.dwAvailPhys/ 1024L;
	return nNew;;
#endif
}
XU32  XSystem::GetUsedMemory()
{
#ifdef _SYMBIAN
	return 0;
#else
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
#endif
}
