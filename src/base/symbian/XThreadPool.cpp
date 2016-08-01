/*
 ============================================================================
 Name		: XThreadPool.cpp
 Author	  : zhangchen
 Version	 : 1.0
 Copyright   : �̳߳�
 Description : CXThreadPool implementation
 ============================================================================
 */

#include "XThreadPool.h"
#include "symbiandebug.h"
#include <pthread.h>

XThreadPool::XThreadPool()
	{
	m_hStarter = NULL;				//�߳�������
	m_nAlive = 0;					//���ŵ��߳���
	m_nBusy = 0;					//æµ���߳���
	m_nCreateNum = 0;				//�ȴ��������߳���
	m_pObserver = NULL;
	// No implementation required
	}

XThreadPool::~XThreadPool()
	{
	Release();
	}

XThreadPool* XThreadPool::NewLC()
	{
	XThreadPool* self = new (ELeave) XThreadPool();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

XThreadPool* XThreadPool::NewL()
	{
	XThreadPool* self = XThreadPool::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void XThreadPool::ConstructL()
	{
	m_hStarter = CAfterTimer::NewL(*this);
	}

//
void XThreadPool::OnTimer(const unsigned char* msg,int err)
	{
//	switch(m_nMsg)
//		{
//		case EStartPool:
//			{
			int size = m_Array.GetSize();
			XThread* thread;
			if(size>0)
				{
				thread = m_Array.GetAt(size-1);
				if(!thread->IsStart())
					{
					m_hStarter->StartL(100000);
					return;
					}
				else
					{
					m_nAlive++;
//					m_nCreateNum--;
					}
				}
			if(m_nCreateNum > size)
				{
				thread = new XThread;
				m_Array.Add(thread);
				thread->Create(NULL,100);
				thread->SetErrHandle(this);
				m_hStarter->StartL(100000);
				}
//			}
//			break;
//		case EErrOperate:
//			{
//			int size = m_ErrArray.GetSize();
//			if(size>0)
//				{
//				XThread *thread = m_ErrArray.GetAt(0);
//				thread->Create(NULL,100);
//				thread->SetErrHandle(this);
//				m_ErrArray.RemoveAt(0,1);
//				m_hStarter->StartL(100000);
//				}
//			}
//			break;
//		}
#ifdef _SYMBIANDEBUG
	else
		{
		SYMBIANDEB(DefaultLog,"ThreadPool is created\r\n");
		}
#endif
	}

//�����̶߳��󣬲���Ϊ�����߳���
void XThreadPool::CreateThread(int num)
	{
	m_nCreateNum = num;
//	m_nMsg = EStartPool;
	m_hStarter->StartL(100);
	}

//�õ������߳�
XThread* XThreadPool::GetThread(XBase* base,bool necessary)
	{
	XThread *thread = NULL;
	if(m_nBusy>=m_nAlive) return NULL;
	for(int i=0;i<m_nAlive;i++)
		{
		thread = m_Array.GetAt(i);
		if(!thread->IsStoped() && !thread->IsBusy())
			{
			thread->SetBase(base);
			m_nBusy++;
			return thread;
			}
		}
	if(necessary)
		{
		//
		}
	return thread;
	}

//�ͷ��߳̾��
void XThreadPool::FreeThread(XThread*& thread)
	{
	if(thread==NULL) return;
	thread->SetBase(NULL);
	thread=NULL;
	m_nBusy--;
	}

//����̳߳����߳�
void XThreadPool::Release()
	{
	m_hStarter->Cancel();
	int size = m_Array.GetSize();
	for(int i=0;i<size;i++)
		delete m_Array.GetAt(i);
	m_Array.RemoveAll();
	m_nAlive=0;
	m_nBusy=0;
	m_nCreateNum=0;
	}

int XThreadPool::ErrHandle(int err,XU32 pData)
	{
//	XThread* temp = (XThread*)pData;
//	m_ErrArray.Add(temp);
//	m_nMsg = EErrOperate;
//	if(m_pObserver)
		m_pObserver->Message(err);
	}

//��ʱ������ʱ��
void XThreadPool::StartTimer()
	{
	m_hStarter->StartL(0);
	}

XThread* XThreadPool::GetCurrent()
	{
		pthread_t current;
		current = pthread_self();
		XThread *temp = NULL;
		int size;
		size = m_Array.GetSize();
		for(int i=0 ; i<size ; i++)
			{
			temp = m_Array.GetAt(i);
			if(current == temp->GetHandle())
				return temp;
			}
		return NULL;
	}
