// XImage.cpp: implementation of the XImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxggbase.h"
#include "XImage.h"
#include "XResource.h"
#include "XDib.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef _ARGB_PIXEL_
	#define COLOR16TO32A(C16,C32) {C32=0;if(C16&0x8000) C32|=0xff000000;\
								  C32|=((((C16>>10)&0x3f)<<3));\
								  C32|=((((C16>>5)&0x3f)<<3)<<8);\
								  C32|=((((C16)&0x3f)<<3)<<16);}
	#define COLOR24TO32A(C24,C32) (C32=0xff000000|(C24[0]<<16)|(C24[1]<<8)|C24[2])
	#define SWAPPIXELRB(COLOR) {XU8 n=((XU8*)&COLOR)[0];((XU8*)&COLOR)[0]=((XU8*)&COLOR)[2];((XU8*)&COLOR)[2]=n;}
#else
	#define COLOR16TO32A COLOR16TO32
	#define COLOR24TO32A COLOR24TO32
#endif
 XImage::XImage()
{
  m_pImage=XNULL;
  m_pStream=XNULL;
  m_nFrames=0;
  m_pDelay=XNULL;
  m_bBitmap=XFALSE;
}

XImage::~XImage()
{
	Release();  
}

XU32 XImage::Width() const
{
	if(m_pImage!=XNULL)
		return ((Image*)m_pImage)->GetWidth();
	return 0;
}

XU32 XImage::Height() const
{
	if(m_pImage!=XNULL)
		return ((Image*)m_pImage)->GetHeight();	
	return 0;
}

XBOOL XImage::IsEmpty()const
{	 
   if(m_pImage==XNULL) return XTRUE;
   /*if(((Image*)m_pImage)->GetLastStatus()!=Ok) return XTRUE;
   if(Width()<=0||
	  Height()<=0) return XTRUE;*/
   return XFALSE;
}

void XImage::Release()
{
   if(m_pImage!=XNULL) delete (Image*)m_pImage;
   if(m_pDelay!=XNULL) delete m_pDelay;
   if(m_pStream!=XNULL) 
	   ((IStream*)m_pStream)->Release();
	   //delete m_pStream;
   m_pDelay=XNULL;
   m_pStream=XNULL;
   m_pImage=XNULL;
   m_nFrames=0;

}

XINT XImage::FrameCount()
{
   return m_nFrames;
}

XBOOL XImage::Create(XPCWSTR strFile)
{
	Release();
	m_pImage=new Image(strFile);
	if(((Image*)m_pImage)->GetLastStatus()!=Ok)
	{
		Release();
		return XFALSE;
	}
	SetFrames();
	return m_pImage!=XNULL;
}

XBOOL XImage::SetActive(XINT index)
{
	if(m_pImage==XNULL) return XFALSE;
	((Image*)m_pImage)->SelectActiveFrame((GUID*)&m_guid,index);
	
	return XTRUE;
}

XBOOL XImage::Final(XU8 bBitmap)
{
	if(m_pStream==XNULL) return XFALSE;
	if(m_pImage!=XNULL) delete (XImage*)m_pImage;
	if(bBitmap)
	{
		Bitmap*pBitmap=new Bitmap((IStream*)m_pStream);
		m_pImage=pBitmap;

		
	}
	else
		m_pImage=new Image((IStream*)m_pStream);  
	Status s=((Image*)m_pImage)->GetLastStatus();
	if(s!=Ok)
	{
		Release();
		return XFALSE;
	}
	((IStream*)m_pStream)->Release();
	m_pStream=XNULL; 
	SetFrames();
	m_bBitmap=bBitmap;
	return XTRUE;
}

XBOOL XImage::Append(const void *pData, XINT nSize)
{
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
}

void XImage::SetFrames()
{
   m_nFrames=1;
   if(m_pImage!=XNULL)
   {
	   
	   ((Image*)m_pImage)->GetFrameDimensionsList((GUID*)&m_guid,1);
	   m_nFrames=((Image*)m_pImage)->GetFrameCount((GUID*)&m_guid);
	   InitDelay();
   }
}

XBOOL XImage::CreateFromResource(XPCTSTR strName,XBOOL bBitmap)
{
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

void XImage::InitDelay()
{
	//return;
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
	
	
}

XU8 XImage::GetFrameDelay(int id)
{
	if(m_pDelay==XNULL) return DEFAULT_DELAY;
	else if(id<0||id>m_nFrames) return DEFAULT_DELAY;
	return m_pDelay[id];
}

void XImage::ResetStream()
{
	if(m_pStream==XNULL) return ;
	
	((IStream*)m_pStream)->Release();
	m_pStream=XNULL; 
	//SetFrames();
}

XBOOL XImage::GetPalette(XU32 rgba[], int nCount)
{
    if(IsEmpty()) return XFALSE;
	Bitmap*pImage=(Bitmap*)m_pImage;
	int nSize=pImage->GetPaletteSize();
	XU32*pData=new XU32[nCount+2];
	if(pData==XNULL) return XFALSE;
	pData[0]=1;
	pData[1]=nCount;
	XU8 bRet=XFALSE;
	Status s=pImage->GetPalette((ColorPalette*)pData,nSize);//sizeof(ColorPalette));
	if(s==Ok)
	{
		bRet=XTRUE;
		BYTE*pDst=(BYTE*)rgba;
		BYTE*pSrt=(BYTE*)(pData+2);
		for(int i=0;i<nCount;i++)
		{
#ifdef _ARGB_PIXEL_
			pDst[0]=pSrt[2];//3];//i+3];
			pDst[1]=pSrt[1];//0];
			pDst[2]=pSrt[0];//];
#else
			pDst[0]=pSrt[0];//3];//i+3];
			pDst[1]=pSrt[1];//0];
			pDst[2]=pSrt[2];//];
#endif
			pDst[3]=pSrt[3];
			pDst+=4;
			pSrt+=4;
		}
	}
	delete pData;
	return bRet;
	//ColorPalette pal={0,256,rgba};
}

XU8 XImage::GetPixelBits()
{
	if(IsEmpty()) return 0;
	Image*pImage=(Image*)m_pImage;
	XU32 f=pImage->GetPixelFormat();
	return (XU8)((f>>8)&0xff);
}

XBOOL XImage::GetBitmapData(XU8 *pBuffer, XU32 nLength)
{
	if(IsEmpty()||!m_bBitmap) return XFALSE;
	
	Bitmap* pBmp=(Bitmap*)m_pImage;

	int w=pBmp->GetWidth();
	int h=pBmp->GetHeight();
	Rect rt(0,0,w,h);
	BitmapData data;
	XU32 nFmt=pBmp->GetPixelFormat();
	Status s=pBmp->LockBits(&rt,0,nFmt,&data);
	if(s==Ok)
	{
		memcpy(pBuffer,data.Scan0,nLength);
		pBmp->UnlockBits(&data);
	}
	return XTRUE;
}


XBOOL XImage::GetBitmapDataSkip(XU8*pData,int wi,int hi,int nBitsi,int nSkip)//XDib &dib)
{
	if(IsEmpty()||!m_bBitmap) return XFALSE;
	if(nBitsi!=32) return XFALSE;
	Bitmap* pBmp=(Bitmap*)m_pImage;

	int w=pBmp->GetWidth();
	int h=pBmp->GetHeight();
	int nBits=GetPixelBits();
	int iPitch=(((((w*nBits)>>3)+3)>>2)<<2)<<nSkip;
	Rect rt(0,0,w,h);
	XU32 nFmt=pBmp->GetPixelFormat();
	BitmapData data;
	XU32 rgba[256];
	if(nBits==8)
		GetPalette(rgba,256);
	Status s=pBmp->LockBits(&rt,0,nFmt,&data);
	//s=pBmp->GetLastStatus();
	if(s==Ok)
	{
		XU8*pSrt=((XU8*)data.Scan0)+(iPitch/2)+((nBits<<nSkip)>>4);
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
						COLOR16TO32A(color,c);
						pDst[x]=c;
					}
					break;
				case 24:
					{
					  XU8*p=(pSrt+x1);
					  x1+=nSkip24;
					  XU32 c;
					  COLOR24TO32A(p,c);
					  pDst[x]=c;
					}
					break;
				case 32:
					{
					  pDst[x]=*(XU32*)(pSrt+(x<<nSkip32));
#ifdef _ARGB_PIXEL
					  SWAPPIXELRB(pDst[x]);
#endif
					}
					break;
				}
		   }
		   pSrt+=iPitch;
		   pDst+=wi;
		   //pDst+=dib.Width();
		}
	}
	return XTRUE;
}

XBOOL XImage::GetBitmapData(XU8*pData,int wi,int hi,int nBitsi,int nLength)//XDib &dib)
{
	if(GetPixelBits()==nBitsi)//dib.GetBits())
	{
		XBOOL bOK=GetBitmapData(pData,nLength);
#ifdef _ARGB_PIXEL_
		if(bOK)
		{
			XU32*pDst=(XU32*)pData;
			nLength>>=2;
			for(int i=0;i<nLength;i++)
			{
				SWAPPIXELRB(pDst[i]);
			}
		}
#endif
		return bOK;
	}
			//dib.GetBuffer(),dib.GetDataLength());
	if(IsEmpty()||!m_bBitmap) return XFALSE;
	if(nBitsi!=32) return XFALSE;
	Bitmap* pBmp=(Bitmap*)m_pImage;

	int w=pBmp->GetWidth();
	int h=pBmp->GetHeight();
	int nBits=GetPixelBits();
	int iPitch=((((w*nBits)>>3)+3)>>2)<<2;
	Rect rt(0,0,w,h);
	XU32 nFmt=pBmp->GetPixelFormat();
	BitmapData data;
	XU32 rgba[256];
	if(nBits==8)
		GetPalette(rgba,256);
	Status s=pBmp->LockBits(&rt,0,nFmt,&data);
	//s=pBmp->GetLastStatus();
	if(s==Ok)
	{
		XU8*pSrt=(XU8*)data.Scan0;
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
						COLOR16TO32A(color,c);
						pDst[x]=c;
					}
					break;
				case 24:
					{
					  XU8*p=(pSrt+x1);
					  x1+=3;					  
					  XU32 c;
					  COLOR24TO32A(p,c);
					  pDst[x]=c;
					}
					break;
				case 32:
					{
					  pDst[x]=*(XU32*)(pSrt+(x<<2));
#ifdef _ARGB_PIXEL_
					  SWAPPIXELRB(pDst[x]);
#endif
					}
					break;
				}
		   }
		   pSrt+=iPitch;
		   pDst+=wi;
		   //pDst+=dib.Width();
		}
	}
	return XTRUE;
}
