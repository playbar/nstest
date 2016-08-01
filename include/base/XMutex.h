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

#ifdef __ANDROID__
#include <pthread.h>
#include <semaphore.h>
#endif

////////////////////////////////////////////////////////
// 事件支持类，用于多线程数据锁定
#define XWAITMONMENT 10
#define XSHORTTIME  100
#define XNORMALTIME	500
#define XLONGTIME	1000

#ifdef _SYMBIAN
// thread specific data space
#define XTSDNUM 5
#endif

class _XEXT_CLASS XMutex
{
public:
	XMutex();
	XBOOL Lock(XU32 dwTimeout = XINFINITE);
	XBOOL UnLock();
	~XMutex();
	int m_nCount;
protected:
#ifdef _SYMBIAN
	RMutex m_iMutex;
	RSemaphore m_iSemaphore;
	bool	m_bLocking;
#elif defined(_WIN32)
	XHANDLE m_hObject;
#elif defined(__APPLE__)
	void LockBlock();
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;
	bool m_isLocked;
	pthread_t m_owner;
	unsigned int m_count;

#elif defined( __ANDROID__ )
//	sem_t  m_sem;
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;
	bool m_isLocked;
	pthread_t m_owner;
	unsigned int m_count;
#endif
};

///////////////////////////////////////////
// 线程锁
#ifdef __ANDROID__
class _XEXT_CLASS XLock
{
public:
	XLock(XMutex*pObject );
	XLock();
	~XLock();
	void SetObject(XMutex*pObj);
public:
	XBOOL Lock(XU32 dwTimeOut= XINFINITE);
	XBOOL UnLock();
	XBOOL IsLocked();

protected:
	XBOOL	 m_bLocked;
	XMutex*  m_pObject;
};
#else
class _XEXT_CLASS XLock
{
// Constructors
public:
	XLock(XMutex*pObject=XNULL)
	{
		m_pObject=pObject;
		m_bLocked=XFALSE;
	}
	~XLock(){}

	void SetObject(XMutex*pObj){m_pObject=pObj;}

public:
	XBOOL Lock(XU32 dwTimeOut= XINFINITE)
	{
		if(m_pObject==XNULL)
			return XFALSE;
		if(m_bLocked)
			return XTRUE;
		m_bLocked=m_pObject->Lock(dwTimeOut);
		return m_bLocked;
	}
	XBOOL UnLock()
	{
		if(m_pObject==XNULL)
			return XFALSE;
		if(m_bLocked)
		  m_bLocked= !m_pObject->UnLock();
		return !m_bLocked;
	}
	XBOOL IsLocked(){return m_bLocked;}

protected:
	XBOOL	 m_bLocked;
	XMutex*  m_pObject;
};

#endif

class _XEXT_CLASS XBase  
{
public:
	XBase(){}
	virtual ~XBase(){}
	virtual XBOOL NeedStop()=0;
public:
	virtual XUINT CallBack(XU32 nID) {return nID;}
	virtual void ReleaseCall(){}
	virtual void StartCall(){}
};


#ifdef _SYMBIAN
class XEvent : public CBase
#else
//事件对象，用于全局互斥
//简单的模拟了P，V操作

class XEvent
#endif
{
public:
#ifdef __APPLE__
	XBOOL IsEmpty();
	XEvent();
#elif defined(_SYMBIAN)
	XBOOL IsEmpty(){return !m_bIsCreate;}
	XEvent(){m_bIsCreate=XFALSE; m_bEvent=XFALSE;}
#elif defined(_WIN32)
	XBOOL IsEmpty(){return m_hObject==XNULL;}
	XEvent(){m_hObject=XNULL;}
#elif defined( __ANDROID__ )
	XBOOL IsEmpty();	//TODO by hgl
	XEvent();
#endif
	XBOOL IsEvent(XU32 t);
	void SetEvent();
	void ResetEvent();
	void Release();
	void Create();

	~XEvent();
protected:
#ifdef _SYMBIAN
	RMutex m_iLock;
	RSemaphore m_iObject;
	bool	m_bEvent;
	bool	m_bIsCreate;
#elif defined(_WIN32)
	XHANDLE m_hObject;
#elif defined(__APPLE__)
	bool m_event;
	bool m_created;
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;
#elif defined(__ANDROID__)
	bool m_event;
	bool m_created;
//	sem_t  m_sem;
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
class CFbsScreenDevice;
class CFont;
class TFontSpec;
class _XEXT_CLASS XThread : public CActive
#elif defined(_WIN32)
class _XEXT_CLASS XThread
#elif defined(__APPLE__)
class _XEXT_CLASS XThread
#elif defined(__ANDROID__)
class _XEXT_CLASS XThread
#endif
{
#ifdef _SYMBIAN
public:
class MErrObserver
	{
	public:
		virtual int ErrHandle(int err,XU32 pData)=0;
	};
#endif
protected:
#ifdef _SYMBIAN
	static TInt _CallBack(TAny *pThread);
	void Starter();
//	inline TTimeIntervalMicroSeconds32 Interval(){return iInterval;};
	void RunL();                            //CActive virtual function     
	TInt RunError(TInt aError);             //CActive virtual function
	void DoCancel();                          //CActive virtual function
	void ErrExecuted(TInt aErr);            //异常捕获处理
public:
	void CreateScreenDevice();
	CFbsScreenDevice* GetScreenDevice(){return m_hScreenDevice;}
	CFont* GetFont(const TFontSpec& fontspec);
	TInt GetHandle();
protected:
	CFbsScreenDevice* m_hScreenDevice;
	CFont* m_hFont;
#elif defined(_WIN32)
	static XUINT _CallBack(XThread*pThread) {return pThread->CallBack();}
#elif defined(__APPLE__)
	static void* _CallBack(void*pThread) {((XThread*)pThread)->CallBack(); return 0;}	
#elif defined( __ANDROID__ )
	static void* _CallBack(void*pThread) {((XThread*)pThread)->CallBack(); return 0;}
#endif

	XUINT  CallBack();
	
public:
	static void Sleep(XU32 t);
	XBOOL IsRunning()
	{
#ifdef _SYMBIAN
	return m_hThread.Handle() != XNULL;
#elif defined(_WIN32)
	return m_hThread!=XNULL;
#elif defined(__APPLE__)
	return m_hasThread;
#elif defined( __ANDROID__ )
	return m_hasThread;
#endif
	}
	
	XBOOL IsStoped()
	{
#ifdef _SYMBIAN
		return m_hThread.Handle()==XNULL || m_hStoped.IsEvent(0);
#elif defined(_WIN32)
		return m_hThread==XNULL||m_hStoped.IsEvent(0);
#elif defined(__APPLE__)
		return !m_hasThread || m_hStoped.IsEvent(0);
#elif defined ( __ANDROID__ )
		return !m_hasThread || m_hStoped.IsEvent(0);
#endif
	}

	XBOOL NeedStop(int iWait=0);
	XBOOL Stop(XBOOL bWait=XTRUE);
	void Release(XBOOL bWait=XTRUE);
	XBOOL Create(XBase* pBase,XU32 nID);
	XThread(XBOOL bDelete=XFALSE);
	virtual ~XThread();
#ifdef _SYMBIAN
	friend class XThreadPool;
	void SetErrHandle(MErrObserver* observer){m_pErrObserver = observer;}
public:
	void SetName(const TDesC &name);			//ÉèÖÃÏß³ÌÃû×Ö
	bool IsStart(){return m_bIsStart;}			//²é¿´Ïß³ÌÊÇ·ñ¼¤»î
	bool IsBusy(){return m_pBase!=XNULL;}		//²é¿´Ïß³ÌÊÇ·ñÎªÕ¼ÓÐ×´Ì¬
	bool NeedFree(int bWait=0);							//¼ì²éÏß³ÌÊÇ·ñÐèÒªÊÍ·ÅÏß³ÌÌå
protected:
	bool SetBase(XBase* base,bool bWait = true);					//ÉèÖÃÏß³ÌÌå
	XEvent m_WaitSemaphore;						//Ïß³Ì¹ÒÆðÐÅºÅ
	XEvent m_FreeSemaphore,m_FreedSemaphore;	//Í¨ÖªÏß³Ì¹ÒÆðÐÅºÅ
	bool m_bTSDReady;							//when thread specific data is ready,the value.
	unsigned int m_rTSDkey[XTSDNUM];
public:
	HBufC *m_hThreadName;						//Ïß³ÌÃû
#endif
protected:
	XBase* m_pBase;
	XU8	   m_bDelete;
#ifdef _SYMBIAN
	RThread m_hThread;
//	TTimeIntervalMicroSeconds32     iInterval;    //线程存活最短时间
	RTimer  iThreadStarter;
	bool		m_bIsStart;						  //Ïß³Ì¼¤»î±êÖ¾
//	XU8    m_bThreadType;
#elif defined(_WIN32)
	XHANDLE m_hThread;
#endif
	
#ifdef __APPLE__
	pthread_t m_threadId;
	bool m_hasThread;
#endif

#ifdef __ANDROID__
	pthread_t m_threadId;
	bool m_hasThread;
#endif
	XEvent m_hStop;
	XEvent m_hStoped;
	XU32   m_nThreadID;
#ifdef _SYMBIAN
	MErrObserver* m_pErrObserver;
#endif
};



#endif // !defined(AFX_XMUTEX_H__B8D9CAC1_4EC8_469F_9833_4C5F5586B96E__INCLUDED_)
