/*
 ============================================================================
 Name		: XThreadPool.h
 Author	  : zhangchen
 Version	 : 1.0
 Copyright   : �̳߳�
 Description : CXThreadPool declaration
 ============================================================================
 */

#ifndef XTHREADPOOL_H
#define XTHREADPOOL_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "XMutex.h"
#include "XVector.h"
#include "CAfterTimer.h"

typedef XArray<XThread*> threadArray;

// CLASS DECLARATION

/**
 *  CXThreadPool
 * 
 */
class XThreadPool : public CBase , public CAfterTimer::MTimerObserver , public XThread::MErrObserver
	{
public:
	class MObserver
		{
		public:
		virtual int Message(int msg)=0;
		};
	enum{EStartPool,EErrOperate};
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~XThreadPool();

	/**
	 * Two-phased constructor.
	 */
	static XThreadPool* NewL();

	/**
	 * Two-phased constructor.
	 */
	static XThreadPool* NewLC();
	
	static XThreadPool* GetInstance()
			{
			static XThreadPool* instance = NULL;
			if(instance==NULL)
				instance = XThreadPool::NewL();
			return instance;
			}
	
	int ErrHandle(int err,XU32 pData);
	void SetObserver(MObserver* observer){m_pObserver = observer;}
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	XThreadPool();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
public:
	//
	void OnTimer(const unsigned char* msg,int err);
public:
	//�����̶߳��󣬲���Ϊ�����߳���
	void CreateThread(int num);
	//�õ������߳�
	XThread* GetThread(XBase* base,bool necessary = false);
	//�ͷ��߳̾��
	void FreeThread(XThread*& thread);
	//�õ���ǰ�����е��߳���
	int GetCount(){return m_nAlive;}
	//�õ��̳߳��п��е��߳���
	int GetFreeCount(){return m_nAlive-m_nBusy;}
	//����̳߳����߳�
	void Release();
	//��ʱ������ʱ��
	void StartTimer();
	//
	XThread* GetCurrent();
protected:
	threadArray  m_Array;					//�߳�����
	CAfterTimer	*m_hStarter;				//�߳�������
	int 		 m_nAlive;					//���ŵ��߳���
	int			 m_nBusy;					//æµ���߳���
	int			 m_nCreateNum;				//�ȴ��������߳���
	XThreadPool::MObserver *m_pObserver;	//�ص���Ϣ���
//	threadArray  m_ErrArray;
//	int 		 m_nMsg;					//��ʱ״̬
	};

#endif // XTHREADPOOL_H
