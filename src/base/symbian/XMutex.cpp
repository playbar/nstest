// XMutex.cpp: implementation of the XMutex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxggbase.h"
#include "XMutex.h"
#include "XString.h"
#include <e32const.h>
# include <Ecom\ECom.h>                      //For REComSession: link against: ecom.lib
#include "symbiandebug.h"
#include <pthread.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef _SYMBIAN
const TInt KStackSize = 0x10000;							//�̶߳�ջ�Ĵ�С
//const TInt KStackSize = KDefaultStackSize;
_LIT(KExecThreadBaseName, "ZC_XThread");
_LIT(KExecMutexBaseName,"ZC_XMutex");
 
// Global thread id counter for making CMyThread thread names unique.
// This is achieved by appending counter to the end of thread name and
// incrementing counter value for next thread.
// This is writable static data.
// TInt g_thread_id_counter = 0;

//�������
LOCAL_C void GenerateUniqueName(TDes& aResult, TAny* aExecutor,TInt nID)
    {
    _LIT(KHexFormat, "_0x%x");
    _LIT(KCounterFormat, "_%d");
    aResult.AppendFormat(KHexFormat, aExecutor);
    aResult.AppendFormat(KCounterFormat, nID);
    }
#endif

XMutex::XMutex()
{
#ifdef WINDOWS
	m_hObject = ::CreateMutex(XNULL,0,XNULL);
#endif
#ifdef _SYMBIAN
	m_iMutex.CreateLocal();
	m_iSemaphore.CreateLocal(1);
	m_bLocking = false;
	m_nCount = 0;
#endif
}

XBOOL XMutex::Lock(XU32 dwTimeout)
{
#ifdef WINDOWS
	if (::WaitForSingleObject(m_hObject, dwTimeout) == WAIT_OBJECT_0)
		return XTRUE;
	else
		return XFALSE;
#endif
#ifdef _SYMBIAN
//	if(m_iMutex.IsHeld()) return XTRUE;
//	m_iMutex.Wait();
//	m_bLocking = true;
//	if(dwTimeout == XINFINITE)
//		{
//		m_iMutex.Signal();
//		m_iSemaphore.Wait();
//		m_iMutex.Wait();
//		}
//	else
//		{
//		m_iMutex.Signal();
//		int err = m_iSemaphore.Wait(dwTimeout);
//		m_iMutex.Wait();
//		if(err)
//			{
//			m_iMutex.Signal();
//			return XFALSE;
//			}
//		}
//	m_nCount++;
//	m_iMutex.Signal();
//	return XTRUE;
	
	if(m_iMutex.IsHeld()) return XTRUE;
	if(dwTimeout == XINFINITE)
		{
		m_iMutex.Wait();
		m_iSemaphore.Wait();
		}
	else
		{
		if(KErrNone != m_iSemaphore.Wait(dwTimeout)) return XFALSE;
		m_iMutex.Wait();
		}
	return XTRUE;
//	if(m_iMutex.IsHeld())
//		{
//		m_nCount++;
//		return XTRUE;
//		}
//	else if(dwTimeout == XINFINITE)
//		{
//		m_iMutex.Wait();
//		m_nCount++;
//		return XTRUE;
//		}
//	else
//		{
//		
//		}
#endif
}

XBOOL XMutex::UnLock()
{
#ifdef WINDOWS
	return ::ReleaseMutex(m_hObject);
#endif
#ifdef _SYMBIAN
//	m_iMutex.Wait();
//	if(m_nCount || m_bLocking)
//		{
//		m_iMutex.Signal();
//		m_iSemaphore.Signal(); 
//		m_iMutex.Wait();
//		m_bLocking = false; 
//		m_nCount--;
//		}
//	m_iMutex.Signal();
//	return XTRUE;
	if(m_iMutex.IsHeld())
		{
		m_iMutex.Signal();
		m_iSemaphore.Signal();
		}
	return XTRUE;
#endif
}

XMutex::~XMutex()
{
#ifdef WINDOWS
	if(m_hObject!=XNULL)
	{
		CloseHandle(m_hObject);
		m_hObject=XNULL;
	}
#endif
#ifdef _SYMBIAN
		m_iMutex.Close();
		m_iSemaphore.Close();
#endif
}


#ifdef WINDOWS
XThread::XThread(XBOOL bDelete)
{
	m_bDelete=bDelete;
	m_hThread=XNULL;
	m_pBase=XNULL;
	m_nThreadID=0;
}
#endif
#ifdef _SYMBIAN
XThread::XThread(XBOOL bDelete) : CActive(EPriorityHigh)
	{
	m_bDelete=bDelete;
	m_hThread.SetHandle(XNULL);
	m_pBase=XNULL;
	m_nThreadID=0;
	m_hThreadName = XNULL;
	m_bIsStart = false;
	m_pErrObserver = XNULL;
	m_bTSDReady = false;
	m_hScreenDevice = XNULL;
	m_hFont = XNULL;
//	CActiveScheduler::Add(this);
	}
#endif

XThread::~XThread()
{
   Release();
   if(m_hThreadName) delete m_hThreadName;
   m_hThreadName = XNULL;
}

XBOOL XThread::Create(XBase* pBase,XU32 nID)
{
	 Release();
	 m_nThreadID=nID;
	 m_pBase=pBase;
	 m_hStop.Create();
	 m_hStoped.Create();
#ifdef WINDOWS
	 m_hThread=::CreateThread(XNULL,0,(LPTHREAD_START_ROUTINE)_CallBack,
				this,CREATE_SUSPENDED,&m_nThreadID);
	 ::ResumeThread(m_hThread);
#endif
#ifdef _SYMBIAN
	 m_WaitSemaphore.Create();
	 m_FreeSemaphore.Create();
	 m_FreeSemaphore.ResetEvent();
	 m_FreedSemaphore.Create();
	 m_FreedSemaphore.ResetEvent();
	 if(pBase)
		 m_WaitSemaphore.SetEvent();
	 else
		 m_WaitSemaphore.ResetEvent();
	 HBufC* threadName = HBufC::NewL(KMaxFileName);
	 if(!m_hThreadName)
		 {
		 TPtr ptr = threadName->Des();
		 ptr.Copy(KExecThreadBaseName);
		 GenerateUniqueName(ptr, this, nID);
		 }
	 else
		 {
		 threadName->Des().Append(*m_hThreadName);
		 }
	 int err;
	 err = m_hThread.Create(
					 *threadName,														//�߳����֡�
					 XThread::_CallBack,										//�߳��壬����ָ��
					 KStackSize,
					 &User::Allocator(),
					 this);	
	#ifdef _SYMBIANDEBUG
	 if(err != KErrNone)
		 {
		 SYMBIANDEB1(DefaultLog,":::: %d\r\n",err);
		 }
	#endif
	 m_hThread.Resume();
	delete threadName;
#endif
	 return XTRUE;
}

XUINT XThread::CallBack()
{
	m_hStop.ResetEvent();
	m_hStoped.ResetEvent();
	XUINT nID=m_nThreadID;
//	pthread_key_t key;
//	pthread_key_create(&key,NULL);
	while(1)
	{
		if(NeedStop()) 
			break;
		if(m_WaitSemaphore.IsEvent(100000000))
			{
			if(!m_bTSDReady)
				{
				pthread_key_t key;
				for(int i=0;i<XTSDNUM;i++)
					{
					pthread_key_create(&key,NULL);
					m_rTSDkey[i] = key;
					}
				m_bTSDReady = true;
				}
			if(m_pBase)
				m_pBase->CallBack(m_nThreadID);
			if(NeedFree())
				{
				for(int i=0;i<XTSDNUM;i++)
					pthread_key_delete(m_rTSDkey[i]);
				m_bTSDReady=false;
				m_WaitSemaphore.ResetEvent();
				m_FreedSemaphore.SetEvent();
				m_pBase=XNULL;
				}
			Sleep(200);
			}
	}	 
	m_hStoped.SetEvent();
	if(m_bDelete) 
	{
		if(m_pBase!=XNULL) delete m_pBase;
		m_pBase=XNULL;
	}
	return nID;
}

void XThread::Release(XBOOL bWait)
{
	m_bIsStart = false;
	Stop(bWait);
#ifdef WINDOWS
	if(m_hThread!=XNULL)
	{
		CloseHandle(m_hThread);
		m_hThread=XNULL;
	}
#endif
#ifdef _SYMBIAN
	if(IsActive())
		Cancel();
	if(m_hThread.Handle())
		{
		m_hThread.Kill(0);
		m_hThread.Close();
		}
	m_WaitSemaphore.Release();
	m_FreeSemaphore.Release();
	m_FreedSemaphore.Release();
#endif
	m_hStop.Release();
	m_hStoped.Release();
	
}

XBOOL XThread::NeedStop(int iWait)
{
	if(m_hStop.IsEmpty()) return XTRUE;
	return m_hStop.IsEvent(iWait);
//	if(m_hStoped.IsEmpty()) return XTRUE;
//	return m_hStoped.IsEvent(0);
}

XBOOL XThread::Stop(XBOOL bWait)
{
   if(IsStoped()) return XTRUE;
#ifdef WINDOWS
   if(m_hThread!=XNULL)
#endif
#ifdef _SYMBIAN
	if(m_hThread.Handle())
	{
		m_WaitSemaphore.SetEvent();
#endif
		m_hStop.SetEvent();
		if(bWait)
		{
			while(1)
			{
				if(m_hStoped.IsEvent(0))
					break;
				m_hStop.SetEvent();
				Sleep(200);
			}
		}
	}
   return XTRUE;
}


//XBOOL XThread::IsStoped()
//{return m_hThread==XNULL||WaitForSingleObject(m_hStoped,0)==WAIT_OBJECT_0;}

void XEvent::Create()
{
#ifdef _SYMBIAN
	if(!IsEmpty()) return;
	m_iLock.CreateLocal();
	m_iObject.CreateLocal(1);
	m_iObject.Wait();
	m_bIsCreate = true;
#elif defined(_WIN32)
	m_hObject=::CreateEvent(XNULL,1,0,XNULL);
#endif	
}

void XEvent::Release()
{
#ifdef _SYMBIAN
if(m_bIsCreate)
	{
	m_iLock.Close();
	m_iObject.Close();
	}
m_bIsCreate = XFALSE;
m_bEvent = XFALSE;
#elif defined(_WIN32)
	if(m_hObject!=XNULL) ::CloseHandle(m_hObject);
	m_hObject=XNULL;
#endif	
}

XEvent::~XEvent()
{
	Release();
}

void XEvent::ResetEvent()
{
#ifdef _SYMBIAN
	if(IsEmpty()) return;
	m_iLock.Wait();
	if(m_bEvent)
		{
		m_iLock.Signal();
		m_iObject.Wait();
		m_iLock.Wait();
		m_bEvent=false;
		}
	m_iLock.Signal();
#elif defined(_WIN32)
	if(m_hObject!=XNULL)
		::ResetEvent(m_hObject);
#endif
}

void XEvent::SetEvent()
{
#ifdef _SYMBIAN
	if(IsEmpty()) return;
	if(m_bEvent) return;
	m_iLock.Wait();
	if(!m_bEvent)
		{
		m_iObject.Signal();
		m_bEvent = true;
		}
	m_iLock.Signal();
#elif defined(_WIN32)
	if(m_hObject!=XNULL)
		::SetEvent(m_hObject);
#endif
}

XBOOL XEvent::IsEvent(XU32 t)
{
#ifdef _SYMBIAN
	if(IsEmpty()) return XFALSE;
	if(t == 0) return m_bEvent;
	m_iLock.Wait();
	bool b = false;
	if(m_bIsCreate)
		{
		m_iLock.Signal();
		int err = m_iObject.Wait(t);
		m_iLock.Wait();
		if(err == KErrNone)
			{
			b = true;
			m_iObject.Signal();
			}
		}
	m_iLock.Signal();
	return b;
#elif defined(_WIN32)
   return WaitForSingleObject(m_hObject,t)==WAIT_OBJECT_0;
#endif   
}
//�ж��̣߳�������Դ��libc�е�sleep�ĵ�λ���룬ʱ��̫��fuck������
void XThread::Sleep(XU32 t)
{
#ifdef _SYMBIAN
	User::After(t);
//	sleep(t);
#elif defined(_WIN32)
	::Sleep(t);
#endif
}



#ifdef _SYMBIAN
//�����̵߳�ʹ�û�����
TInt XThread::_CallBack(TAny *pThread)
{

//	pthread_key_t key;
//	for(int i=0;i<5;i++)
//		{
//		pthread_key_create(&key,NULL);
//		}
    // 1. Add cleanup stack support.
    CTrapCleanup* cleanupStack = CTrapCleanup::New();
 
    // 2. Get pointer to thread host
    XThread* host = (XThread*)pThread;
    
    TRAPD(err,
        // 3. Add support for active objects
        CActiveScheduler* activeScheduler = new CActiveScheduler;
        CleanupStack::PushL(activeScheduler);
        CActiveScheduler::Install(activeScheduler);
        
        //create a session with the font and bitmap server
        RFbsSession iFbsS;
        iFbsS.Connect();
//        User::LeaveIfError(iFbsS.Connect());
 
        // 4. Create and start CPeriodic class that is executed in this thread
//        CPeriodic* periodic = CPeriodic::NewL(CActive::EPriorityLow);
//        CleanupStack::PushL(periodic);
//        periodic->Start(host->Interval(),host->Interval(),
//        TCallBack(host->Periodic, host));
 
        // NOTE: When adding CActiveScheduler support for threads we have to
        // add atleast one active object in it or it fails on 
        // CActiveScheduler::Start().
        // CPeriodic is derived from CActive active object so that is good for
        // this example.
        
        // the thread function well be start.
        CActiveScheduler::Add(host);
        host->Starter();
        // 5. --> Thread execution starts
        CActiveScheduler::Start();
        // 6. --> Thread execution ends (waiting for CActiveScheduler::Stop())
 
        iFbsS.Disconnect();
//        CleanupStack::PopAndDestroy(periodic);
        CleanupStack::PopAndDestroy(activeScheduler);
//        delete activeScheduler;
        );
 
    REComSession::FinalClose();          //���ӿڲ��ʹ�õ��ڴ�
    delete cleanupStack;
    host->ErrExecuted(err);
    return KErrNone;
}

//TInt Periodic(TAny *pThread)
//	{
//	XThread *tempThread;
//	tempThread=(XThread*)pThread;
//	TInt nID;
//    tempThread->CallBack();
//	CActiveScheduler::Stop();
//	return nID;
//	}

void XThread::ErrExecuted(TInt aErr)
	{
//	 m_hStop.Release();
//	 m_hStoped.Release();
//	 m_WaitSemaphore.Release();
//	 m_FreeSemaphore.Release();
//	 m_FreedSemaphore.Release();
//	 if(m_pErrObserver)
//		 {
//		 m_FreedSemaphore.Create();
//		 m_FreedSemaphore.SetEvent();
//		 m_hStoped.Create();
//		 m_hStoped.SetEvent();
//		 m_pErrObserver->ErrHandle(aErr,(XU32)this);
//		 }
	return;
	}

void XThread::RunL()
	{
#ifdef _SYMBIANDEBUG
	SYMBIANDEB1(DefaultLog,"%s:::is start\r\n",m_hThread.FullName().PtrZ());
#endif
	m_bIsStart = true;
	CallBack();
	if(m_hFont)
		m_hScreenDevice->ReleaseFont(m_hFont);
	if(m_hScreenDevice)
		delete m_hScreenDevice;
	iThreadStarter.Close();
	CActiveScheduler::Stop();
	return;
	}
TInt XThread::RunError(TInt aError)
	{
#ifdef _SYMBIANDEBUG
	SYMBIANDEB1(DefaultLog,"xthread had error----%d/r/n",aError);
#endif
//	iThreadStarter.Close();
//	CActiveScheduler::Stop();
	SYMBIANDEB1(DefaultLog,"xthread had error----%d/r/n",aError);
//	User::Exit(-1);
	exit(aError);
	/*
	if(m_pErrObserver)
		m_pErrObserver->ErrHandle(aError,(XU32)this);
	iThreadStarter.Close();
	Starter();
	*/
//	return aError;
	return aError;
	}
void XThread::DoCancel()
	{
	iThreadStarter.Cancel();
	return;
	}
void XThread::Starter()
	{
	iThreadStarter.CreateLocal();
	iThreadStarter.AfterTicks(iStatus,0);
	SetActive();
	}
//�����߳���
void XThread::SetName(const TDesC &name)
	{
	m_hThreadName = HBufC::NewL(name.Length());
	m_hThreadName->Des().Append(name);
	if(m_hThread.Handle())
		m_hThread.RenameMe(*m_hThreadName);
	}

//�����߳���
bool XThread::SetBase(XBase* base,bool bWait)
	{
	if(base)
		{
		m_pBase = base;
		m_FreedSemaphore.ResetEvent();
		m_FreeSemaphore.ResetEvent();
		m_WaitSemaphore.SetEvent();
		}
	else
		{
		m_FreedSemaphore.ResetEvent();
		m_FreeSemaphore.SetEvent();
		if(bWait)
			{
			while(1)
				{
				if(m_FreedSemaphore.IsEvent(0))
					break;
				m_FreeSemaphore.SetEvent();
				Sleep(200);
				}
			}
		}
	return true;
	}

//����߳��Ƿ���Ҫ�ͷ��߳���
bool XThread::NeedFree(int bWait)
	{
	return m_FreeSemaphore.IsEvent(bWait);
	}

void XThread::CreateScreenDevice()
	{
	if(m_hScreenDevice) return;
	m_hScreenDevice = CFbsScreenDevice::NewL(0,EColor16MA);
	}

CFont* XThread::GetFont(const TFontSpec& fontspec)
	{
	if(!m_hScreenDevice)
		CreateScreenDevice();
	if(m_hFont)
		{
		m_hScreenDevice->ReleaseFont(m_hFont);
		}
	m_hScreenDevice->GetNearestFontInTwips(m_hFont,fontspec);
	return m_hFont;
	}

TInt XThread::GetHandle()
	{
	return pthread_self();
	}
#endif
