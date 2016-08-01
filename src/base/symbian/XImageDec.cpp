// XImage.cpp: implementation of the XImage class.
//
//////////////////////////////////////////////////////////////////////
#include "XImageDec.h"

//内存分配错误
#define KMemError -1
#define KDecodError -2
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XImageDec::XImageDec() : CActive(EPriorityHigh)
{
  iWait = XNULL;  
  m_pDec = XNULL;
  m_bAdded = XFALSE; 
  m_nStyle = 0;
  m_bRfsed = XFALSE;
//  m_pMsg = XNULL;
  m_iFs.Connect();
}

XImageDec::~XImageDec()
{
	Release();
	m_iFs.Close();
}
void XImageDec::Release()
{
   if(IsActive())
	   Cancel();                         //丢弃正在执行的请求
   if(iWait)
   {
	   if(iWait->IsStarted())
		   iWait->AsyncStop();
	   delete iWait;
	   iWait=XNULL;
   }
   if(m_pDec)
	   delete m_pDec;
   m_pDec = XNULL;
   m_nStyle = 0;
}

/////////////////////////////////////////////////////////////////////
//初始化帧与帧间的时间间隔
int XImageDec::InitDelay(XU8*& aDelay)
{	
	if(!m_pDec)
		{
		aDelay=NULL;
		return 1;
		}
	int nFrames = m_pDec->FrameCount();
	if(aDelay)
		delete aDelay;
	aDelay = XNULL;
	if(nFrames<=1) return nFrames;
	aDelay = new XU8[nFrames];
	for(int i=0;i<nFrames;i++)
		{
		aDelay[i]=m_pDec->FrameInfo(i).iDelay.Int64()/5;
		}
	return nFrames;
}

//通过文件名在线程中解码
int XImageDec::ThreadDecoderF(const TDesC& fname,CFbsBitmap*& aImage,CFbsBitmap*& aMask,int index)
	{
	Release();
	if(!m_bRfsed)
		{
		m_iFs.Connect();
		m_bRfsed=true;
		}
	int err;
	TRAP(err,m_pDec = CImageDecoder::FileNewL(m_iFs,fname,CImageDecoder::EOptionAlwaysThread));
	if(err!=KErrNone)
		{
		Release();
		return KDecodError;
		}
	if(m_pDec->FrameCount()>1)
		m_nStyle = KIsGIF;
	if(aImage)
		delete aImage;
	if(aMask)
		delete aMask;
	aImage = new(ELeave) CFbsBitmap();
	aMask = new(ELeave) CFbsBitmap();
	if(!aImage || !aMask)
		{
		if(aImage) delete aImage;
		aImage=NULL;
		if(aMask) delete aMask;
		aMask=NULL;
		return KMemError;
		}
	if(aImage->Create(m_pDec->FrameInfo(index).iOverallSizeInPixels,m_pDec->FrameInfo(index).iFrameDisplayMode))
		{
		delete aImage; aImage=NULL;
		delete aMask; aMask=NULL;
		return KMemError;
		}
	if(aMask->Create(m_pDec->FrameInfo(index).iOverallSizeInPixels,EGray256))
		{
		delete aImage; aImage=NULL;
		delete aMask; aMask=NULL;
		return KMemError;
		}
	m_pDec->Convert(&iStatus,*aImage,*aMask);
	User::WaitForRequest(iStatus);
	return 0;
	}

int XImageDec::Decode(TDesC8&buf,CFbsBitmap*& aImage,CFbsBitmap*& aMask)
	{
	Release();
	if(!m_bAdded)
		{
		  CActiveScheduler::Add(this);
		  m_bAdded = XTRUE;
		}
		
	if(iWait)
		{
		  if(iWait->IsStarted())
			iWait->AsyncStop();
		}
	else
		iWait = new (ELeave)CActiveSchedulerWait; 
	if(buf.Ptr()[0] == 'g' || buf.Ptr()[0] == 'G')
		m_nStyle=KIsGIF;
	if(!m_bRfsed)
		{
		m_iFs.Connect();
		m_bRfsed=true;
		}
	m_pDec = CImageDecoder::DataNewL(m_iFs,buf);//,(CImageDecoder::TOptions)(CImageDecoder::EOptionAlwaysThread|CImageDecoder::EAllowGeneratedMask));

	
	XImageThread *ImgThrd = XImageThread::GetInstance();
	//创建对应图像
	if(aImage != NULL)
		{
		XMessageEvent msg(aImage);
		ImgThrd->FreeImage(msg);
		}
	if(aMask != NULL)
		{
		XMessageEvent msg(aMask);
		ImgThrd->FreeImage(msg);
		}
	XMessageEvent nmsg1(aImage);
	ImgThrd->NewImage(nmsg1);
	XMessageEvent nmsg2(aMask);
	ImgThrd->NewImage(nmsg2);
	XMessageEvent cmsg1(aImage,m_pDec->FrameInfo().iOverallSizeInPixels,m_pDec->FrameInfo().iFrameDisplayMode);
	ImgThrd->CreateBitmap(cmsg1);
	XMessageEvent cmsg2(aMask,m_pDec->FrameInfo().iOverallSizeInPixels,EGray2);
	ImgThrd->CreateBitmap(cmsg2);
	
	m_pDec->Convert(&iStatus,*aImage,*aMask);

	CActive::SetActive();
	iWait->Start();
//	User::WaitForRequest(iStatus);

	return 0;
	}

//得到指定帧的图像
int XImageDec::GetFrame(CFbsBitmap*& aImage,CFbsBitmap*& aMask,int frameId)
	{
	Cancel();
	XImageThread *ImgThrd = XImageThread::GetInstance();
	if(aImage != NULL)
		{
		XMessageEvent msg(aImage);
		ImgThrd->FreeImage(msg);
		}
	if(aMask != NULL)
		{
		XMessageEvent msg(aMask);
		ImgThrd->FreeImage(msg);
		}
	XMessageEvent nmsg1(aImage);
	ImgThrd->NewImage(nmsg1);
	XMessageEvent nmsg2(aMask);
	ImgThrd->NewImage(nmsg2);
	XMessageEvent cmsg1(aImage,m_pDec->FrameInfo(frameId).iOverallSizeInPixels,m_pDec->FrameInfo(frameId).iFrameDisplayMode);
	ImgThrd->CreateBitmap(cmsg1);
	XMessageEvent cmsg2(aMask,m_pDec->FrameInfo(frameId).iOverallSizeInPixels,EGray2);
	ImgThrd->CreateBitmap(cmsg2);
	m_pDec->Convert(&iStatus,*aImage,*aMask,frameId);
	SetActive();
	iWait->Start();
	return frameId;	
	}

//int XImageDec::ThreadGetFrame(CFbsBitmap *& aImage,CFbsBitmap*& aMask,int frameId)
//	{
//	CActiveScheduler::Add(this);
//	if(aImage)
//		delete aImage;
//	aImage = new(ELeave) CFbsBitmap();
//	if(aMask)
//		delete aMask;
//	aMask = new(ELeave) CFbsBitmap();
//	if(!aImage || !aMask)
//		return KMemError;
//	if(aImage->Create(m_pDec->FrameInfo(frameId).iOverallSizeInPixels,m_pDec->FrameInfo(frameId).iFrameDisplayMode))
//			{
//			delete aImage; aImage=NULL;
//			delete aMask; aMask=NULL;
//			return KMemError;
//			}
//	if(aMask->Create(m_pDec->FrameInfo(frameId).iOverallSizeInPixels,EGray256))
//			{
//			delete aImage; aImage=NULL;
//			delete aMask; aMask=NULL;
//			return KMemError;
//			}
//	m_pDec->Convert(&iStatus,*aImage,*aMask,frameId);
//	SetActive();
//
//	iWait->Start();
//	if(iStatus.Int() != KErrNone)
//		{
//		Release();
//		delete aImage;
//		aImage = XNULL;
//		delete aMask;
//		aMask = XNULL;
//		return iStatus.Int();
//		}
//	return 0;
//	}

//在图像线程中创建图像解码解码图像
int XImageDec::ThreadDecoderD(TDesC8& buf,CFbsBitmap*& aImage,CFbsBitmap*& aMask,int index)
	{
	Release();
#ifdef _SYMBIANDEBUG
//	aImage = new(ELeave) CFbsBitmap();
//	aMask=new(ELeave) CFbsBitmap();
//	aImage->Create(TSize(20,20),EColor16MA);
//	aMask->Create(TSize(20,20),EColor256);
//	return 0;
#endif
	if(!m_bAdded)
		{
		  CActiveScheduler::Add(this);
		  m_bAdded = XTRUE;
		}
	if(aImage)
		delete aImage;
	aImage=NULL;
	if(aMask)
		delete aMask;
	aMask=NULL;
	int err(0);
	TRAP(err,m_pDec = CImageDecoder::DataNewL(m_iFs,buf););
	if(err!=KErrNone)
		{
		Release();
		return KDecodError;
		}
	iWait = new (ELeave)CActiveSchedulerWait; 
	aImage = new(ELeave) CFbsBitmap();
	aMask = new(ELeave) CFbsBitmap();
	if(!aImage || !aMask)
		{
		Release();
		if(aImage) delete aImage;
		aImage=NULL;
		if(aMask) delete aMask;
		aMask=NULL;
		return KMemError;
		}
	if(buf.Ptr()[0] == 'g' || buf.Ptr()[0] == 'G')
		m_nStyle=KIsGIF;
	if(aImage->Create(m_pDec->FrameInfo(index).iOverallSizeInPixels,m_pDec->FrameInfo(index).iFrameDisplayMode))
//	if(aImage->Create(TSize(1,1),EGray2))
		{
		Release();
		delete aImage; aImage=NULL;
		delete aMask; aMask=NULL;
		return KMemError;
		}
	if(aMask->Create(m_pDec->FrameInfo(index).iOverallSizeInPixels,EGray256))
//	if(aImage->Create(TSize(1,1),EGray2))
		{
		Release();
		delete aImage; aImage=NULL;
		delete aMask; aMask=NULL;
		return KMemError;
		}
	m_pDec->Convert(&iStatus,*aImage,*aMask);
	SetActive();

	iWait->Start();
	if(iStatus.Int() != KErrNone)
		{
		Release();
		delete aImage;
		aImage = XNULL;
		delete aMask;
		aMask = XNULL;
		return iStatus.Int();
		}
	Release();
	return 0;
	}
////在图像线程中获得指定帧的解码
//int XImageDec::ThreadGetFrame(CFbsBitmap*& aImage,CFbsBitmap*& aMask,int frameId)
//	{
//	Cancel();
//	if(aImage)
//		delete aImage;
//	if(aMask)
//		delete aMask;
//	aImage = new(ELeave) CFbsBitmap();
//	aMask = new(ELeave) CFbsBitmap();
//	aImage->Create(m_pDec->FrameInfo(frameId).iOverallSizeInPixels,m_pDec->FrameInfo(frameId).iFrameDisplayMode);
//	aMask->Create(m_pDec->FrameInfo(frameId).iOverallSizeInPixels,EGray2);
//	m_pDec->Convert(&iStatus,*aImage,*aMask,frameId);
//	SetActive();
//	iWait->Start();
//	return frameId;	
//	}

//得到指定帧的相对坐标
TRect XImageDec::GetCoordsRect(int frameId)
	{
	if(!m_pDec) return TRect();
	return m_pDec->FrameInfo(frameId).iFrameCoordsInPixels;
	}

//得到帧数
int XImageDec::GetFramesCount()
	{
	if(!m_pDec) return 0;
	return m_pDec->FrameCount();
	}

void XImageDec::RunL()
{
	iWait->AsyncStop();	
}

TInt XImageDec::RunError(TInt aError)
{
	iWait->AsyncStop();
	KErrNone;
	return aError;
}

void XImageDec::DoCancel()
	{
		if(IsActive())
			m_pDec->Cancel();
		iWait->AsyncStop();
	}
