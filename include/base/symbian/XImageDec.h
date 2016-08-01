/*
 * XImageDec.h
 *
 *  Created on: 2010-12-17
 *      Author: Administrator
 */

#ifndef XIMAGEDEC_H_
#define XIMAGEDEC_H_

#include "stdafxggbase.h"
#include "XImage.h"
#include "XResource.h"
#include "XStream.h"
#include "XDib.h"
#ifdef _SYMBIAN
# include <ImageConversion.h>
# include <unistd.h> 
# include <gdi.h>
# include "XFile.h"
# include "ZCConvert.h"
# include "ImageThread.h"

#define KIsGIF		1
#define KIsNotGIF	2
#endif

class XImageDec : public CActive
{
public:
	static XImageDec* NewL()
	{
		XImageDec*pDec=new(ELeave)XImageDec;
		return pDec;
	}
	void DoCancel();
	void RunL();
	TInt RunError(TInt aError);
	XImageDec();
	void Release();
	virtual ~XImageDec();
	//void InitDelay(XU8*pDelay,int nCount);
	int Decode(TDesC8&buf,CFbsBitmap*& aImage,CFbsBitmap*& aMask);
	//得到指定帧的图像
	int GetFrame(CFbsBitmap*& aImage,CFbsBitmap*& aMask,int frameId);
	//在图像线程中创建图像解码解码图像
	int ThreadDecoderD(TDesC8& buf,CFbsBitmap*& aImage,CFbsBitmap*& aMask,int index=0);
	int ThreadDecoderF(const TDesC& fname,CFbsBitmap*& aImage,CFbsBitmap*& aMask,int index=0);
//	int ThreadGetFrame(CFbsBitmap *& aImage,CFbsBitmap*& aMask,int frameId);
//	//在图像线程中获得指定帧的解码
//	int ThreadGetFrame(CFbsBitmap*& aImage,CFbsBitmap*& aMask,int frameId);
	//得到指定帧的相对坐标
	TRect GetCoordsRect(int frameId);
	//得到帧数
	int GetFramesCount();
	int InitDelay(XU8*& aDelay);
	int GetStyle(){return m_nStyle;}
protected:
	CImageDecoder*        m_pDec;
	RFs					  m_iFs;
	CActiveSchedulerWait* iWait;
	bool				  m_bRfsed;
	bool				  m_bAdded;
	int 				  m_nStyle;
};

#endif /* XIMAGEDEC_H_ */
