// XMutex.cpp: implementation of the XMutex class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XMutex.h"
#ifdef __ANDROID__
//#include <unistd.h>
#include <errno.h>
#endif

#include <time.h>
#include <sys/time.h>
#include <unistd.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#if 0
#define LOGWHEREXMutex()  LOGWHERE()
#else
#define LOGWHEREXMutex()
#endif


XMutex::XMutex()
{
	LOGWHEREXMutex();
#ifdef __ANDROID__
	pthread_mutexattr_t mattr;
	pthread_mutexattr_init(&mattr);
	pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE); // PTHREAD_MUTEX_RECURSIVE  PTHREAD_MUTEX_INITIALIZER
	pthread_mutex_init(&m_mutex, &mattr);
	//sem_init( &m_sem, 0, 1 );
	pthread_cond_init(&m_cond, 0);
	m_isLocked = false;
	m_count = 0;
#else
	m_hObject = ::CreateMutex(XNULL,0,XNULL);
	m_nCount=0;
#endif
}

XBOOL XMutex::Lock(XU32 dwTimeout)
{
	LOGWHEREXMutex();
#ifdef __ANDROID__
	//sem_trywait( &m_sem );
	pthread_mutex_lock(&m_mutex);
	if (m_isLocked && pthread_equal(m_owner, pthread_self()))
	//if ( m_isLocked )
	{
		LOGWHEREXMutex();
		++m_count;
		//sem_post(&m_sem);
		pthread_mutex_unlock(&m_mutex);
		return true;
	}
	LOGWHEREXMutex();
	while (m_isLocked)
	{
		LOGWHEREXMutex();
		timespec timeout;
		timeout.tv_sec = dwTimeout / 1000;
		timeout.tv_nsec = (dwTimeout % 1000) * 1000000;
		struct timeval    tp;
		gettimeofday(&tp, NULL);
		timeout.tv_sec += tp.tv_sec;
		timeout.tv_nsec += tp.tv_usec * 1000;
		if (timeout.tv_nsec > 1000 * 1000 * 1000)
		{
			timeout.tv_nsec -= 1000 *1000 * 1000;
			timeout.tv_sec++;
		}
		LOGWHEREXMutex();
		int const cond_res = pthread_cond_timedwait(&m_cond, &m_mutex, &timeout);
		//int const cond_res = pthread_cond_timedwait(&m_cond, NULL, &timeout);
		LOGWHEREXMutex();
		if (cond_res == ETIMEDOUT)
		{
			//sem_post(&m_sem);
			pthread_mutex_unlock(&m_mutex);
			return XFALSE;
		}
	}
	LOGWHEREXMutex();
	m_isLocked = true;
	++m_count;
	m_owner = pthread_self();
	//sem_post(&m_sem);
	pthread_mutex_unlock(&m_mutex);
	return XTRUE;
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
	LOGWHEREXMutex();
#ifdef __ANDROID__
	pthread_mutex_lock(&m_mutex);
	//sem_trywait( &m_sem );
	if ( --m_count == 0 )
	{
		m_isLocked = false;
	}
	pthread_cond_signal(&m_cond);
	pthread_mutex_unlock(&m_mutex);
	//sem_post(&m_sem);
	return XTRUE;
#else
	m_nCount--;
	return ::ReleaseMutex(m_hObject);
#endif
}

XMutex::~XMutex()
{
	LOGWHEREXMutex();
#ifdef __ANDROID__
	//sem_destroy( &m_sem );
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy( &m_cond );
#else
	if(m_hObject!=XNULL)
	{
		CloseHandle(m_hObject);
		m_hObject=XNULL;
	}
#endif
}

/////////////////////////////////////

XLock::XLock(XMutex*pObject )
{
	LOGWHEREXMutex();
	m_pObject = pObject;
	m_bLocked = XFALSE;
}

XLock::XLock( )
{
	LOGWHEREXMutex();
	m_pObject = NULL;
	m_bLocked = XFALSE;
}

XLock::~XLock()
{
	LOGWHEREXMutex();
}

void XLock::SetObject(XMutex*pObj)
{
	LOGWHEREXMutex();
	m_pObject=pObj;
}

XBOOL XLock::Lock(XU32 dwTimeOut)
{
	LOGWHEREXMutex();
	if(m_pObject==XNULL)
		return XFALSE;
	if(m_bLocked)
		return XTRUE;
	m_bLocked=m_pObject->Lock(dwTimeOut);
	return m_bLocked;
}

XBOOL XLock::UnLock()
{
	LOGWHEREXMutex();
	if(m_pObject==XNULL)
		return XFALSE;
	if(m_bLocked)
	{
	  m_pObject->UnLock();
	  m_bLocked =false;
	}
	return !m_bLocked;
}

XBOOL XLock::IsLocked()
{
	LOGWHEREXMutex();
	return m_bLocked;
}

///////////////////////


XEvent::XEvent()
{
	LOGWHEREXMutex();
#ifdef __ANDROID__
	m_event = false;
	m_created = false;
#else
	m_hObject = XNULL;
#endif
}

XEvent::~XEvent()
{
	LOGWHEREXMutex();
	Release();
}

XBOOL XEvent::IsEmpty()
{
	LOGWHEREXMutex();
#ifdef __ANDROID__
	return !m_created;
#else
	return m_hObject==XNULL;
#endif
}

void XEvent::Create()
{
	LOGWHEREXMutex();
#ifdef __ANDROID__
	pthread_mutexattr_t mattr;
	pthread_mutexattr_init(&mattr);
	pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
	int ret = pthread_mutex_init(&m_mutex, &mattr);
	//sem_init( &m_sem, 0, 1 );
	pthread_cond_init(&m_cond, 0);
	m_event = false;
	m_created = true;
#else
	m_hObject=::CreateEvent(XNULL,1,0,XNULL);
#endif
}

void XEvent::Release()
{
	LOGWHEREXMutex();
#ifdef __ANDROID__
	m_event = false;
	if (m_created)
	{
		//sem_destroy( &m_sem );
		pthread_mutex_destroy(&m_mutex);
		pthread_cond_destroy(&m_cond);
		m_created = false;
	}

#else
	if(m_hObject!=XNULL)
		::CloseHandle(m_hObject);
	m_hObject=XNULL;
#endif
}

void XEvent::ResetEvent()
{
	LOGWHEREXMutex();
#ifdef __ANDROID__
	if (!m_created)
		return;
	//sem_trywait( &m_sem );
	pthread_mutex_lock(&m_mutex);
	if (m_event)
	{
		m_event = false;
	}
	//sem_post(&m_sem);
	pthread_mutex_unlock(&m_mutex);
#else
	if(m_hObject!=XNULL)
	::ResetEvent(m_hObject);
#endif
}

void XEvent::SetEvent()
{
	LOGWHEREXMutex();
#ifdef __ANDROID__
	//sem_trywait( &m_sem );
	pthread_mutex_lock(&m_mutex);
	if (!m_event)
	{
		m_event = true;
		pthread_cond_broadcast(&m_cond);
	}
	//sem_post(&m_sem);
	pthread_mutex_unlock(&m_mutex);
#else
	if(m_hObject!=XNULL)
		::SetEvent(m_hObject);
#endif
}

XBOOL XEvent::IsEvent(XU32 t)
{
	LOGWHEREXMutex();
#ifdef __ANDROID__
	pthread_mutex_lock(&m_mutex);
	//sem_trywait( &m_sem );
	if (m_event)
	{
		//sem_post(&m_sem);
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
		//int const cond_res = pthread_cond_timedwait(&m_cond, NULL, &timeout);
		if (cond_res == ETIMEDOUT)
		{
			//sem_post(&m_sem);
			pthread_mutex_unlock(&m_mutex);
			return XFALSE;
		}
		else
		{
			//sem_post(&m_sem);
			pthread_mutex_unlock(&m_mutex);
			return XTRUE;
		}
	}
#else
   return WaitForSingleObject(m_hObject,t)==WAIT_OBJECT_0;
#endif
}

void XThread::Sleep(XU32 t)
{
	LOGWHEREXMutex();
#ifdef __ANDROID__
	usleep( t * 1000 );
#else
	::Sleep(t);
#endif
	return;
}

XThread::XThread(XBOOL bDelete)
{
	LOGWHEREXMutex();

	m_bDelete=bDelete;

	m_pBase=XNULL;
#ifdef __ANDROID__
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
	LOGWHEREXMutex();
   Release();
}

XBOOL XThread::Create(XBase* pBase,XU32 nID)
{
	LOGWHEREXMutex();
	 Release();
	 m_nThreadID=nID;
	 m_pBase=pBase;
	 m_hStop.Create();
	 m_hStoped.Create();
#ifdef __ANDROID__
	pthread_attr_t threadAttr;
	pthread_attr_init(&threadAttr);

	pthread_create(&m_threadId, &threadAttr, _CallBack, this);

	pthread_attr_destroy(&threadAttr);
	m_hasThread = true;
	//LOGWHEREVAL( (int)m_threadId );

#else
	 m_hThread=::CreateThread(XNULL,0,(LPTHREAD_START_ROUTINE)_CallBack,
				this,CREATE_SUSPENDED,&m_nThreadID);
	 ::ResumeThread(m_hThread);
#endif
	 return XTRUE;
}

XUINT XThread::CallBack()
{
	//LOGWHEREXMutex();
	m_hStop.ResetEvent();
	m_hStoped.ResetEvent();
	XUINT nID=m_nThreadID;

	while(1)
	{
		if(NeedStop())
			break;
		m_pBase->CallBack(nID);
		Sleep( 10 );
	}
	m_hStoped.SetEvent();
	if(m_bDelete)
	{
		if(m_pBase!=XNULL)
			delete m_pBase;
		m_pBase=XNULL;
	}
	return nID;
}

void XThread::Release(XBOOL bWait)
{
	LOGWHEREXMutex();
	Stop(bWait);
#ifdef __ANDROID__
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
	LOGWHEREXMutex();
	if(m_hStop.IsEmpty())
		return XTRUE;
	return m_hStop.IsEvent(nWait);
}

XBOOL XThread::Stop(XBOOL bWait)
{
	LOGWHEREXMutex();
   if(IsStoped())
	   return XTRUE;
#ifdef __ANDROID__
	if (m_hasThread)
#else
   if(m_hThread!=XNULL)
#endif
	{
		m_hStop.SetEvent();
		if(bWait)
		{
			while( true )
			{
				Sleep(10);
				if(m_hStoped.IsEvent(0))
					break;
				m_hStop.SetEvent();
			}
		}
		m_hasThread = false;
	}

   return XTRUE;

}


