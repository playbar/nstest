/*
 ============================================================================
 Name		: ImageThread.cpp
 Author	  : zhangchen
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CImageThread implementation
 ============================================================================
 */

#include "ImageThread.h"
#include <Ecom\ECom.h>
#include "XImageDec.h"

#define KEventWaitTime  10000
#define KCallBackWaitTime 100000000

XMessageEvent::XMessageEvent(CFbsBitmap*& image,TSize size,TDisplayMode mode):m_hImage(image)
	{
//	m_hImage = image;
	m_nMessage=ENewImage;
	m_Size = size;
	m_Mode = mode;
	}
XMessageEvent::~XMessageEvent()
	{
	
	}

XMessageDecode::XMessageDecode(CFbsBitmap*& image,CFbsBitmap*& mask):XMessageEvent(image),m_hMask(mask)
	{
//	m_hDec = dec;
	m_hBuf=XNULL;
	m_nFrameIndex = 0;
	m_nFrames=1;
	m_pDelay = XNULL;
	}
XMessageDecode::~XMessageDecode()
	{
	
	}
//void XMessageDecode::CallBack()
//	{
//	m_nFrames = m_hDec->GetFramesCount();					//�����ͼƬ֡��
//	m_hDec->InitDelay(m_pDelay);		//ÿ֡ʱ����
//	m_hEvent->SetEvent();
//	delete m_hDec;
//	m_hDec = NULL;
//	}

XImageThread::XImageThread(): CActive(CActive::EPriorityHigh)
	{
	// No implementation required
	m_hSetMsg = m_rFlag;
	m_hGetMsg = m_rFlag;
	m_nImageNum=0;
	m_nCreateNum = 0;
	m_iThread.SetHandle(0);
	m_bIsStart=XNULL;
	m_pObserver = XNULL;
	}

XImageThread::~XImageThread()
	{
	Stop();
	if(m_iThread.Handle()!=NULL)
		{
		m_iThread.Kill(0);
		m_iThread.Close();
		}
//	m_rImageVector.FreeAll();
	m_iStoped.Release();
	m_iStop.Release();
	m_iEvent.Release();
	m_pObserver = XNULL;
//	m_iStart.Release();
	}

XImageThread* XImageThread::NewLC()
	{
	XImageThread* self = new (ELeave) XImageThread();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

XImageThread* XImageThread::NewL()
	{
	XImageThread* self = XImageThread::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void XImageThread::ConstructL()
	{
	m_iStop.Create();
	m_iStoped.Create();
	m_iEvent.Create();
	m_iStop.ResetEvent();
	m_iStoped.ResetEvent();
	m_iEvent.ResetEvent();

	 HBufC* threadName = HBufC::NewLC(KMaxFileName);
	 TPtr ptr = threadName->Des();
	 ptr.Copy(_L("XImageThread"));
	 ptr.AppendNum((TInt32)this);
	 int err = m_iThread.Create(
						 *threadName,														//�߳����֡�
						 XImageThread::TFonction,										//�߳��壬����ָ��
						 KDefaultStackSize,
						 &User::Allocator(),
						 this);
//	 User::LeaveIfError(m_iThread.Create(
//										 *threadName,														//�߳����֡�
//										 XImageThread::TFonction,										//�߳��壬����ָ��
//										 0x1000,
//										 &User::Allocator(),
//										 this));	//�����߳��������
//	 m_bThreadType |= KISACTIVE;
#ifdef _SYMBIANDEBUG
	 SYMBIANDEB1(DefaultLog,"CreateError::%d\r\n",err);
#endif

	 m_iThread.Resume();

//	 m_iStart.ResetEvent();
//	 while(!m_iStart.IsEvent(KEventWaitTime));
//	 User::After(10000);
	 CleanupStack::PopAndDestroy(threadName); 
	}

//�̺߳���
int XImageThread::TFonction(TAny* aThread)
	{
#ifdef _SYMBIANDEBUG
	SYMBIANDEB(DefaultLog,"int XImageThread::TFonction(TAny* aThread)::Start\r\n");
#endif
	   // 1. Add cleanup stack support.
	    CTrapCleanup* cleanupStack = CTrapCleanup::New();
	 
	    // 2. Get pointer to thread host
	    XImageThread* host = (XImageThread*)aThread;
	    
	    TRAPD(err,
	        // 3. Add support for active objects
	        CActiveScheduler* activeScheduler = new (ELeave) CActiveScheduler;
	        CleanupStack::PushL(activeScheduler);
	        CActiveScheduler::Install(activeScheduler);
	        
	        //create a session with the font and bitmap server
	        RFbsSession iFbsS;
//	        User::LeaveIfError(iFbsS.Connect());
	        iFbsS.Connect();
	 
	        // the thread function well be start.
	        CActiveScheduler::Add(host);
	        host->Starter();
	        // 5. --> Thread execution starts
	        CActiveScheduler::Start();
	        // 6. --> Thread execution ends (waiting for CActiveScheduler::Stop())
	 
	        iFbsS.Disconnect();
	        CleanupStack::PopAndDestroy(activeScheduler);
	        );
	 
	    host->ErrExecuted(err);
	    REComSession::FinalClose();          //���ӿڲ��ʹ�õ��ڴ�
	    delete cleanupStack;
	    return KErrNone;
	}

void XImageThread::RunL()                            //CActive virtual function     
	{
#ifdef _SYMBIANDEBUG
	SYMBIANDEB(DefaultLog,"void XImageThread::RunL()::Start\r\n");
#endif
//	m_iStart.SetEvent();
	CallBack();
	m_iTimer.Close();
	CActiveScheduler::Stop();
	return;
	}
TInt XImageThread::RunError(TInt aError)             //CActive virtual function
	{
	m_iTimer.Close();
	Starter();
	m_pObserver->ErrObserver(aError);
	return aError;
	}
void XImageThread::DoCancel()                        //CActive virtual function
	{
	m_iTimer.Cancel();
	}
//ѭ���ص�
int XImageThread::CallBack()
	{
	XMessageEvent* message;
	int err(0);
	m_bIsStart=true;
	while(1)
		{
		if(m_iStop.IsEvent(0))
			{
			m_iStoped.SetEvent();
			if(m_nImageNum)
				err = KMemoryNotFree;
			return err;
			}
		if(m_iEvent.IsEvent(KCallBackWaitTime))
			{
			err = 0;
			message = _GetMsg();
			if(message!=XNULL)
				{
					switch(message->m_nMessage)
					{
						case XMessageEvent::ENewImage:
							{
							if(_NewImage(*message)!=0)
								{
								err = KNewErr;
								if(m_pObserver) m_pObserver->ErrObserver(err);
								}
							}
							break;
						case XMessageEvent::ECreateImage:
							{
							//����ͼ��
							if(_CreateImage(*message) != KErrNone)
								{
								err = KCreateErr;
								if(m_pObserver) m_pObserver->ErrObserver(err);
								}
							}
							break;
						case XMessageEvent::EReleaseImage:
							{
							//ɾ��ָ����ͼ��
							_FreeImage(*message);
							}
							break;
						case XMessageEvent::EDecodeImage:
							{
							//�������
							if(KErrNone != _DecoderImage(*(XMessageDecode*)message))
								{
								err = KMemoryErr;
								if(m_pObserver) m_pObserver->ErrObserver(err);
								}
							}
							break;
						case XMessageEvent::EReleaseXImage:
							{
							//ɾ��XImage
							_FreeXImage(*(XMessageDecode*)message);
							}
							break;
					}
					
				}
			_Sleep();
			}
		User::After(200);
		}
	}

//���Id�õ�ͼ��
//CFbsBitmap* XImageThread::GetImage(int Id)
//	{
//	int size = m_rImageVector.GetSize();
//	if(Id>=size || Id<0) return XNULL;
//	CFbsBitmap* temp;
//	temp=m_rImageVector.GetData()[Id];
//	return temp;
//	}
//����ͼ��
bool XImageThread::NewImage(XMessageEvent& msg)
	{
	msg.m_nMessage = XMessageEvent::ENewImage;
	XEvent event;
	event.Create();
	msg.m_hEvent = &event;
	msg.m_hEvent->ResetEvent();
	if(!_AddMsg(msg)) return false;
	m_iEvent.SetEvent();
	while(!msg.m_hEvent->IsEvent(KEventWaitTime));
	return true;
	}
//���Id���ͼ��
bool XImageThread::FreeImage(XMessageEvent& msg)
	{
#ifdef _SYMBIANDEBUG
	SYMBIANDEB(DefaultLog,"freeImage::Start\r\n");
#endif
	msg.m_nMessage = XMessageEvent::EReleaseImage;
	XEvent event;
	event.Create();
	msg.m_hEvent = &event;
	msg.m_hEvent->ResetEvent();
	if(!_AddMsg(msg)) return false;
	m_iEvent.SetEvent();
	while(!msg.m_hEvent->IsEvent(KEventWaitTime));
	return true;
	}

bool XImageThread::FreeXImage(XMessageDecode& msg)
	{
	if(msg.m_bInQueue)
		{
		_LostMsg(msg);
		}
	msg.ResetMsg(XMessageEvent::EReleaseXImage);
	XEvent event;
	event.Create();
	msg.m_hEvent = &event;
	msg.m_hEvent->ResetEvent();
	if(!_AddHighMsg(msg)) return false;
	m_iEvent.SetEvent();
	while(!msg.m_hEvent->IsEvent(KEventWaitTime));
	return true;
	}

//������ͼ
bool XImageThread::CreateBitmap(XMessageEvent& msg)
	{
#ifdef _SYMBIANDEBUG
	SYMBIANDEB1(DefaultLog,"CreateBitmap::%d\r\n",m_nImageNum);
#endif
	msg.m_nMessage = XMessageEvent::ECreateImage;
	XEvent event;
	event.Create();
	msg.m_hEvent = &event;
	msg.m_hEvent->ResetEvent();
	if(!_AddMsg(msg)) return false;
	m_iEvent.SetEvent();
	while(!msg.m_hEvent->IsEvent(KEventWaitTime));
	return true;
//	while(1)
//		{
//		if(msg.m_hEvent->IsEvent(KEventWaitTime))
//			return true;
//		}
	}

//����ͼƬ
int XImageThread::DecoderImage(XMessageDecode& msg)
	{
#ifdef _SYMBIANDEBUG
	SYMBIANDEB(DefaultLog,"int XImageThread::DecoderImage::Start\r\n");
	SYMBIANDEB1(DefaultLog,"NewImage::%d\r\n",m_nImageNum);
#endif

	msg.m_nMessage = XMessageEvent::EDecodeImage;
	XEvent event;
	event.Create();
	msg.m_hEvent = &event;
	msg.m_hEvent->ResetEvent();
	if(!_AddMsg(msg)) return false;
	m_iEvent.SetEvent();
	while(!msg.m_hEvent->IsEvent(KEventWaitTime));
	msg.m_hFname=NULL;
	msg.m_hBuf=NULL;
	return true;
	}

//����ʱ���첽
void XImageThread::Starter()
	{
#ifdef _SYMBIANDEBUG
	SYMBIANDEB(DefaultLog,"void XImageThread::Starter()::Start\r\n");
#endif
	m_iTimer.CreateLocal();
	m_iTimer.AfterTicks(iStatus,0);
	SetActive();
	}
//�쳣����
void XImageThread::ErrExecuted(int aErr)
	{
	return;
	}

//ֹͣ�߳�
int XImageThread::Stop()
	{
	m_bIsStart=false;
	m_iStop.SetEvent();
	m_iEvent.SetEvent();
	Cancel();
	while(1)
		{
		if(m_iStoped.IsEvent(0))
			break;
		m_iStop.SetEvent();
		m_iEvent.SetEvent();
		User::After(300);
		}
	return true;
	}

int XImageThread::_NewImage(XMessageEvent &msg)
	{
	int err(0);
	msg.m_hImage=new(ELeave) CFbsBitmap();
	if(msg.m_hImage == XNULL)
		err = KNewErr;
	else
		m_nImageNum++;
	msg.m_hEvent->SetEvent();
	return err;
	}

void XImageThread::_FreeImage(XMessageEvent &msg)
	{
	delete msg.m_hImage;
	msg.m_hImage = XNULL;
	m_nImageNum--;
	msg.m_hEvent->SetEvent();
	}

int XImageThread::_CreateImage(XMessageEvent &msg)
	{
	int err = msg.m_hImage->Create(msg.m_Size,msg.m_Mode);
//	msg.m_hImage->HorizontalTwipsToPixels(msg.m_hImage->SizeInTwips().iWidth);
	msg.m_hEvent->SetEvent();
	return err;
	}

int XImageThread::_DecoderImage(XMessageDecode &msg)
	{
	int err(0);
		if(msg.m_hBuf)
			{
			XStream* pStream = (XStream*)msg.m_hBuf;
			msg.m_hDec = XImageDec::NewL();
	//		msg.m_hDec->SetMsgHandle(&msg);
			TPtrC8 ptr;
			ptr.Set(pStream->ReadAddr(),pStream->GetDataSize());
			err=msg.m_hDec->ThreadDecoderD(ptr,msg.m_hImage,msg.m_hMask,msg.m_nFrameIndex);
			}
		else
			{
			msg.m_hDec = XImageDec::NewL();
			TPtrC fname;
			fname.Set((unsigned short*)msg.m_hFname);
			err = msg.m_hDec->ThreadDecoderF(fname,msg.m_hImage,msg.m_hMask,msg.m_nFrameIndex);
			}
		
		if(KErrNone != err)
			{
			delete msg.m_hDec;
			msg.m_hDec = XNULL;
			msg.m_hEvent->SetEvent();
			return err;
			}
		
		int aframes = msg.m_hDec->GetFramesCount();
		msg.m_nFrames = aframes;					//�����ͼƬ֡��
		msg.m_hDec->InitDelay(msg.m_pDelay);		//ÿ֡ʱ����
		delete msg.m_hDec;
		msg.m_hDec = XNULL;
		m_nImageNum+=2;

	msg.m_hEvent->SetEvent();
	return err;
	}

void XImageThread::_FreeXImage(XMessageDecode &msg)
	{
	#define R(M) {if(M) delete (M); (M) = NULL;}
		R(msg.m_hDec);
		R(msg.m_hImage);
		R(msg.m_hMask);
	#undef R
		m_nImageNum-=2;
		msg.m_hEvent->SetEvent();
	}

//����������Ϣ
bool XImageThread::_AddMsg(XMessageEvent& msg)
	{
	int len = m_hSetMsg - m_hGetMsg;
	if(len == KImageMsgMax-1 || len == -1) return false;
	*m_hSetMsg = &msg;
	if(m_hSetMsg - &m_rFlag[KImageMsgMax-1] == 0)
		m_hSetMsg = m_rFlag;
	else
		m_hSetMsg++;
	msg.m_nIndex = m_hSetMsg-&m_rFlag[0];
	msg.m_bInQueue = true;
	return true;
	}

//�������Ӹ����ȼ���Ϣ
bool XImageThread::_AddHighMsg(XMessageEvent &msg)
	{
	int len = m_hSetMsg - m_hGetMsg;
	if(len == KImageMsgMax-1 || len == -1) return false;
	if(m_hGetMsg-&m_rFlag[0] == 0)
		{
		m_hGetMsg = &m_rFlag[KImageMsgMax-1];
		}
	else
		{
		m_hGetMsg--;
		}
	*m_hGetMsg = &msg;
	msg.m_nIndex = m_hGetMsg-&m_rFlag[0];
	msg.m_bInQueue = true;
	return true;
	}

XMessageEvent* XImageThread::_GetMsg()
	{
	if(m_hSetMsg-m_hGetMsg == 0) return XNULL;
	if(m_hGetMsg - &m_rFlag[KImageMsgMax-1] == 0)
		{
		m_hGetMsg = m_rFlag;
		m_rFlag[KImageMsgMax-1]->m_bInQueue=false;
		return m_rFlag[KImageMsgMax-1];
		}
	else
		{
		m_hGetMsg++;
		XMessageEvent* msg = *(m_hGetMsg-1);
		msg->m_bInQueue=false;
		return *(m_hGetMsg-1);
		}
	}

bool XImageThread::_LostMsg(XMessageEvent& msg)
	{
	int index = msg.m_nIndex;
	if(m_rFlag[index])
		{
		if(!(m_rFlag[index]->m_bInQueue)) return false;
		else 
			{
			m_rFlag[index]->m_hEvent->SetEvent();
			m_rFlag[index] = XNULL;
			}
		}
	return true;
	}

void XImageThread::_Sleep()
	{
	if(m_hSetMsg-m_hGetMsg == 0)
		m_iEvent.ResetEvent();
	}
