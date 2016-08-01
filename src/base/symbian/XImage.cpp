// XImage.cpp: implementation of the XImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxggbase.h"
#include "XImage.h"
#include "XResource.h"
#include "XStream.h"
#include "XDib.h"
#ifdef _SYMBIAN
# include <gdi.h>
# include <aknutils.h>
# include "XFile.h"
# include "XImageDec.h"
# include "ImageThread.h"
# include <math.h>
# include "GGBase.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define PI 3.1415926

//ͼ������Ƕ���ת
bool  XImage::Rotate(CFbsBitmap*& des,const CFbsBitmap* src,int angle)
	{
	double rad = angle * (PI/180);
	if(des) delete des;
	des = new CFbsBitmap();
	int h = src->SizeInPixels().iHeight;
	int w = src->SizeInPixels().iWidth;
	int H = h*sin(PI/2-rad);
//	int W = w*w + h*h
	}
	
 XImage::XImage() : m_Mes(m_pImage,m_pMask)
{
	m_pImage=XNULL;
	m_pStream=XNULL;
	m_nFrames=0;
	m_pDelay=XNULL;
#ifdef _SYMBIAN
	m_pMask=XNULL;
	m_bBitmap = false;
//	m_pDecoder = XNULL;
#endif
}

 XImage::~XImage()
{
	Release();
}

////////////////////////////////////////////////////////////////////////
//�õ�ͼ��Ŀ��
XU32 XImage::Width()const
{
#ifdef _WINDOWS32
	if(m_pImage!=XNULL)
		return ((Image*)m_pImage)->GetWidth();
	return 0;
#endif
#ifdef _SYMBIAN
	int w=0;
	if(m_pImage!=XNULL)
		w=((CFbsBitmap*)m_pImage)->SizeInPixels().iWidth;
	return w;
#endif	
}

/////////////////////////////////////////////////////////////////////////
//�õ�ͼ��ĸ�
XU32 XImage::Height()const
{
#ifdef _WINDOWS32
	if(m_pImage!=XNULL)
		return ((Image*)m_pImage)->GetHeight();
#endif
#ifdef _SYMBIAN
	if(m_pImage!=XNULL)
		return((CFbsBitmap*)m_pImage)->SizeInPixels().iHeight;
#endif
	return 0;
}


/////////////////////////////////////////////////////////////////////////
//true ��û��ͼ��ʱ��false �������
XBOOL XImage::IsEmpty()const
{	 
   if(m_pImage==XNULL) return XTRUE;
   /*if(((Image*)m_pImage)->GetLastStatus()!=Ok) return XTRUE;
   if(Width()<=0||
	  Height()<=0) return XTRUE;*/
   return XFALSE;
}

/////////////////////////////////////////////////////////////////////
//�ͷŵ�ǰʹ�õ�������Դ
void XImage::Release()
{
#ifdef _SYMBIANDEBUG
	SYMBIANDEB(DefaultLog,"image is release\r\n");
#endif
   if(m_pDelay!=XNULL) delete m_pDelay;
#ifdef WINDOWS32
   if(m_pImage!=XNULL) delete (Image*)m_pImage;
   
   if(m_pStream!=XNULL) 
	   ((IStream*)m_pStream)->Release();
#endif
	   //delete m_pStream;
#ifdef _SYMBIAN
   if(m_pStream!=XNULL)
	   delete (XStream*)m_pStream;

   if(m_pImage && m_pMask)
	   {
	   XImageThread *ImgThd = XImageThread::GetInstance();
	   if(ImgThd)
		   ImgThd->FreeXImage(m_Mes);
	   }
   m_nFrames = 0;
   m_pMask=XNULL;
#endif
   m_pDelay=XNULL;
   m_pStream=XNULL;
   m_pImage=XNULL;
   m_nFrames=0;
   m_bBitmap = XFALSE;

}

////////////////////////////////////////////////////////////////////
//������֡��
XINT XImage::FrameCount()
{
   return m_nFrames;
}


////////////////////////////////////////////////////////////////////
//����ͼ����   ����strFile�����ļ��� 16λ�ַ�
XBOOL XImage::Create(XPCWSTR str)
{
#ifdef _WINDOWS32
	Release();
	m_pImage=new Image(strFile);
	if(((Image*)m_pImage)->GetLastStatus()!=Ok)
	{
		Release();
		return XFALSE;
	}
	SetFrames();
	return m_pImage!=XNULL;
#endif
#ifdef _SYMBIAN
	Release();
#ifdef _LostImage
	return false;
#endif
	XImageThread *ImgThd = XImageThread::GetInstance();
	if(!ImgThd) return false;
	m_Mes.m_hFname = (unsigned short*)str;

	ImgThd->DecoderImage(m_Mes);
	
	if(!m_pImage || !m_pMask)
		{
		{FILE *fl;
		fl=fopen("c:/Data/GGClient.log","a");
		if(fl){ fprintf(fl,"image decode error------Create()\r\n"); fclose(fl);}}
		return XFALSE;
		}
	
	if(m_pDelay)
		delete m_pDelay;
	m_pDelay = m_Mes.m_pDelay;
	SetFrames();
	return m_pImage!=XNULL;
#endif
}

////////////////////////////////////////////////////////////////////////////////////
//���õ�ǰ�֡
XBOOL XImage::SetActive(XINT index)
{
#ifdef _LostImage
	return false;
#endif
	if(index<0||index>=m_nFrames) return XFALSE;
#ifdef _SYMBIAN
	if(m_pStream == XNULL) return XFALSE;
	XImageThread *ImgThd = XImageThread::GetInstance();
	if(!ImgThd) return XFALSE;
	m_Mes.m_nFrameIndex = index;
	m_Mes.m_hBuf = m_pStream;
	ImgThd->DecoderImage(m_Mes);
#elif defined(_WIN32)
	if(m_pImage==XNULL) return XFALSE;
	((Image*)m_pImage)->SelectActiveFrame((GUID*)&m_guid,index);
#endif
	return XTRUE;
}



/////////////////////////////////////////////////////////////////////////////////
//���õ�����Ϣ��תΪͼ��
XBOOL XImage::Final(XU8 bBitmap)
{	
#ifdef _SYMBIAN
#ifdef _LostImage
	return XFALSE;
#endif
	if(m_pStream==XNULL) return XFALSE;
	
	XImageThread *ImgThd = XImageThread::GetInstance();
	if(!ImgThd) return XFALSE;
	m_Mes.m_hBuf = m_pStream;
	_ANALYENTER("XImage::Final");
	m_Mes.m_nFrameIndex=0;
	ImgThd->DecoderImage(m_Mes);
	if(!m_pImage || !m_pMask)
		{
		{FILE *fl;
		fl=fopen("c:/Data/GGClient.log","a");
		if(fl){ fprintf(fl,"image decode error-------Final()\r\n"); fclose(fl);}}
		return XFALSE;
		}
	m_bBitmap = bBitmap;
	if(m_pDelay)
		delete m_pDelay;
	m_pDelay = m_Mes.m_pDelay;
	SetFrames();
	if(m_nFrames<=1)
		{
		delete m_pStream;
		m_pStream = XNULL;
		}
	return XTRUE;

#elif defined(_WIN32)
	if(m_pStream==XNULL) return XFALSE;
	if(m_pImage!=XNULL) delete (XImage*)m_pImage;
	m_pImage=new Image((IStream*)m_pStream);
	if(((Image*)m_pImage)->GetLastStatus()!=Ok)
	{
		Release();
		return XFALSE;
	}
	((IStream*)m_pStream)->Release();
	m_pStream=XNULL; 
	SetFrames();
	return XTRUE;
#endif


}

////////////////////////////////////////////////////////////////////////////////////////
//���µ���Ϣ������
XBOOL XImage::Append(const void *pData, XINT nSize)
{
#ifdef _SYMBIAN
#ifdef _LostImage
	return false;
#endif
	if(!m_pStream)
		m_pStream=new XStream;
	if(!m_pStream) return XFALSE;
	((XStream*)m_pStream)->WriteData(pData,nSize);
	return XTRUE;
#elif defined(_WIN32)
	if(m_pStream==XNULL) 
	{
		if(::CreateStreamOnHGlobal(NULL,XFALSE,(IStream**)&m_pStream))
		{
			m_pStream=XNULL;
			return XFALSE;
		}
	}
	XU32 nOutSize=0;
	((IStream*)m_pStream)->Write(pData,nSize,&nOutSize);
	return XTRUE;
#endif
}


////////////////////////////////////////////////////////////////////////
//����֡��
void XImage::SetFrames()
{
   m_nFrames=1;
#ifdef WINDOWS32
   if(m_pImage!=XNULL)
   {	   
	   ((Image*)m_pImage)->GetFrameDimensionsList((GUID*)&m_guid,1);
	   m_nFrames=((Image*)m_pImage)->GetFrameCount((GUID*)&m_guid);
	   InitDelay();
   }
#endif
#ifdef _SYMBIAN
   m_nFrames = m_nFrames>m_Mes.m_nFrames ? m_nFrames : m_Mes.m_nFrames;

#endif
}

/////////////////////////////////////////////////////////////////////////////////
//����Դ�ļ��л�ȡһ��ͼƬ
XBOOL XImage::CreateFromResource(XPCTSTR strName,XBOOL bBitmap)
{
#ifdef _LostImage
	return false;
#endif
	/*HRSRC hrsrc=::FindResource(NULL,strName,strRes);
	int nSize=SizeofResource(NULL,hrsrc);
	if(hrsrc==NULL) return XFALSE;
	HGLOBAL hObj=::LoadResource(NULL,hrsrc);
	if(hObj==NULL) return XFALSE;  
	void* pData=LockResource(hObj);
	Append(pData,nSize);
	UnlockResource(pData);
	//img.Create(hObj);
	DeleteObject(hObj);
	return Final();*/
	
	return XResource::LoadImage(this,strName,bBitmap); 
}

#define DEFAULT_DELAY 4

/////////////////////////////////////////////////////////////////////
//��ʼ��֡��֡���ʱ����
void XImage::InitDelay()
{
	//return;
#ifdef WINDOWS32
	if(m_pImage==XNULL||m_nFrames<=1) return;
	BYTE*pDelay=XNULL;
	PropertyItem*p=XNULL;

	XINT count=((Image*)m_pImage)->GetPropertyCount();
	if(count>0)
	{
		PROPID* pid=new PROPID[count];
		((Image*)m_pImage)->GetPropertyIdList(count,pid);
		
		for(int i=0;i<count;i++)
		{
			if(pid[i]!=PropertyTagFrameDelay) continue;
			int sz=((Image*)m_pImage)->GetPropertyItemSize(pid[i]);
			if(sz>0&&sz<10240)
			{
			   pDelay=new BYTE[sz];
			   p=(PropertyItem*)pDelay;
			   ((Image*)m_pImage)->GetPropertyItem(pid[i],sz,p);
			}
			break;
		}
		delete pid;
	}
	m_pDelay=new XU8[m_nFrames];
	for(XU32 i=0;i<m_nFrames;i++)
	{
	   if(p==XNULL||
		  i>=p->length) m_pDelay[i]=DEFAULT_DELAY;
	   else
	   {
		   int v=DEFAULT_DELAY;
		   switch(p->type)
		   {
		   case PropertyTagTypeByte:
		   case PropertyTagTypeASCII:
			    v=((BYTE*)p->value)[i]/5;
			    break;
		   case	PropertyTagTypeShort:
			    v=((WORD*)p->value)[i]/5;
			    break;
		   case PropertyTagTypeLong:
			    v=((DWORD*)p->value)[i]/5;
			    break;
		   }
		   m_pDelay[i]=(XU8)(XMIN(v,0xff));
	   }
	}
	if(pDelay!=XNULL) delete pDelay;
#endif
#ifdef _SYMBIAN
//	if(m_nFrames<=1) return;
//	if(m_pDelay)
//		delete m_pDelay;
//	m_pDelay = XNULL;
//	m_Mes.m_hDec->InitDelay(m_pDelay);
#endif
	
}


////////////////////////////////////////////////////////////////////////////////////
//�õ�ָ��֡�ӳ�
XU8 XImage::GetFrameDelay(int id)
{
#ifdef _LostImage
	return DEFAULT_DELAY;
#endif
	if(m_pDelay==XNULL) return DEFAULT_DELAY;
	else if(id<0||id>m_nFrames) return DEFAULT_DELAY;
	return m_pDelay[id];
}

//////////////////////////////////////////////////////////////////////////////////////
//�ͷ���
void XImage::ResetStream()
{
	if(m_pStream==XNULL) return ;
#ifdef WINDOWS32
	((IStream*)m_pStream)->Release();
#endif
#ifdef _SYMBIAN
	((XStream*)m_pStream)->Reset();
	delete m_pStream;
#endif
	m_pStream=XNULL; 
}

XBOOL XImage::GetBitmapData(XU8 *pBuffer, XU32 nLength)
{
	if(IsEmpty()||!m_bBitmap) return XFALSE;
	((CFbsBitmap*)m_pImage)->HorizontalTwipsToPixels(((CFbsBitmap*)m_pImage)->SizeInTwips().iWidth);
	((CFbsBitmap*)m_pImage)->LockHeap();
	memmove(pBuffer,((CFbsBitmap*)m_pImage)->DataAddress(),nLength);
	((CFbsBitmap*)m_pImage)->UnlockHeap();
	((CFbsBitmap*)m_pImage)->HorizontalTwipsToPixels(((CFbsBitmap*)m_pImage)->SizeInPixels().iWidth);
	return XTRUE;
}

XBOOL XImage::GetBitmapData(XU8*pData,int wi,int hi,int nBitsi,int nLength)//XDib &dib)
{
//	return false;
	if(GetPixelBits()==nBitsi)//dib.GetBits())
		return GetBitmapData(pData,nLength);
			//dib.GetBuffer(),dib.GetDataLength());
	if(IsEmpty()||!m_bBitmap) return XFALSE;
	if(nBitsi!=32) return XFALSE;
	
	CFbsBitmap pBmp;
	pBmp.Duplicate(((CFbsBitmap*)m_pImage)->Handle());
	
	//Bitmap* pBmp=(Bitmap*)m_pImage;
	TSize szz=pBmp.SizeInPixels();

	int w=szz.iWidth;
	int h=szz.iHeight;
	int nBits=GetPixelBits();
//	int iPitch=((((w*nBits)>>3)+3)>>2)<<2;
	int iPitch=w*(nBits>>3);
	XU32 rgba[256];
	if(nBits==8)
		GetPalette(rgba,256);
#ifdef _SYMBIAN
	int l=nLength/iPitch;
	h = h<l ? h : l;
	pBmp.LockHeap();
	{
		TRgb r;
		XU32*pDst=(XU32*)pData;
		h=XMIN(h,hi);
		w=XMIN(w,wi);
		for(int y=0;y<h;y++)
			{
			for(int x=0;x<w;x++)
				{
				pBmp.GetPixel(r,TPoint(x,y));
				pDst[x] = r.Internal();
				}
			pDst+=w;
			}
	}
	pBmp.UnlockHeap();
	return XTRUE;
#else
	
	pBmp->LockHeap();
	XU8*pSrt=(XU8*)pBmp->DataAddress();
	if(pSrt==XNULL)
		{
			pBmp->UnlockHeap();
			return XFALSE;
		}
		
	//Status s=pBmp->LockBits(&rt,0,nFmt,&data);
	//s=pBmp->GetLastStatus();
	//if(s==Ok)
	{
		//XU8*pSrt=(XU8*)data.Scan0;
		XU32*pDst=(XU32*)pData;
		h=XMIN(h,hi);
		w=XMIN(w,wi);
		for(int y=0;y<h;y++)
		{
		   int x1=0;
		   for(int x=0;x<w;x++)
		   {
			   	switch(nBits)
				{
				case 8:
					 pDst[x]=rgba[pSrt[x]];
					 break;
				case 16:
					{
						XU16 color=*((XU16*)(pSrt+(x<<1)));

						XU32 c;
						COLOR16TO32(color,c);
						pDst[x]=c;
					}
					break;
				case 24:
					{
					  XU8*p=(pSrt+x1);
					  x1+=3;					  
					  XU32 c;
					  COLOR24TO32(p,c);
					  pDst[x]=c;
					}
					break;
				case 32:
					{
					  pDst[x]=*(XU32*)(pSrt+(x<<2));
					}
					break;
				}
		   }
		   pSrt+=iPitch;
		   pDst+=wi;
		}
	}
	pBmp->UnlockHeap();
	return XTRUE;
#endif
}

XU8 XImage::GetPixelBits()
{
	if(IsEmpty()) return 0;
	
	
	TDisplayMode nFmt=((CFbsBitmap*)m_pImage)->DisplayMode();
	switch(nFmt)
		{
		default:
		case EGray256:return 8;
		case EColor64K:return 16;
		case EColor16M:return 24;
		case ERgb:
		case EColor16MU:
		case EColor16MA:return 32;
		}
}

XBOOL XImage::GetPalette(XU32 rgba[], int nCount)
{
    if(IsEmpty()) return XFALSE;
    CPalette* pal=XNULL;
	
	if(((CFbsBitmap*)m_pImage)->GetPalette(pal)==KErrNotSupported) return XFALSE;
	if(pal)
	{
		int nc=pal->Entries();
		if(nc>nCount) nc=nCount;
		for(int i=0;i<nc;i++)
		{
			rgba[i]=pal->GetEntry(i).Value();
		}
	}
	return XTRUE;
}
/*
XBOOL XImage::GetBitmapDataSkip(XU8*pData,int wi,int hi,int nBitsi,int nSkip)//XDib &dib)
{
	
//	return false;
	if(IsEmpty()||!m_bBitmap) return XFALSE;
	if(nBitsi!=32) return XFALSE;
	CFbsBitmap* pBmp=(CFbsBitmap*)m_pImage;

	int w=pBmp->SizeInPixels().iWidth;
	int h=pBmp->SizeInPixels().iHeight;
	int nBits=GetPixelBits();
//	int iPitch=(((((w*nBits)>>3)+3)>>2)<<2)<<nSkip;
	int iPitch=((w*nBits)>>3)<<nSkip;
//	Rect rt(0,0,w,h);
#ifdef _DEBUG
	int Tw = pBmp->SizeInTwips().iWidth;
	int Pw = pBmp->SizeInPixels().iWidth;
	if(Tw != Pw)
		{
		int a;
		a=0;
		}
#endif
//	XU32 nFmt=pBmp->HorizontalTwipsToPixels(pBmp->SizeInTwips().iWidth);
	XU32 nFmt=pBmp->HorizontalTwipsToPixels(640);
//	BitmapData data;
	XU32* data;
	XU32 rgba[256];
	if(nBits==8)
		GetPalette(rgba,256);
	pBmp->LockHeap();
	data = pBmp->DataAddress();
//	Status s=pBmp->LockBits(&rt,0,nFmt,&data);
	//s=pBmp->GetLastStatus();
//	if(s==Ok)
//	{
		XU8*pSrt=((XU8*)data)+(iPitch/2)+((nBits<<nSkip)>>4);
		XU32*pDst=(XU32*)pData;		
		int nSkip16=1+nSkip;
		int nSkip24=3<<nSkip;
		int nSkip32=2+nSkip;
		if(hi<<nSkip>h) hi=h>>nSkip;
		if(wi<<nSkip>w) hi=w>>nSkip;
		for(int y=0;y<hi;y++)
		{
		   int x1=0;
		   for(int x=0;x<wi;x++)
		   {
			   	switch(nBits)
				{
				case 8:
					 pDst[x]=rgba[pSrt[x<<nSkip]];
					 break;
				case 16:
					{
						XU16 color=*((XU16*)(pSrt+(x<<nSkip16)));

						XU32 c;
						COLOR16TO32(color,c);
						pDst[x]=c;
					}
					break;
				case 24:
					{
					  XU8*p=(pSrt+x1);
					  x1+=nSkip24;
					  XU32 c;
					  COLOR24TO32(p,c);
					  pDst[x]=c;
					}
					break;
				case 32:
					{
					  pDst[x]=*(XU32*)(pSrt+(x<<nSkip32));
					}
					break;
				}
		   }
		   pSrt+=iPitch;
		   pDst+=wi;
		   //pDst+=dib.Width();
		}
		pBmp->UnlockHeap();
		pBmp->HorizontalTwipsToPixels(pBmp->SizeInPixels().iWidth);
//	}
	return XTRUE;
	
	
//==============================I'm a confused cut-off rule ===========================
//	if(IsEmpty()||!m_bBitmap) return XFALSE;
//	if(nBitsi!=32) return XFALSE;
//	CFbsBitmap* pBmp=(CFbsBitmap*)m_pImage;
//
//	int w=pBmp->SizeInPixels().iWidth;
//	int h=pBmp->SizeInPixels().iHeight;
//	int nBits=GetPixelBits();
//	int iPitch=((w*nBits)>>3)<<nSkip;
//	int offset = (iPitch/2)/(nBits>>3);
//	int offsetY = offset/w;
//	int offsetX = offset%w;
//	
//	int nSkip16=1+nSkip;
//	int nSkip24=3<<nSkip;
//	int nSkip32=2+nSkip;
//	if(hi<<nSkip>h) hi=h>>nSkip;
//	if(wi<<nSkip>w) hi=w>>nSkip;
//	pBmp->LockHeap();
//	XU32*pDst=(XU32*)pData;	
//	int y=0;
//	int x=0;
//	TRgb r;
//	for(;y<hi;y++)
//		{
//		int x1=0;
//		for(;x<wi;x++)
//			{
//			switch(nBits)
//				{
//				case 8:
//					 pBmp->GetPixel(r,TPoint((x<<nSkip)%w,y+(x<<nSkip)/w));//[x<<nSkip]];
//					 pDst[x]=r.Internal();
//					 break;
//				case 16:
//					{
////						XU16 color=*((XU16*)(pSrt+(x<<nSkip16)));
////
////						XU32 c;
////						COLOR16TO32(color,c);
////						pDst[x]=c;
//						pBmp->GetPixel(r,TPoint((x<<nSkip16)%w,y+(x<<nSkip16)/w));
//						pDst[x]=r.Internal();
//					}
//					break;
//				case 24:
//					{
////					  XU8*p=(pSrt+x1);
////					  x1+=nSkip24;
////					  XU32 c;
////					  COLOR24TO32(p,c);
////					  pDst[x]=c;
//						pBmp->GetPixel(r,TPoint(x1%w,y+x1/w));
//						pDst[x]=r.Internal();
//						x1+=nSkip24;
//					}
//					break;
//				case 32:
//					{
////					  pDst[x]=*(XU32*)(pSrt+(x<<nSkip32));
//						pBmp->GetPixel(r,TPoint((x<<nSkip32)%w,y+(x<<nSkip32)/w));
//						pDst[x] = r.Internal();
//					}
//					break;
//				}
//			}
//		x=0;
//		pDst+=wi;
//		}
//	pBmp->UnlockHeap();
//	return XTRUE;

}
*/
XBOOL XImage::GetBitmapDataSkip(XU8*pData,int wi,int hi,int nBitsi,int nSkip)//XDib &dib)
	{
	if(IsEmpty()||!m_bBitmap) return XFALSE;
	if(nBitsi!=32) return XFALSE;
	CFbsBitmap* pBmp=(CFbsBitmap*)m_pImage;

	int w=pBmp->SizeInPixels().iWidth;
	int h=pBmp->SizeInPixels().iHeight;
	hi = (hi)>(h>>nSkip) ? (h>>nSkip) : hi;
//	wi = (wi)>(w>>nSkip) ? (w>>nSkip) : wi;
	pBmp->LockHeap();
	XU32*pDst=(XU32*)pData;	
	int l = hi*wi;
	int xi=0 , yi=0;
	TRgb r;
	for(int y=0 ; y<hi ;y++)
		{
		for(int x=0 ; x<wi ; x++)
			{
			xi=x<<nSkip;
			yi=y<<nSkip;
			if(xi>w)
				continue;
			pBmp->GetPixel(r,TPoint(xi,yi));
			pDst[x] = r.Internal();
			}
		pDst+=wi;
		}
	pBmp->UnlockHeap();
//	===============================I'm a crazy cut-off rule==================================
//	for(int i=0 ; i<l ; i++)
//		{
//		pBmp->GetPixel(r,TPoint(xi,yi));
//		pDst[l] = r.Internal();
//		xi+=2;
//		if(xi>w)
//			{
//			yi+=2;
//			xi=0;
//			}
//		}
	return XTRUE;
	}
