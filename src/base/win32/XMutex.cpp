// XMutex.cpp: implementation of the XMutex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxggbase.h"
#include "XMutex.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XMutex::XMutex()
{
	m_hObject = ::CreateMutex(XNULL,0,XNULL);
	m_nCount=0;
}

XBOOL XMutex::Lock(XU32 dwTimeout)
{
	if (::WaitForSingleObject(m_hObject, dwTimeout) == WAIT_OBJECT_0)
	{
		m_nCount++;
		return XTRUE;
	}
	else
		return XFALSE;
}

XBOOL XMutex::UnLock()
{
	//if(m_nCount)
	m_nCount--;
	return ::ReleaseMutex(m_hObject);
}

XMutex::~XMutex()
{
	if(m_hObject!=XNULL)
	{
		CloseHandle(m_hObject);
		m_hObject=XNULL;
	}
}



XThread::XThread(XBOOL bDelete)
{
	m_bDelete=bDelete;
	m_hThread=XNULL;
	m_pBase=XNULL;
	m_nThreadID=0;
}

XThread::~XThread()
{
   Release();
}

XBOOL XThread::Create(XBase* pBase,XU32 nID)
{
	 Release();
	 m_nThreadID=nID;
	 m_pBase=pBase;
	 m_hStop.Create();
	 m_hStoped.Create();
	 m_hThread=::CreateThread(XNULL,0,(LPTHREAD_START_ROUTINE)_CallBack,
				this,CREATE_SUSPENDED,&m_nThreadID);
	 ::ResumeThread(m_hThread);
	 return XTRUE;
}

XUINT XThread::CallBack()
{
	m_hStop.ResetEvent();
	m_hStoped.ResetEvent();
	XUINT nID=m_nThreadID;
	if(m_pBase)
		m_pBase->StartCall();
	while(1)
	{
		if(NeedStop())
			break;
		m_pBase->CallBack(m_nThreadID);
		Sleep(10);
	}	 
	m_hStoped.SetEvent();
	if(m_pBase)
		m_pBase->ReleaseCall();
	if(m_bDelete) 
	{
		if(m_pBase!=XNULL) delete m_pBase;
		m_pBase=XNULL;
	}
	
	//::ExitThread(nID);
	//Release();
	return nID;
}

void XThread::Release(XBOOL bWait)
{
	Stop(bWait);
	if(m_hThread!=XNULL)
	{	
		CloseHandle(m_hThread);
		m_hThread=XNULL;
	}
	m_hStop.Release();
	m_hStoped.Release();
	
}

XBOOL XThread::NeedStop(int nWait)
{
	if(m_hStop.IsEmpty())
		return XTRUE;
	return m_hStop.IsEvent(nWait);
}

XBOOL XThread::Stop(XBOOL bWait)
{
   if(IsStoped()) return XTRUE;
   if(m_hThread!=XNULL)
	{
		m_hStop.SetEvent();
		if(bWait)
		{
			while(1)
			{
				Sleep(10);
				if(m_hStoped.IsEvent(0))
					break;
				m_hStop.SetEvent();
			}
		}
	}
   return XTRUE;
}


//XBOOL XThread::IsStoped()
//{return m_hThread==XNULL||WaitForSingleObject(m_hStoped,0)==WAIT_OBJECT_0;}

void XEvent::Create()
{
	m_hObject=::CreateEvent(XNULL,1,0,XNULL);
}

void XEvent::Release()
{
	if(m_hObject!=XNULL) ::CloseHandle(m_hObject);
	m_hObject=XNULL;
}

XEvent::~XEvent()
{
	Release();
}

void XEvent::ResetEvent()
{
	if(m_hObject!=XNULL)
	::ResetEvent(m_hObject);
}

void XEvent::SetEvent()
{
	if(m_hObject!=XNULL)
		::SetEvent(m_hObject);
}

XBOOL XEvent::IsEvent(XU32 t)
{
   return WaitForSingleObject(m_hObject,t)==WAIT_OBJECT_0;
}

void XThread::Sleep(XU32 t)
{
	::Sleep(t);
}
