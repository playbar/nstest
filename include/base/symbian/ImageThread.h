/*
 ============================================================================
 Name		: ImageThread.h
 Author	  : zhangchen
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CImageThread declaration
 ============================================================================
 */

#ifndef IMAGETHREAD_H
#define IMAGETHREAD_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "XVector.h"
#include <fbs.h>
#include "XMutex.h"
#include "symbiandebug.h"

#define KImageMsgMax 63

/////////////////////////////////////////////
//创建图片没有释放
#define KMemoryNotFree		-1000

////////////////////////////////////////////
//内存不足错误
#define KMemoryErr			-1
////////////////////////////////////////////
//new图片不成功
#define KNewErr				-2
////////////////////////////////////////////
//create图片不成功
#define KCreateErr			-3
////////////////////////////////////////////
//测试临时码
#define KDebugErr			-6

/////////////////////////////////////////////
//

class XImageDec;
// CLASS DECLARATION

//消息
class XMessageEvent
	{
public:
	enum
	{
		ENewImage,
		ECreateImage,
		EReleaseImage,
		EReleaseXImage,
		EDecodeImage
	};
	XMessageEvent(CFbsBitmap*& image,TSize size = TSize(),TDisplayMode mode = EGray2); 
	~XMessageEvent();
	void ResetMsg(int EMsg)
		{
		m_nMessage = EMsg;
		m_bInQueue = false;
		m_nIndex = 0;
		}
public:
	int		m_nMessage;			//消息种类
	XEvent*	m_hEvent;			//消息事件
	CFbsBitmap*& m_hImage;		//要处理的指针
	TSize		m_Size;			//要分配尺寸
	TDisplayMode m_Mode;		//要分配模式
	bool		 m_bInQueue;	//是否在队列里
	int			 m_nIndex;		//消息队列中的索引记录
	};

class XMessageDecode : public XMessageEvent
	{
public:
	XMessageDecode(CFbsBitmap*& image,CFbsBitmap*& mask);
	~XMessageDecode();
//	void CallBack();
public:
	CFbsBitmap*& m_hMask;
	void		*m_hBuf;
	void		*m_hFname;
	XImageDec	*m_hDec;
	int			 m_nFrameIndex;				//要解码的帧数
	int			 m_nFrames;					//图片总帧数
	XU8*		 m_pDelay;					//帧间间隔时间
	};

/**
 *  CImageThread
 * 
 */
class XImageThread : public CActive
	{
public:
	class MObserver
		{
		public:
		virtual bool ErrObserver(int err)=0; 
		};
public:
	// Constructors and destructor
	/**
	 * Constructor for performing 1st stage construction
	 */
	XImageThread();
	/**
	 * Destructor.
	 */
	virtual ~XImageThread();
	
	static XImageThread* NewL();
	static XImageThread* NewLC();

	//得到单例
	static XImageThread* GetInstance()
			{
#ifdef _LostImage
			return NULL;
#endif
			static XImageThread* instance = NULL;
			if(instance == NULL)
				instance = XImageThread::NewL();
			return instance;
			}
public:
	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	//线程函数
	static int TFonction(TAny* aThread);
public:
	void RunL();                            //CActive virtual function     
	TInt RunError(TInt aError);             //CActive virtual function
	void DoCancel();                        //CActive virtual function
	//循环回调
	int CallBack();
	//停止线程
	int Stop();
	//设置回调接受者
	void SetObserver(MObserver* observer){m_pObserver = observer;}
public:
	//创建图像，返回创建图像的ID
	bool NewImage(XMessageEvent& msg);
	//根据Id清除图像
	bool FreeImage(XMessageEvent& msg);
	//删除XImage
	bool FreeXImage(XMessageDecode& msg);
	//创建新图
	bool CreateBitmap(XMessageEvent& msg);
	//解码图片
	int DecoderImage(XMessageDecode& msg);
	//检查图像线程是否已经启动
	bool IsStart(){return m_bIsStart;}
protected:
	void _Sleep();
protected:
	//向队列添加消息
	bool _AddMsg(XMessageEvent& msg);
	bool _AddHighMsg(XMessageEvent &msg);
	XMessageEvent* _GetMsg();
	bool _LostMsg(XMessageEvent& msg);
public:
	//启动时间异步
	void Starter();
	//异常处理
	void ErrExecuted(int aErr);
protected:
	int _NewImage(XMessageEvent &msg);
	void _FreeImage(XMessageEvent &msg);
	int _CreateImage(XMessageEvent &msg);
	int _DecoderImage(XMessageDecode &msg);
	void _FreeXImage(XMessageDecode &msg);
protected:
	RThread 	m_iThread;						//线程
	RTimer		m_iTimer;						//异步启动器
//	XVector<CFbsBitmap*>		m_rImageVector;			//数据队列
	int			m_nImageNum;					//创建图像数目
	int			m_nCreateNum;					//构造图像数目
	XEvent		m_iStop;						//线程停止消息
	XEvent		m_iStoped;						//线程停止标志
//	XEvent		m_iStart;						//线程启动事件
	XMessageEvent	*m_rFlag[KImageMsgMax];						//同步消息
	XMessageEvent **m_hSetMsg;
	XMessageEvent **m_hGetMsg;		//队列读写指针
	XEvent		m_iEvent;						//事件,队列中有消息就会触发这个事件
	bool		m_bIsStart;
//	TSize		m_iImageSize;					//创建图像尺寸
//	TDisplayMode	m_nImageMode;				//图像模式
//	XVector<XMessageEvent*>		m_rEvent;		//事件
	MObserver	*m_pObserver;					//异常事件回调
	};

#endif // IMAGETHREAD_H
