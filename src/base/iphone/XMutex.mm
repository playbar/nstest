// XMutex.cpp: implementation of the XMutex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxggbase.h"
#include "XMutex.h"
#ifdef __APPLE__
//#include <unistd.h>
#include <errno.h>
#endif

#include <time.h>
#include <sys/time.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define USE_COND

XMutex::XMutex()
{
#ifdef __APPLE__
	pthread_mutexattr_t mattr;
	pthread_mutexattr_init(&mattr);
	
	
	pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
	int ret = pthread_mutex_init(&m_mutex, &mattr);
	assert(ret == 0);
#ifdef USE_COND
	pthread_cond_init(&m_cond, 0);
	m_isLocked = false;
	m_count = 0;
#endif
	
#else
	m_hObject = ::CreateMutex(XNULL,0,XNULL);
#endif
	m_nCount=0;
}

void XMutex::LockBlock()
{
	pthread_mutex_lock(&m_mutex);
	if (m_isLocked && pthread_equal(m_owner, pthread_self()))
	{
		++m_count;
		pthread_mutex_unlock(&m_mutex);
		return;
	}
	
	while (m_isLocked) 
	{
		pthread_cond_wait(&m_cond, &m_mutex);
	}
	m_isLocked = true;
	++m_count;
	m_owner = pthread_self();
	pthread_mutex_unlock(&m_mutex);
}

XBOOL XMutex::Lock(XU32 dwTimeout)
{
#ifdef __APPLE__
#ifdef USE_COND
//	if (dwTimeout == 0)
//	{
//		LockBlock();
//		return XTRUE;
//	}
	pthread_mutex_lock(&m_mutex);
	
	if (m_isLocked && pthread_equal(m_owner, pthread_self()))
	{
		++m_count;
		pthread_mutex_unlock(&m_mutex);
		return true;
	}
	
	while (m_isLocked) {
		timespec timeout;
		timeout.tv_sec = dwTimeout / 1000;
		timeout.tv_nsec = (dwTimeout % 1000) * 1000000;
		struct timeval    tp;
		gettimeofday(&tp, NULL);
		timeout.tv_sec += tp.tv_sec;
		timeout.tv_nsec += tp.tv_usec * 1000;
		if (timeout.tv_nsec > 1000000000)
		{
			timeout.tv_nsec -= 1000000000;
			timeout.tv_sec++;
		}
		
		int const cond_res = pthread_cond_timedwait(&m_cond, &m_mutex, &timeout);
		if (cond_res == ETIMEDOUT)
		{
			pthread_mutex_unlock(&m_mutex);
			return XFALSE;
		}
		assert(!cond_res);
	}
	m_isLocked = true;
	++m_count;
	m_owner = pthread_self();
	pthread_mutex_unlock(&m_mutex);
	return XTRUE;
#else
	int count = dwTimeout / 10;
	int status = pthread_mutex_trylock(&m_mutex);
	while (status == EBUSY && count > 0) {
		[NSThread sleepForTimeInterval:0.01];
		count --;
		status = pthread_mutex_trylock(&m_mutex);
	}
	if (status == 0)
	{
		m_nCount++;
		return XTRUE;
	}
	else {
		return false;
	}
#endif

#else
	if (::WaitForSingleObject(m_hObject, dwTimeout) == WAIT_OBJECT_0)
	{
		m_nCount++;
		return XTRUE;
	}
	else
		return XFALSE;
#endif
	
}

XBOOL XMutex::UnLock()
{
	m_nCount--;
#ifdef __APPLE__
#ifdef USE_COND
	pthread_mutex_lock(&m_mutex);
	
	if (!--m_count)
	{
		m_isLocked = false;
	}

	pthread_cond_signal(&m_cond);
	
	pthread_mutex_unlock(&m_mutex);
	return XTRUE;
#else
	return pthread_mutex_unlock(&m_mutex);
#endif
#else
	return ::ReleaseMutex(m_hObject);
#endif
}

XMutex::~XMutex()
{
#ifdef __APPLE__
	pthread_mutex_destroy(&m_mutex);
#else
	if(m_hObject!=XNULL)
	{
		CloseHandle(m_hObject);
		m_hObject=XNULL;
	}
#endif
}

XThread::XThread(XBOOL bDelete)
{
	m_bDelete=bDelete;

	m_pBase=XNULL;
#ifdef __APPLE__
	m_threadId = 0;
	m_nThreadID = 0;
	m_hasThread = false;
#else
	m_hThread=XNULL;
	m_nThreadID=0;
#endif
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
#ifdef __APPLE__
	pthread_attr_t threadAttr;
	pthread_attr_init(&threadAttr);
	
	pthread_create(&m_threadId, &threadAttr, _CallBack, this);

	pthread_attr_destroy(&threadAttr);
	m_hasThread = true;
	return true;
#else
	 m_hThread=::CreateThread(XNULL,0,(LPTHREAD_START_ROUTINE)_CallBack,
				this,CREATE_SUSPENDED,&m_nThreadID);
	 ::ResumeThread(m_hThread);
#endif
	 return XTRUE;
}

XUINT XThread::CallBack()
{
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	m_hStop.ResetEvent();
	m_hStoped.ResetEvent();
	XUINT nID=m_nThreadID;

	while(1)
	{
		if(NeedStop()) break;

		m_pBase->CallBack(nID);
		[NSThread sleepForTimeInterval:0.01];
	}	 
	m_hStoped.SetEvent();
	if(m_bDelete) 
	{
		if(m_pBase!=XNULL) delete m_pBase;
		m_pBase=XNULL;
	}
	[pool release];
	return nID;
}

void XThread::Release(XBOOL bWait)
{
	Stop(bWait);
#ifdef __APPLE__
	if (m_hasThread)
		pthread_exit(0);
	m_hasThread = false;
#else
	if(m_hThread!=XNULL)
	{
		CloseHandle(m_hThread);
		m_hThread=XNULL;
	}
#endif
	m_hStop.Release();
	m_hStoped.Release();
}

XBOOL XThread::NeedStop(int nWait)
{
	if(m_hStop.IsEmpty()) return XTRUE;
	return m_hStop.IsEvent(nWait);
}

XBOOL XThread::Stop(XBOOL bWait)
{
   if(IsStoped()) return XTRUE;
#ifdef __APPLE__
//	if (m_hThread != XNULL)
	if (m_hasThread)
#else
   if(m_hThread!=XNULL)
#endif
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
#ifdef __APPLE__
	m_hasThread = false;
#endif
   return XTRUE;
}


//XBOOL XThread::IsStoped()
//{return m_hThread==XNULL||WaitForSingleObject(m_hStoped,0)==WAIT_OBJECT_0;}

void XEvent::Create()
{
#ifdef __APPLE__
	
	m_created = true;
	pthread_mutexattr_t mattr;
	pthread_mutexattr_init(&mattr);
	pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
	int ret = pthread_mutex_init(&m_mutex, &mattr);
	assert(ret == 0);
	m_event = false;
	
	pthread_cond_init(&m_cond, 0);
	
	
//	pthread_condattr_t condattr;
//	pthread_condattr_init(&condattr);
//	pthread_cond_init(&m_cond, &condattr);
	
#else
	m_hObject=::CreateEvent(XNULL,1,0,XNULL);
#endif
}

void XEvent::Release()
{
#ifdef __APPLE__
	m_event = false;
	if (m_created)
	{
		pthread_mutex_destroy(&m_mutex);
		pthread_cond_destroy(&m_cond);
		m_created = false;
	}
	
#else
	if(m_hObject!=XNULL) ::CloseHandle(m_hObject);
	m_hObject=XNULL;
#endif
}

XEvent::XEvent()
{
#ifdef __APPLE__
	m_event = false;
	m_created = false;
#else
	m_hObject = XNULL;
#endif
}

XEvent::~XEvent()
{
	Release();
}

XBOOL XEvent::IsEmpty()
{
#ifdef __APPLE__
	return !m_created;
#else
	return m_hObject==XNULL;
#endif
}

void XEvent::ResetEvent()
{
#ifdef __APPLE__
//	assert(m_created);
	if (!m_created)
		return;
	pthread_mutex_lock(&m_mutex);
	
	if (m_event)
	{
		m_event = false;
	}
	pthread_mutex_unlock(&m_mutex);
#else
	if(m_hObject!=XNULL)
	::ResetEvent(m_hObject);
#endif
}

void XEvent::SetEvent()
{
#ifdef __APPLE__
	APPLEASSERT(m_created);
	pthread_mutex_lock(&m_mutex);
	if (!m_event)
	{
		m_event = true;
		pthread_cond_broadcast(&m_cond);
	}
	pthread_mutex_unlock(&m_mutex);
#else
	if(m_hObject!=XNULL)
		::SetEvent(m_hObject);
#endif
}

XBOOL XEvent::IsEvent(XU32 t)
{
#ifdef __APPLE__
	assert(m_created);
	pthread_mutex_lock(&m_mutex);
	
	if (m_event)
	{
		pthread_mutex_unlock(&m_mutex);
		return XTRUE;
	}
	else
	{
		timespec timeout;
		timeout.tv_sec = t / 1000;
		timeout.tv_nsec = (t % 1000) * 1000000;
		struct timeval    tp;
		gettimeofday(&tp, NULL);
		timeout.tv_sec += tp.tv_sec;
		timeout.tv_nsec += tp.tv_usec * 1000;
		if (timeout.tv_nsec > 1000000000)
		{
			timeout.tv_nsec -= 1000000000;
			timeout.tv_sec++;
		}
		
		int const cond_res = pthread_cond_timedwait(&m_cond, &m_mutex, &timeout);
		if (cond_res == ETIMEDOUT)
		{
			pthread_mutex_unlock(&m_mutex);
			return XFALSE;
		}
		else
		{
			pthread_mutex_unlock(&m_mutex);
			assert(!cond_res);
			return XTRUE;
		}
	}
	
//	int count = t / 10;

//	bool flag = m_event;
//	while (!flag && count > 0) {
//		[NSThread sleepForTimeInterval:0.01];
//		count --;
//		flag = m_event;
//	}
//	return flag;

#else
   return WaitForSingleObject(m_hObject,t)==WAIT_OBJECT_0;
#endif
}

void XThread::Sleep(XU32 t)
{
#ifdef __APPLE__
//	sleep(t);
	NSTimeInterval interval = (float)t * .001;
	[NSThread sleepForTimeInterval:interval];
	
#else
	::Sleep(t);
#endif
}
