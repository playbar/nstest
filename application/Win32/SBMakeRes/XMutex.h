#ifndef _XMUTEX_
#define _XMUTEX_

#include "GGType.h"

#ifdef _SYMBIAN
#include <e32cmn.h>
#include <unistd.h>
#include <e32std.h>
#include <e32base.h>
#endif

#ifdef __APPLE__
#include <pthread.h>
#endif

////////////////////////////////////////////////////////
// 事件支持类，用于多线程数据锁定
#define XWAITMONMENT 10
#define XSHORTTIME  100
#define XNORMALTIME	500
#define XLONGTIME	1000

#ifdef _SYMBIAN
////////////////////////////////////////////////////////
//互斥量，对Symbian下的RMutex的扩展
class ZCMutex :public RMutex
{
public:
	TBool WaitForSingle(TInt waitTime)
	{
		int i;
		i=waitTime/100;
		if(i<1)
		{
			if(RMutex::IsHeld())
				return RMutex::IsHeld();
			else
				Wait();
			return true;
		}
		else
		{
			while(i>=0)
				{
				if(RMutex::IsHeld()){i--; User::After(100);}
				else { Wait(); return true;}
				}
			return false;
		}
	}
	
	TBool IsHeld()
	{
		return RMutex::IsHeld();
	}
	
	TBool IsHeld(TInt waitTime)
	{
		int i;
		i=waitTime/100;
		if(i<1) return RMutex::IsHeld();
		else
		{
			while(i)
			{
				if(RMutex::IsHeld()) return true;
				else
				{
				i--; User::After(100);
				}
			}
		}
		return RMutex::IsHeld();
	}
};
#endif

class XMutex
{
public:
	XMutex();
	XBOOL Lock(XU32 dwTimeout = XINFINITE);
	XBOOL UnLock();
	~XMutex();
	int m_nCount;
protected:
#ifdef _SYMBIAN
	ZCMutex m_hObject;
#elif defined(_WIN32)
	XHANDLE m_hObject;
#elif defined(__APPLE__)
	pthread_mutex_t m_mutex;
#endif
};

///////////////////////////////////////////
// 线程锁

class _XEXT_CLASS XLock
{
// Constructors
public:
	XLock(XMutex*pObject=XNULL)
	{
		m_pObject=pObject;
		m_bLocked=XFALSE;
	}
	void SetObject(XMutex*pObj){m_pObject=pObj;}
// Operations
public:
	XBOOL Lock(XU32 dwTimeOut= XINFINITE)
	{
		if(m_pObject==XNULL) return XFALSE;
		if(m_bLocked) return XTRUE;
	   	m_bLocked=m_pObject->Lock(dwTimeOut);
		return m_bLocked;
	}
	XBOOL UnLock()
	{
		if(m_pObject==XNULL) return XFALSE;
		if(m_bLocked)
		  m_bLocked=!m_pObject->UnLock();
		return !m_bLocked;
	}
	XBOOL IsLocked(){return m_bLocked;}

// Implementation
public:
	~XLock()
	{	
	}

protected:
	XBOOL	 m_bLocked;
	XMutex*  m_pObject;
};


class _XEXT_CLASS XBase  
{
public:
	XBase(){}
	virtual ~XBase(){}
public:
	virtual XUINT CallBack(XU32 nID) {return nID;}
};

//#ifdef _SYMBIAN
//class _XEXT_CLASS XBase : public CActive
//{
//public:
//	XBase();
//	virtual ~XBase();
//	
//	void Start(XU32 nID);
//public:
//	virtual XUINT CallBack(XU32 nID) {return nID;}
//protected:
//	void RunL();
//	void DoCancel();
//protected:
//	RTimer iThreadStarter;
//	XU32   m_nID;
//};
//#endif

//事件对象，用于全局互斥
//简单的模拟了P，V操作
class XEvent
{
public:
#ifdef __APPLE__
	XBOOL IsEmpty();
	XEvent();
#else
	XBOOL IsEmpty(){return m_hObject==XNULL;}
	XEvent(){m_hObject=XNULL;}
#endif
	XBOOL IsEvent(XU32 t);
	void SetEvent();
	void ResetEvent();
	void Release();
	void Create();

	~XEvent();
protected:
#ifdef _SYMBIAN
	RMutex m_hLock;
	unsigned int *m_hObject;
#elif defined(_WIN32)
	XHANDLE m_hObject;
#elif defined(__APPLE__)
	bool m_event;
	bool m_created;
	
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;
	
#endif
};

///////////////////////////////////////////////////////////////////////////
//the type for thread
#ifdef _SYMBIAN
#define KISNONE 	0x00
#define KISACTIVE	0x01 
#endif

#ifdef _SYMBIAN
class _XEXT_CLASS XThread : public CActive
#elif defined(_WIN32)
class _XEXT_CLASS XThread
#elif defined(__APPLE__)
class _XEXT_CLASS XThread
#endif
{
protected:
#ifdef _SYMBIAN
	static TInt _CallBack(TAny *pThread);
//	static TInt Periodic(TAny *pThread)
//	{
//		XThread* mythread = (XThread*)pThread;
//		if (mythread)
//        {
//			// Thread is executed once so time to stop it
//			CActiveScheduler::Stop();
//			// After this execution continues from CActiveScheduler::Start()
//        }
//		// Does not continue again 
//		// Note: Does not work with this CPeriodic class
//		return EFalse;
//	}
	void Starter();
//	inline TTimeIntervalMicroSeconds32 Interval(){return iInterval;};
	void RunL();                            //CActive virtual function     
	TInt RunError(TInt aError);             //CActive virtual function
	void DoCancel();                          //CActive virtual function
	void ErrExecuted(TInt aErr);            //异常捕获处理
#elif defined(_WIN32)
	static XUINT _CallBack(XThread*pThread) {return pThread->CallBack();}
#elif defined(__APPLE__)
	static void* _CallBack(void*pThread) {((XThread*)pThread)->CallBack(); return 0;}	
#endif
	

	
	XUINT  CallBack();
	
public:
	static void Sleep(XU32 t);
	XBOOL IsRunning()
	{
#ifdef _SYMBIAN
//	return m_bThreadType != KISNONE;
	return m_hThread.Handle() != XNULL;
#elif defined(_WIN32)
	return m_hThread!=XNULL;
#elif defined(__APPLE__)
	return m_hasThread;
#endif
	}
	
	
	XBOOL IsStoped()
	{
#ifdef _SYMBIAN
//		return m_bThreadType==KISNONE || m_hStoped.IsEvent(0);
		return m_hThread.Handle()==XNULL || m_hStoped.IsEvent(0);
//		XBOOL test;
//		test=NeedStop();
//		test=m_hStoped.IsEvent(0);
//		return test;
#elif defined(_WIN32)
		return m_hThread==XNULL||m_hStoped.IsEvent(0);
#elif defined(__APPLE__)
		return !m_hasThread || m_hStoped.IsEvent(0);
#endif
	}
	
	
	XBOOL NeedStop(int iWait=0);
	XBOOL Stop(XBOOL bWait=XTRUE);
	void Release(XBOOL bWait=XTRUE);
	XBOOL Create(XBase* pBase,XU32 nID);
	XThread(XBOOL bDelete=XFALSE);
	virtual ~XThread();
protected:
	XBase* m_pBase;
	XU8	   m_bDelete;
#ifdef _SYMBIAN
	RThread m_hThread;
//	TTimeIntervalMicroSeconds32     iInterval;    //线程存活最短时间
	RTimer  iThreadStarter;
//	XU8    m_bThreadType;
#elif defined(_WIN32)
	XHANDLE m_hThread;
#endif
	
#ifdef __APPLE__
	pthread_t m_threadId;
	bool m_hasThread;
#endif
	XEvent m_hStop,m_hStoped;
	XU32   m_nThreadID;
};



#endif // !defined(AFX_XMUTEX_H__B8D9CAC1_4EC8_469F_9833_4C5F5586B96E__INCLUDED_)
