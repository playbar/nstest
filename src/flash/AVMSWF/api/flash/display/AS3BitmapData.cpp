#include "StdAfxflash.h"
#include "XDrawDib.h"
#include "Raster_gl.h"
#include "ShellCore.h"
#include "sobject.h"
#include "splay.h"
#include "splayer.h"
using namespace avmplus;
#include "AS3BitmapData.h"
#include "AS3Rectangle.h"
#include "AS3Point.h"
#include "AS3Matrix.h"
#include "AS3ColorTransform.h"
#include "AS3DisplayObject.h"
#include "AS3ByteArray.h"
#include "AS3Bitmap.h"
#include "AS3ByteArray.h"
#include "AS3Bitmap.h"
#include "AS3BlendMode.h"
#include "AS3StageQuality.h"
#include "avm2.h"
#include "../libpng/BWPngEncode.h"

#define MINDIVSIZE 192

//#define USECHAR(CH,OBJ)\
//{\
//	OBJ->m_pChar=CH;\
//	{\
//		ch->bits.AddUse();\
//	}\
//}

namespace avmshell{
BitmapDataClass::BitmapDataClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())BitmapDataObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* BitmapDataClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) BitmapDataObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

BitmapDataObject* BitmapDataClass::CreateBitmapData(int w,int h,bool bTransparent)
{
	BitmapDataObject*pObj=(BitmapDataObject*)createInstance(ivtable(),prototype);
	pObj->AS3_constructor(w,h,bTransparent,0xffffffff);
	return pObj;
}

BitmapDataObject* BitmapDataClass::CreateDumpBitmap(void*pChar)
{
	BitmapDataObject*pObj=(BitmapDataObject*)createInstance(ivtable(),prototype);
	SCharacter*ch=(SCharacter*)pChar;
	//pObj->m_pChar=pChar;	
	//USECHAR(ch,pObj);
	pObj->m_pChar = ch;
	ch->bits.AddUse();
	pObj->LockBitmap(false);
	return pObj;
}

_TEXTUREID* BitmapDataObject::GetBmInfo(TextureInfo&info)
{
	memset(&info,0,sizeof(TextureInfo));
	if(!m_pChar) return XNULL;
	if(!m_pChar->bits.width&&m_pChar->data)
		XSWFPlayer::m_pInstance->CreateIfEmpty(m_pChar,false);
	info.w=m_pChar->bits.width;
	info.h=m_pChar->bits.height;
	info.nDiv=m_pChar->bits.nImgDiv;
	info.preAlpha = m_pChar->bits.preAlpha;
	info.transparent = m_pChar->bits.transparent!=0;
	info.id = m_pChar->bits.bitmapID;

//	if(info.w>5000)
//		int v=0;

	return m_pChar->bits.bitmapID;
}

void BitmapDataObject::CheckEmpty()
{
	if(!m_pChar) return;
	if(m_pChar->bits.width) return;
	XSWFPlayer::m_pInstance->CreateIfEmpty(m_pChar,false);
}


bool BitmapDataObject::SaveIfNeed(XU32 t,XU32 tMax)
{
	//return false;
	if(!m_pChar) return false;
	SBitmapCore* bm=&m_pChar->bits;
	if(bm->HasBits()&&t-bm->m_nLockTime>tMax)
	{
		if(m_pChar->type==bitmapEmptyChar)
		{
			XXVar strFile=XSWFPlayer::m_pInstance->m_strWorkCache,iVar((int)m_pChar);
			strFile.StringAdd(iVar);
			XFile file;
			if(file.Open(strFile.strTxt,XFile::XBINARY|XFile::XCREATE|XFile::XWRITE))
			 {
				 file.Write(bm->baseAddr,bm->rowBytes*bm->height);
				 file.Close();
				 bm->ReleaseData();
			 }
		}
		else bm->ReleaseData();
	}
	_TEXTUREID* tId=bm->bitmapID;
	m_bTextured = tId!=NULL;
    if(!tId||RasterGL::IsLockDisplay(tId)||tId->nUseCount>1) return false;
    if(t-tId->nLastTime>tMax)
	{
		/*if(m_pChar->type==bitmapEmptyChar)
		{
			XXVar strFile=XSWFPlayer::m_pInstance->m_strWorkCache,iVar((int)this);
			strFile.StringAdd(iVar);
			XFile file;
			if(file.Open(strFile.strTxt,XFile::XBINARY|XFile::XCREATE|XFile::XWRITE))
			 {
				 file.Write(m_pChar->bits.baseAddr,m_pChar->bits.rowBytes*m_pChar->bits.height);
				 file.Close();
				 //_SAVETRACE(tId);
				 //delete m_pChar->frame;
				 //m_pChar->frame = NULL;
				 //m_bSaved = true;
				 //return true;
				 m_pChar->bits.ReleaseData();
			 }
		}*/
		//RasterGL::sharedRasterGL()->DeleteTexture(m_pChar->bits.bitmapID);
		//m_pChar->bits.bitmapID = XNULL;
		m_pChar->bits.PIFree();
		//m_bSaved=true;
	}	
	if(!bm->HasBits()&&!bm->bitmapID)
	{
		ShellToplevel*top=(ShellToplevel*)toplevel();
		top->RemoveBitmap(this);
		m_bSaved = true;
	}
	return false;
}
bool BitmapDataObject::RestoreIfNeed()
{
	if(!m_pChar) return false;
	//_TEXTUREID* tId=NULL;
	SBitmapCore* bm=&m_pChar->bits;
	if(!bm->HasBits())
	{
		if(m_pChar->type==bitmapEmptyChar)
		{
			 XXVar strFile=XSWFPlayer::m_pInstance->m_strWorkCache,iVar((int)m_pChar);
			 strFile.StringAdd(iVar);
			 XFile file;
			 if(file.Open(strFile.strTxt,XFile::XBINARY|XFile::XREAD))
			 {
					bm->PICreate(bm->width,bm->height,bm->nImgDiv);
					file.Read(bm->baseAddr,bm->rowBytes*bm->height);
					file.Close();
					//m_pChar->bits.CreateTexture();
			}
			 
		}
		else if(m_pChar->data)
			XSWFPlayer::m_pInstance->CreateIfEmpty(m_pChar,false);
	}
	//if(m_bTextured)
	//	bm->CreateTexture();
	if(m_bSaved)
	{
		ShellToplevel*top=(ShellToplevel*)toplevel();
		top->AddBitmap(this);
		m_bSaved = false;
	}
	return true;
}

bool BitmapDataObject::IsTransparent()
{
	if(!m_pChar) return false;
	return m_pChar->bits.transparent;
}


XBOOL BitmapDataObject::GetDib(XDrawDib&dib)
{
	LockBitmap(false);
	//if(IsEmpty())
	//	return XFALSE;
	if(!m_pChar->bits.HasBits()) return XFALSE;
	dib.CreateFrom(&m_pChar->bits,false);
	//TextureInfo info;
	//GetBmInfo(info);
	//dib.Create(info.w,info.h,32);
	//RasterGL::sharedRasterGL()->ReadTexture(info,dib);
	//m_pChar->frame->GetDib(dib,true);
	return XTRUE;
}

void BitmapDataObject::CreateTexture()
{
	if(!m_pChar) return;

	/*if(m_bModify&&m_pChar->bits.width==3200)
	{
		int a=0;
		int b=0;
	}*/

	if(!m_pChar->bits.bitmapID)
		RestoreIfNeed();
	SBitmapCore* bm=&m_pChar->bits;
	if(bm->bitmapID)
	{
		if(!m_bModify) return;
		RasterGL::sharedRasterGL()->UpdateTexture(bm->bitmapID->id,bm->baseAddr,Texture2DPixelFormat_RGBA8888,0,0,bm->width,bm->height);
		//bm->CreateTexture();
	}
	else
	{
		if(bm->HasBits())
		{
			bm->CreateTexture();
			if(m_pChar->type!=bitmapEmptyChar)
				bm->ReleaseData();
		}
		else
		{
			if(m_pChar->data)
				XSWFPlayer::m_pInstance->CreateIfEmpty(m_pChar,true);
		}
	}
	if(bm->HasBits())
	{
		if(m_pChar->type==bitmapEmptyChar)
		{
			XXVar strFile=XSWFPlayer::m_pInstance->m_strWorkCache,iVar((int)m_pChar);
			strFile.StringAdd(iVar);
			XFile file;
			if(file.Open(strFile.strTxt,XFile::XBINARY|XFile::XCREATE|XFile::XWRITE))
			 {
				 file.Write(bm->baseAddr,bm->rowBytes*bm->height);
				 file.Close();
				 bm->ReleaseData();
			 }
		}
		else bm->ReleaseData();
	}
	m_bModify = false;
}

void BitmapDataObject::LockBitmap(bool forWrite)
{
	RestoreIfNeed();
	if(!forWrite)
	{
		if(!m_pChar->bits.HasBits())
			XSWFPlayer::m_pInstance->CreateIfEmpty(m_pChar,false);
		m_pChar->bits.FormatBitmap();
	}
	else
	{
		if(m_pChar->type==bitmapEmptyChar&&m_pChar->bits.useCount<=1&&m_pChar->bits.HasBits())
		{
			m_pChar->bits.useCount = 1;
			m_pChar->bits.m_nLockTime = XSWFPlayer::m_pInstance->m_nRunTime;
			return;
		}
		XSWFPlayer* player = XSWFPlayer::m_pInstance;
		SCharacter*ch = player->CreateCharacter();
		ch->type = bitmapEmptyChar;
		ch->record.Init();
		//bool bClear=false;
		if(!m_pChar->bits.HasBits())
		{
			player->CreateIfEmpty(m_pChar,false);
			//bClear=true;
		}
		SBitmapCore* bits=&m_pChar->bits;
		//TextureInfo info;
		//GetBmInfo(info);
		ch->bits.PICreate(bits);		
		ch->bits.AddUse();
		ch->rbounds.xmin = 0;
		ch->rbounds.ymin = 0;
		ch->rbounds.xmax = (bits->width*20)<<bits->nImgDiv;
		ch->rbounds.ymax = (bits->height*20)<<bits->nImgDiv;
		
		ReleaseChar(m_pChar);
		m_pChar = ch;
	}
	m_pChar->bits.m_nLockTime = XSWFPlayer::m_pInstance->m_nRunTime;
}


#ifdef _ANALY
	void BitmapDataObject::Report(XSortMemName*pInfo)
	{
		StUTF8String str(vtable->traits->name());
		int nSize = vtable->traits->getTotalSize();
		//if(!m_bAttach)
		if(m_pChar)
		{
			if(m_pChar->bits.bitmapID)
				nSize+=m_pChar->bits.width*m_pChar->bits.height*4;
			if(m_pChar->bits.pAlloc)
				nSize+=m_pChar->bits.width*m_pChar->bits.height*4;
		}
		pInfo->Add(str.c_str(),nSize);
	}
#endif


/*BitmapDataObject* BitmapDataClass::CreateBitmapData(void*s)
{
	BitmapDataObject*pObj=(BitmapDataObject*)createInstance(ivtable(),prototype);
	pObj->Create(s);
	return pObj;
}*/

BitmapDataObject* BitmapDataClass::CreateBitmapData(XImage*pImage)
{
	BitmapDataObject*pObj=(BitmapDataObject*)createInstance(ivtable(),prototype);
	pObj->Create(pImage);
	//pObj->AddBitmap();
	return pObj;
}

static int _bitmapCount = 0;

BitmapDataObject::BitmapDataObject(VTable *vtable, ScriptObject* proto, int capacity): FlashObject(vtable, proto, 0)
{
	m_pChar=XNULL;
	m_pObject=XNULL;
	m_bSaved = false;
	m_bTextured = false;
	//XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	ShellToplevel*top=(ShellToplevel*)toplevel();
	top->AddBitmap(this);
	m_bModify = false;
	_bitmapCount++;
	//player->AddBitmap
	//return ch;
//	m_bAttach = false;
}

void BitmapDataObject::Release()
{
	_bitmapCount--;
	if(IsRunning())
	{
		ShellToplevel*top=(ShellToplevel*)toplevel();
		top->RemoveBitmap(this);
	}
//	if(IsRunning()&&m_pChar)
//	{
////#ifdef _WINEMU
////		TextureInfo info;
////		GetBmInfo(info);
////		XString16 ss("----- Release BitmapData W:"),s1;
////		s1.FromInt(info.w);
////		ss+=s1;ss+=XString16(",H:");
////		s1.FromInt(info.h);
////		ss+=s1;
////		XSWFPlayer::m_pInstance->Trace(ss);
////#endif
//		_XSCharacter*ch=(_XSCharacter*)m_pChar;
//		if((ch->type==loaderImageChar||ch->type==loaderChar)&&ch->classEntry)
//		{
//			((ShellToplevel*)toplevel())->UnLockImage((Stringp)ch->classEntry);
//		}
//	}
	if(m_pChar)
	{
		ReleaseChar(m_pChar);
		m_pChar = XNULL;
	}

}
int BitmapDataObject::AS3_height_get()
{
	//if(m_pChar&&m_pChar->bits.height
	TextureInfo info;
	GetBmInfo(info);
	return info.nDiv?(info.h<<info.nDiv):info.h;	
	return 0;
}
RectangleObject* BitmapDataObject::AS3_rect_get()
{
	RectangleClass*pClass=((ShellToplevel*)toplevel())->getRectangleClass();
	if(m_pChar)
	{
		//SBitmapCore*bits=&m_pChar->bits;
		TextureInfo info;
		GetBmInfo(info);
		return info.nDiv?pClass->CreateRectangle(0,0,info.w<<info.nDiv,info.h<<info.nDiv):pClass->CreateRectangle(0,0,info.w,info.h);
	}
	return pClass->CreateRectangle(0,0,this->AS3_width_get(),this->AS3_height_get());//Width(),Height());
	//else
	//	return pClass->CreateRectangle(0,0,0,0);
	//return NULL;
}
bool BitmapDataObject::AS3_transparent_get()
{
	if(!m_pChar) return false;
	return m_pChar->bits.transparent;
	//return false;
}
int BitmapDataObject::AS3_width_get()
{
	//return m_pDib?m_pDib->Width():	
	TextureInfo info;
	GetBmInfo(info);
	return info.nDiv?(info.w<<info.nDiv):info.w;	
	return 0;
}

void BitmapDataObject::ReleaseChar(_XSCharacter*ch)
{
	if(!ch) return;
	//if(ch->bits.HasBits())
	//	int v=0;
	ch->bits.ReleaseUse();
	if(ch->bits.useCount<=0)
	{
		if(ch->type==bitmapEmptyChar)
			XSWFPlayer::m_pInstance->FreeCharacter(ch);
		else
			ch->bits.PIFree();
	}
}

bool BitmapDataObject::Create(XImage*pImage)
{
	ReleaseChar(m_pChar);
	int w=pImage->Width();
	int h=pImage->Height();
//	m_bAttach = false;
	XSWFPlayer*player = XSWFPlayer::m_pInstance;
	m_pChar = player->CreateCharacter();
	m_pChar->type = bitmapEmptyChar;//bitsChar;	
	int w0 = w;
	int h0 = h;
	int nDiv = 0;
	while(w>MAXTEXTURE||h>MAXTEXTURE)
	{
		w>>=1;
		h>>=1;
		nDiv++;
	}
	m_pChar->bits.PICreate(w,h,nDiv);
	m_pChar->bits.AddUse();
	m_pChar->record.Init();
	{
		_XSCharacter* ch = m_pChar;
		ch->rbounds.xmin = 0;
		ch->rbounds.ymin = 0;
		ch->rbounds.xmax = (w0*20);
		ch->rbounds.ymax = (h0*20);
	}
	if(nDiv)
		pImage->GetBitmapDataSkip((XU8*)m_pChar->bits.baseAddr,w,h,32,nDiv);
	else
		pImage->GetBitmapData((XU8*)m_pChar->bits.baseAddr,w,h,32,w*h*4);

	m_pChar->bits.SwapRB();

	return true;
	
}



//void BitmapDataObject::CreateDummyDib(void*pData)
//{
//	if(!m_pChar) return;
//	_XSCharacter*ch=(_XSCharacter*)pData;
//	//ch->type = bitmapEmptyChar;
//	if(m_pDib&&(XU8*)m_pDib->m_pDib==(XU8*)ch->bits.baseAddr) return;
//	m_nWidth0=ch->bits.width0;
//	m_nHeight0=ch->bits.height0;
//
//
//
//	if(ch->bits.bmFormat==bm32Bit)
//	{
//		if(m_pDib==NULL)
//			m_pDib=new XDrawDib();
//		m_pDib->CreateFrom(&ch->bits,XFALSE);
//		//ch->bits.LockBits();
//	}
//	//m_pChar = NULL;
//}


void BitmapDataObject::SetObject(void*pObject)
{
	m_pObject=(_XSObject*)pObject;
}

void BitmapDataObject::AS3_constructor(int w,int h,bool transparent,uint32_t fillColor)
{
	//m_pDib=new XDrawDib();
	//AvmAssert(m_pDib!=XNULL);
	//VTable*t=this->vtable;
	//if(w>1&&h>1)
	//	int vvv=0;
	
	void*pInitData=this->vtable->GetInitData();//m_pInitData;
	if(pInitData)
	{
		_XSCharacter*ch=(_XSCharacter*)pInitData;
		//ch->type = bitmapEmptyChar;
		m_pChar=ch;
		m_pChar->bits.AddUse();
		LockBitmap(false);
	}
	else
	{
		m_pChar = XSWFPlayer::m_pInstance->CreateCharacter();		
		int w0 = w;
		int h0 = h;
		int nDiv = 0;
		while(w>MAXTEXTURE&&h>MAXTEXTURE)
		{
			w>>=1;
			h>>=1;
			nDiv++;
		}
		m_pChar->bits.PICreate(w,h,nDiv);
		//m_pChar->bits.preAlpha = true;
		m_pChar->bits.transparent=transparent;
		if(!m_pChar->bits.HasBits())
			toplevel()->throwError(kOutOfMemoryError);
		m_pChar->bits.AddUse();
		m_pChar->type = bitmapEmptyChar;
		{
			_XSCharacter* ch = m_pChar;
			ch->rbounds.xmin = 0;
			ch->rbounds.ymin = 0;
			ch->rbounds.xmax = w0*20;
			ch->rbounds.ymax = h0*20;
		}
		
		//m_pChar->frame.pBuffer->transparent=transparent;
		if(!transparent)
				fillColor|=0xff000000;
		//SRECT rect={0,0,w,h};
		//SWAPPIXELRB(fillColor);
#ifndef _ARGB_PIXEL_
		SWAPPIXELRB(fillColor);
#endif
		XDrawDib dib;
		dib.CreateFrom(&m_pChar->bits,true);
		dib.Clear(fillColor);
		//m_pChar->frame->DrawRect(0,0,w,h,fillColor);
	}

	/*if(AS3_height_get()==149)
		int v=0;
	if(AS3_width_get()==0||
	   AS3_height_get()==0)
	   int v=0;*/
	
}
void BitmapDataObject::AS3_applyFilter(BitmapDataObject*,RectangleObject*,PointObject*,BitmapFilterObject*)
{
	//not support filters
}
BitmapDataObject* BitmapDataObject::AS3_clone()
{
	//RestoreIfNeed();
	BitmapDataClass*pClass=((ShellToplevel*)toplevel())->getBitmapDataClass();
	SCharacter*ch = m_pChar;
	if((ch->type==loaderImageChar||ch->type==loaderChar)&&ch->classEntry)
	{
		((ShellToplevel*)toplevel())->LockImage((Stringp)ch->classEntry);
	}
	return pClass->CreateDumpBitmap(ch);
	//return NULL;
}
void BitmapDataObject::AS3_colorTransform(RectangleObject*r,ColorTransformObject*c)
{
	//LockBitmap();
	//ExpandDummy(XTRUE);
	LockBitmap(true);
	if(!IsEmptyBits())
	{
		XDrawDib dib;
		dib.CreateFrom(&m_pChar->bits,true);
		//XDrawDib*m_pDib=&dib;
		AS3RECTANGLEDATA&data=r->GetData();		
		XRect rect(data.left,data.top,data.left+data.width,data.top+data.height);
		XCXForm form;
		c->GetCXForm(form);
		form.SwapRB();
		int nDiv=m_pChar->bits.nImgDiv;
		if(nDiv)
		{
			rect.left>>=nDiv;
			rect.right>>=nDiv;
			rect.top>>=nDiv;
			rect.bottom>>=nDiv;
		}
		dib.Transform(rect,form);
		m_bModify = true;
	}
}
Atom BitmapDataObject::AS3_compare(BitmapDataObject*src)
{
	LockBitmap(false);
	src->LockBitmap(false);
	AvmCore* c=core();
	if(IsEmptyBits()||src->IsEmptyBits())
	{
		if(m_pChar==src->m_pChar)
			return zeroIntAtom;
		else
			return c->intToAtom(-3);
	}
	if(m_pChar->bits.width!=src->m_pChar->bits.width)
		return c->intToAtom(-3);
	else if(m_pChar->bits.height!=src->m_pChar->bits.height)
		return c->intToAtom(-4);
	else
	{
		//XDrawDib dib;
		//dib.CreateFrom(&m_pChar->bits,true);
		//XDrawDib* m_pDib=&dib;
		int w=m_pChar->bits.width,h=m_pChar->bits.height;
		BitmapDataObject*pObj=AS3_clone();
		pObj->LockBitmap(true);
		//XDrawDib dib1;
		//dib1.CreateFrom(&src->m_pChar->bits,true);
		//AvmAssert(pObj!=NULL&&pObj->m_pDib!=NULL);
		bool bSame=true;
		int x,t=w*h;
		XU32*pDst=(XU32*)pObj->m_pChar->bits.baseAddr;//pObj->m_pDib->GetBuffer();
		XU32*pSrc=(XU32*)src->m_pChar->bits.baseAddr;//src->m_pDib->GetBuffer();
		//for(y=0;y<h;y++)
		//{
		//	for(x=0;x<w;x++)
			for(x=0;x<t;x++)
			{
				if(pDst[x]!=pSrc[x])
				{	
					bSame=false;
					XU8*p1=(XU8*)&pDst[x];
					XU8*p2=(XU8*)&pSrc[x];
					if(((pDst[x])&0xffffff)!=((pSrc[x])&0xffffff))
					{											
						p1[3]=0xff;
						p1[1]-=p2[1];
						p1[2]-=p2[2];
						p1[0]-=p2[0];
					}
					else
					{
						pDst[x]=0xffffff|((p1[3]-p2[3])<<24);
					}
					//pDst[x]=0xff000000|(												
				}
				else pDst[x]=0;
			}
			//pSrc+=w;
			//pDst+=w;
		//}
		return bSame?c->intToAtom(0):pObj->atom();
	}
	return kAvmThunkUndefined;
}

void BitmapDataObject::DivImage(int nNewDiv)
{
	LockBitmap(true);
	if(IsEmptyBits()) return;
	m_pChar->bits.nImgDiv = (nNewDiv-m_pChar->bits.nImgDiv);
	m_pChar->bits.DivImage();
	m_pChar->bits.nImgDiv = nNewDiv;
	m_bModify = true;
	if(m_pObject)
		m_pObject->Modify();
}

void BitmapDataObject::SameDib(BitmapDataObject*obj)
{
	if(IsEmptyBits()||obj->IsEmptyBits()) return;
	if(m_pChar->bits.nImgDiv==obj->m_pChar->bits.nImgDiv) return;
	if(m_pChar->bits.nImgDiv<obj->m_pChar->bits.nImgDiv)
	{
		/*int nCount=m_sbitmap.lockCount;
		m_sbitmap.PIFree();
		m_pDib->DivDib(obj->m_sbitmap.nImgDiv-m_sbitmap.nImgDiv);
		m_sbitmap.PICreate(*m_pDib,m_sbitmap.transparent,this);
		m_sbitmap.lockCount=nCount;
		m_sbitmap.nImgDiv=obj->m_sbitmap.nImgDiv;*/
		DivImage(obj->m_pChar->bits.nImgDiv);
	}
	else
	{
		/*obj->m_sbitmap.PIFree();
		int nCount=obj->m_sbitmap.lockCount;
		obj->m_pDib->DivDib(m_sbitmap.nImgDiv-obj->m_sbitmap.nImgDiv);
		obj->m_sbitmap.PICreate(*obj->m_pDib,obj->m_sbitmap.transparent,obj);
		obj->m_sbitmap.lockCount=nCount;
		obj->m_sbitmap.nImgDiv=m_sbitmap.nImgDiv;*/
		obj->DivImage(m_pChar->bits.nImgDiv);
		//obj->DivImage(m_sbitmap.nImgDiv);
	}
	//if(this->m_pObject)
	//	m_pObject->Modify();

}

void BitmapDataObject::AS3_copyChannel(BitmapDataObject*src,RectangleObject*r,PointObject*d,uint32_t sc,uint32_t dc)
{
	LockBitmap(true);
	src->LockBitmap(false);
	if(!IsEmptyBits())
	{
		AS3RECTANGLEDATA&data=r->GetData();		
		XRect rect(data.left,data.top,data.left+data.width,data.top+data.height);
		AS3POINTDATA&p=d->GetData();
		XPoint dest(p.x,p.y);
		SameDib(src);
		int nDiv = m_pChar->bits.nImgDiv;
		if(nDiv)
		{
			DivPoint(dest,nDiv);
			DivRect(rect,nDiv);
		}
		XDrawDib dib,dib1;
		dib.CreateFrom(&m_pChar->bits,true);
		dib1.CreateFrom(&src->m_pChar->bits,true);
		bool bAlpha=dib.CopyChannel(dib1,
							 rect,
							 dest,
							 (XU8)sc,
							 (XU8)dc);
		if(bAlpha)
			dib.preAlpha();
		m_bModify = true;
	}
}
void BitmapDataObject::AS3_copyPixels(BitmapDataObject*src,
									 RectangleObject*r,
									 PointObject*d,
									 BitmapDataObject*a,
									 PointObject*ap,bool mea)
{
	//StUTF8String ss(src->vtable->traits->name());
	//if(strcmp(ss.c_str(),"RoleBmd")==0)
	//	int v=0;
	//return;
	//mea=false;
	//return;
	
	LockBitmap(true);
	src->LockBitmap(false);
	if(a)
		a->LockBitmap(false);
	if(IsEmptyBits()) return;

	//if(a)
	//{
	//	src->ExpandDummy();
	//}
	
		//return;
//		int v=0;
	//if(a)
	//	return;
	if(m_pObject)
		m_pObject->Modify();
//	if(!src->IsEmptyBits())
//	{		
//		//return;
//			//int v=0;
//		//mea=false;
//		if(!src->m_pChar) return;
//		SCharacter*pChar=(SCharacter*)src->m_pChar;
//		((ShellCore*)core())->GetPlayer()->CreateIfEmpty(pChar);
//		Stringp url=(Stringp)pChar->classEntry;
//		if((pChar->type==bitsChar||pChar->type==loaderImageChar)&&pChar->bits.HasBits())
//		{
//			AS3RECTANGLEDATA&data=r->GetData();
//			XRect rect(data.left,data.top,data.left+data.width,data.top+data.height);
//			AS3POINTDATA&dst0=d->GetData();
//			SPOINT dst={dst0.x,dst0.y};
//			int nDiv=pChar->bits.nImgDiv;
//			if(nDiv)
//			{
//				//DivPoint(dst);
//				dst.x>>=nDiv;
//				dst.y>>=nDiv;
//				DivRect(rect,nDiv);
//				if(m_sbitmap.nImgDiv<nDiv)
//				{
//					m_pDib->DivDib(nDiv-m_sbitmap.nImgDiv);
//					/*m_sbitmap.nImgDiv=nDiv;
//					int w=(m_pDib->Width()<<m_sbitmap.nImgDiv)>>nDiv;
//					int h=(m_pDib->Height()<<m_sbitmap.nImgDiv)>>nDiv;
//					m_pDib->Create(w,h,32);*/
//				}
//			}
//			/*if(pChar->bits.nImgDiv)
//			{
//				int div=pChar->bits.nImgDiv;
//				dst.x>>div;
//				dst.y>>div;
//				rect.left>>div;
//				rect.top>>div;
//				rect.right>>div;
//				rect.bottom>>div;				
//			}*/
//#if (__CORE_VERSION__>=0x02072000)
//			if(a&&a->m_pDib)
//			{
//				m_pDib->Overlay(pChar->bits,dst.x,dst.y,rect,*a->m_pDib);
//			}
//			else if(mea)
//				m_pDib->Overlay(pChar->bits,dst.x,dst.y,rect);
//			else
//				m_pDib->Copy(pChar->bits,dst.x,dst.y,rect);
//#else
//			if(mea)
//				m_pDib->Overlay(pChar->bits,dst.x,dst.y,rect);
//			else
//				m_pDib->Copy(pChar->bits,dst.x,dst.y,rect);
//#endif
//		}
//		return;
//	}
	//if(m_pDib->m_pDib&&src->m_pDib)
	if(!src->IsEmptyBits())
	{
		m_bModify = true;
		AS3RECTANGLEDATA&data=r->GetData();
		XRect rect(data.left,data.top,data.left+data.width,data.top+data.height);
		AS3POINTDATA&dst0=d->GetData();
		SPOINT dst={dst0.x,dst0.y};
		SameDib(src);
		int nDiv=m_pChar->bits.nImgDiv;//src->m_sbitmap.nImgDiv;
		if(nDiv)
		{
			dst.x>>=nDiv;
			dst.y>>=nDiv;
			DivRect(rect,nDiv);
			/*if(a&&a->m_pChar)
			{
				if(nDiv>a->m_pChar->bits.nImgDiv)
					a->DivImage(nDiv);
			}*/
		}
		XDrawDib dib,dib1;
		dib.CreateFrom(&m_pChar->bits,true);
		dib1.CreateFrom(&src->m_pChar->bits,true);
		if(a&&!a->IsEmptyBits())
		{
			XDrawDib dib2;
			dib2.CreateFrom(&a->m_pChar->bits,true);
			if(nDiv>a->m_pChar->bits.nImgDiv)
				a->DivImage(nDiv);
			if(ap)
			{

				AS3POINTDATA&adst=ap->GetData();
				dib.Copy(dib1,
							  dst.x,dst.y,
							  rect,
							  dib2,
							  adst.x,adst.y);
			}
			else
			{
				dib.Copy(dib1,
							  dst.x,dst.y,
							  rect,
							  dib2,
							  0,0);
			}
		}
		else
		{
			if(mea)
			{
				//return;
				//if(src->m_pDib->m_b
				dib.Overlay(dib1,dst.x,dst.y,rect);
			}
			else
			{
				//return;
				dib.Copy(dib1,
						 dst.x,
						 dst.y,
						 rect);
			}
		}
	}
	//if(m_pDib->Width()== 202 && m_pDib->Height() == 24)
	//{
	//	m_pDib->SaveFile("D:\\dst1.bmp");
	//	src->m_pDib->SaveFile("D:\\src2.bmp");
	//}
	return;
}

void BitmapDataObject::AS3_dispose()
{
	if(IsRunning())
	{
		ShellToplevel*top=(ShellToplevel*)toplevel();
		top->RemoveBitmap(this);
	}
	if(m_pChar)
	{
		ReleaseChar(m_pChar);
	}
	m_pChar = NULL;
}

void BitmapDataObject::Draw(BitmapDataObject*pDst,MatrixObject*pMatrix,ColorTransformObject*pTransform,Stringp blendMode,RectangleObject*pClip,bool sm)
{
	LockBitmap(false);
	//pDst->LockBitmap(true);
	if(!IsEmptyBits()&&!pDst->IsEmptyBits())
	{
		//if(m_pChar->bits.width==320)
		//	return;
		//SameDib(pDst);
		//if(m_nTagID==21)
		//	int v=0;
		MATRIX mat,*pMat=NULL;
		if(pMatrix)
		{
			pMatrix->GetMatrix(&mat,false);
			pMat=&mat;
			//if(pMat->a<0)
			//	int v=0;
		}
		//else
		//	::MatrixIdentity(&mat);
		XCXForm form,*pForm=NULL;
		if(pTransform)
		{
			pTransform->GetCXForm(form);
			pForm=&form;
		}
		//else
		//	form.Clear();
		//AS3RECTANGLEDATA&data=clip->GetData();
		XRect rc;//(data.left,data.top,data.left+data.width,data.top+data.height);
		if(pClip)
		{
			AS3RECTANGLEDATA&data=pClip->GetData();
			rc.left=data.left;
			rc.top=data.top;
			rc.right=(data.left+data.width);
			rc.bottom=(data.top+data.height);
		}
		else
		{
			int nDiv=pDst->m_pChar->bits.nImgDiv;
			rc.left=0;
			rc.right=pDst->m_pChar->bits.width;//m_pDib->Width();
			rc.top=0;
			rc.bottom=pDst->m_pChar->bits.height;//>m_pDib->Height();
			rc.right<<=nDiv;
			rc.bottom<<=nDiv;
		}
		if(m_pChar->bits.nImgDiv!=pDst->m_pChar->bits.nImgDiv)//m_sbitmap.nImgDiv!=pDst->m_sbitmap.nImgDiv)
		{
			int nDiv=m_pChar->bits.nImgDiv-pDst->m_pChar->bits.nImgDiv;//m_sbitmap.nImgDiv-pDst->m_sbitmap.nImgDiv;
			if(pMat==NULL)
			{
				::MatrixIdentity(&mat);
				pMat=&mat;
				if(nDiv>0)
				{
					mat.a<<=nDiv;
					mat.d<<=nDiv;
				}
				else
				{
					mat.a>>=nDiv;
					mat.d>>=nDiv;
				}
			}
			else
			{
				MATRIX mat0;
				::MatrixIdentity(&mat0);
				mat0.a<<=m_pChar->bits.nImgDiv;//m_sbitmap.nImgDiv;
				mat0.d<<=m_pChar->bits.nImgDiv;//m_sbitmap.nImgDiv;
				mat0.a>>=pDst->m_pChar->bits.nImgDiv;//pDst->m_sbitmap.nImgDiv;
				mat0.d>>=pDst->m_pChar->bits.nImgDiv;//pDst->m_sbitmap.nImgDiv;
				::MatrixConcat(&mat0,pMat,pMat);
				//::MatrixIdentity(&mat0);
				
			}
		}
		else if(pMat&&m_pChar->bits.nImgDiv)//m_sbitmap.nImgDiv)
		{
			pMat->tx>>=m_pChar->bits.nImgDiv;//m_sbitmap.nImgDiv;
			pMat->ty>>=m_pChar->bits.nImgDiv;//m_sbitmap.nImgDiv;
		}
		if(pDst->m_pChar->bits.nImgDiv)//m_sbitmap.nImgDiv)
		{
			int nDiv=pDst->m_pChar->bits.nImgDiv;//m_sbitmap.nImgDiv;
			rc.left>>=nDiv;
			rc.top>>=nDiv;
			rc.right>>=nDiv;
			rc.bottom>>=nDiv;
		}
		XDrawDib dib,dib1;
		dib.CreateFrom(&pDst->m_pChar->bits,true);
		dib1.CreateFrom(&m_pChar->bits,true);
		dib.Draw(dib1,pMat,pForm,rc,sm);
		m_bModify = true;
	}
}

void BitmapDataObject::AS3_draw(Atom s,MatrixObject*m,ColorTransformObject* t,Stringp b,RectangleObject*r,bool sm)
{
	if(b!=NULL)
	{
		ShellToplevel*top=(ShellToplevel*)toplevel();
		BlendModeClass*pClass=top->getBlendModeClass();
		switch(pClass->getModeIDOf(b))
		{
			case drawBlendAdd://	 = 8,
				 break;
			case drawBlendNormal0:// = 0,
			case drawBlendNormal1:// = 1,
			case drawBlendLayer://	 = 2,
			case drawBlendLighten:// = 5,
			//case drawBlendDarken://	 = 6,
			case drawBlendDefrence://= 7,
			
			//case drawBlendSubtrace://= 9,
			//case drawBlendInvert://	 =10,
			//case drawBlendAlpha://	 =11,
				//drawBlendErease  =12,
			case drawBlendScreen:
			case drawBlendHardLight://=14
				break;
			case drawBlendOverlay:// =13,
			case drawBlendInvert://	 =10,
				break;
			default:
				return;
				//drawBlendMulty	 = 3,
				//drawBlendScreen	 = 4,
				
		}
	}
//	pClass->getMo
	LockBitmap(true);
	if(!IsEmptyBits())
	{
		//if(m_pChar->bits.width==85&&m_pChar->bits.height==85)
		//{
		//	int v=m_pChar->bits.width;
		//	/XSWFPlayer::m_pInstance->Trace(XString16("Draw Big Image"));
		//	//return;
		//}
		DisplayObjectObject*pDraw=(DisplayObjectObject*)core()->atomToScriptObject(s);
		ShellToplevel*top=(ShellToplevel*)toplevel();
		//if(ShellCore::IsProtoTypeOf(top->getDisplayObjectClass(),pDraw))
		if(ShellCore::IsTypeClass(pDraw,top->getBitmapClass()))
		{
			BitmapObject*pBitmap=(BitmapObject*)pDraw;
			if(!pBitmap->m_pBitmapData) return;
			pBitmap->m_pBitmapData->Draw(this,m,t,b,r,sm);
			//pBitmap->Draw(this,m,t,b,r,sm);
		}
		else if(ShellCore::IsTypeClass(pDraw,top->getDisplayObjectClass()))
			pDraw->Draw(this,m,t,b,r,sm);
		//else if(ShellCore::IsProtoTypeOf(top->getBitmapDataClass(),pDraw))
		else if(ShellCore::IsTypeClass(pDraw,top->getBitmapDataClass()))
		{
			BitmapDataObject* pBitmap=(BitmapDataObject*)pDraw;
			//pBitmap->ExpandDummy(XTRUE);
			pBitmap->Draw(this,m,t,b,r,sm);
		}
		else
			top->throwArgumentError(kInvalidArgumentError,core()->kEmptyString);
		if(m_pObject)
		{
			m_pObject->Modify();
		}
		m_bModify = true;
	}
	
}

void BitmapDataObject::Encode(XU8Array&output)
{
	LockBitmap(false);
	if(IsEmptyBits()) return;
	//BYTE*pData=XNULL;
	//if(m_pChar->bits.transparent&&m_pChar->bits.preAlpha)
	//{
	//	RGB8*pRgb=(RGB8*)m_pChar->bits.baseAddr;//m_pDib->m_pDib;
	//	int t = m_pChar->bits.width*m_pChar->bits.height;//m_pDib->Width()*m_pDib->Height();
	//	int i;
	//	for(i=0;i<t;i++)
	//	{
	//		switch(pRgb->alpha>0&&pRgb->alpha<0xff)
	//		{
	//			pRgb->blue = (pRgb->blue<<8)/pRgb->alpha;
	//			pRgb->green = (pRgb->green<<8)/pRgb->alpha;
	//			pRgb->red = (pRgb->red<<8)/pRgb->alpha;
	//		}
	//		pRgb++;
	//	}
	//	//pData=new XU8[m_pDib->m_nDataLength];
	//}

	XPngEncode encode;
	XDrawDib dib;
	dib.CreateFrom(&m_pChar->bits,true);
	encode.Encode(dib,output);
	/*{
		XFile file;
		if(file.Open("D://test.png",XFile::XCREATE|XFile::XWRITE|XFile::XBINARY))
		{
			file.Write(output.GetData(),output.GetSize());
			file.Close();
		}
	}*/

	//if(m_bTransparent)
	//{
	//	RGB8*pRgb=(RGB8*)m_pDib->m_pDib;
	//	int t = m_pDib->Width()*m_pDib->Height();
	//	int i;
	//	for(i=0;i<t;i++)
	//	{
	//		switch(pRgb->alpha>0&&pRgb->alpha<0xff)
	//		{
	//			pRgb->blue = (pRgb->blue*pRgb->alpha)>>8;
	//			pRgb->green = (pRgb->green*pRgb->alpha)>>8;
	//			pRgb->red = (pRgb->red*pRgb->alpha)>>8;
	//		}
	//		pRgb++;
	//	}
	//	//pData=new XU8[m_pDib->m_nDataLength];
	//}
}

void BitmapDataObject::AS3_fillRect(RectangleObject*r,uint32_t c)
{
	LockBitmap(true);
	//ExpandDummy(XTRUE);
	if(!IsEmptyBits())
	{
		AS3RECTANGLEDATA&data=r->GetData();
		XRect rc(data.left,data.top,data.left+data.width,data.top+data.height);
		if(rc.IsRectEmpty())
			//return;
			toplevel()->throwArgumentError(kInvalidArgumentError,core()->kEmptyString);
		if(!m_pChar->bits.transparent)
			c|=0xff000000;
#ifdef _ARGB_PIXEL_
		SWAPPIXELRB(c);
#endif
		if(m_pChar->bits.nImgDiv)//m_sbitmap.nImgDiv)
			this->DivRect(rc,m_pChar->bits.nImgDiv);
		XDrawDib dib;
		dib.CreateFrom(&m_pChar->bits,true);
		dib.FillRect(rc,c,XFALSE);
		m_bModify = true;
	}
}
void BitmapDataObject::AS3_floodFill(int x,int y,uint32_t c)
{
	LockBitmap(true);
	if(!IsEmptyBits())
	{
		if(!m_pChar->bits.transparent)//m_bTransparent)
			c|=0xff000000;
#ifndef _ARGB_PIXEL_
		SWAPPIXELRB(c);
#endif
		if(m_pChar->bits.nImgDiv)
		{
			x>>=m_pChar->bits.nImgDiv;
			y>>=m_pChar->bits.nImgDiv;
		}
		XDrawDib dib;
		dib.CreateFrom(&m_pChar->bits,true);
		dib.FloodFill(x,y,c);
		m_bModify = true;
	}
}
RectangleObject* BitmapDataObject::AS3_generateFilterRect(RectangleObject*r,BitmapFilterObject*)
{
	return r;
}
RectangleObject* BitmapDataObject::AS3_getColorBoundsRect(uint32_t mask,uint32_t color,bool bFind)
{
	LockBitmap(false);	
	RectangleClass*pClass=((ShellToplevel*)toplevel())->getRectangleClass();
	RectangleObject*pRect=pClass->CreateRectangle(0,0,0,0);
	XRect rect;
	
	if(!IsEmptyBits())
	{
		m_pChar->bits.GetColorBounds(rect,mask,color,bFind);
	}
	int nDiv = m_pChar->bits.nImgDiv;
	if(nDiv)
	{
		rect.left<<=nDiv;
		rect.right<<=nDiv;
		rect.top<<=nDiv;
		rect.bottom<<=nDiv;
	}
	pRect->constructor(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top);

	return pRect;
}
uint32_t BitmapDataObject::AS3_getPixel(int x,int y)
{
	LockBitmap(false);
	if(IsEmptyBits()) return 0;
	if(m_pChar->bits.nImgDiv)
	{
		x>>=m_pChar->bits.nImgDiv;
		y>>=m_pChar->bits.nImgDiv;
	}
	if(x<0||y<0||x>=m_pChar->bits.width||y>=m_pChar->bits.height)
		return 0;
	RGB8*pRgb=(RGB8*)m_pChar->bits.baseAddr;
	pRgb+=m_pChar->bits.width*y+x;
	//ch->bits.GetRGBPixel8(x,y,&c);
	pRgb->alpha = 0;
	//c.alpha=0;
	XU32 cc=*((XU32*)pRgb);
#ifndef _ARGB_PIXEL_
	SWAPPIXELRB(cc);
#endif
	return cc;

}
uint32_t BitmapDataObject::AS3_getPixel32(int x,int y)
{
	LockBitmap(false);
	if(IsEmptyBits()) return 0;
	if(m_pChar->bits.nImgDiv)
	{
		x>>=m_pChar->bits.nImgDiv;
		y>>=m_pChar->bits.nImgDiv;
	}
	if(x<0||y<0||x>=m_pChar->bits.width||y>=m_pChar->bits.height)
		return 0;
	RGB8*pRgb=(RGB8*)m_pChar->bits.baseAddr;
	pRgb+=m_pChar->bits.width*y+x;
	//ch->bits.GetRGBPixel8(x,y,&c);
	if(!pRgb->alpha) return 0;
	XU32 cc=*((XU32*)pRgb);
#ifndef _ARGB_PIXEL_
	SWAPPIXELRB(cc);
#endif
	return cc;
}

void BitmapDataObject::AS3_setPixels(RectangleObject*r,ByteArrayObject*cs)
{
	LockBitmap(true);
	if(IsEmptyBits()||r==NULL||cs==NULL)
		   toplevel()->throwTypeError(kInvalidArgumentError,core()->knull);
	{
		m_bModify = true;
		//int l=cs->get_length();
		//Atom*pAtom=cs->GetAtoms();
		AS3RECTANGLEDATA&data=r->GetData();
		XRect rect(data.left,data.top,data.left+data.width,data.top+data.height);		
		XU32*pData=(XU32*)cs->GetByteArray().GetBuffer();
		int nDiv = m_pChar->bits.nImgDiv;
		int wb=m_pChar->bits.width,hb=m_pChar->bits.height;//
		if(!nDiv)
		{
			int x,y,l=(rect.right-rect.left);//,i;		
			if(cs->GetByteArray().GetLength()<l*rect.Height()*4)
				toplevel()->throwError(kOutOfRangeError);
			int top=rect.top;
			int left=rect.left;
			rect&=XRect(0,0,wb,hb);//m_pChar->bits.width,m_pChar->bits.height);//m_pDib->Width(),m_pDib->Height());
			if(rect.top!=top)
			{
				pData+=(rect.top-top)*l;
			}
			if(rect.left!=left)
			{
				pData+=(rect.left-left);
			}
			XU32*pColor=(XU32*)m_pChar->bits.baseAddr;//m_pDib->GetBuffer();
			pColor+=rect.top*wb;//m_pDib->Width();
			for(y=rect.top;y<rect.bottom;y++)
			{
				XU32*pSrt=(XU32*)pData;
				for(x=rect.left;x<rect.right;x++)
				{
					XU8*srt=(XU8*)pSrt;
					XU8*dst=(XU8*)(pColor+x);
#ifndef _ARGB_PIXEL_
					dst[2]=srt[3];
					dst[1]=srt[0];
					dst[0]=srt[1];
					dst[3]=srt[2];
#else
					dst[0]=srt[3];
					dst[1]=srt[2];
					dst[2]=srt[1];
					dst[3]=srt[0];
#endif
					switch(dst[3])
					{
					case 0xff:break;
					case 0:
						//dst[0]=dst[1]=dst[2]=0;
						//break;
						break;
					default:
						dst[0]=(dst[0]*dst[3])>>8;
						dst[1]=(dst[1]*dst[3])>>8;
						dst[2]=(dst[2]*dst[3])>>8;
						break;
					}
					pSrt++;
				}
				pData+=l;
				pColor+=wb;
			}
		}
		else
		{
			int x,y,l=(rect.right-rect.left),nSkip=m_pChar->bits.nImgDiv;//,i;		
			if(cs->GetByteArray().GetLength()<l*rect.Height()*4)
				toplevel()->throwError(kOutOfRangeError);
			DivRect(rect,nSkip);
			int top=rect.top;
			int left=rect.left,nSet=1<<nSkip;
			rect&=XRect(0,0,wb,hb);//m_pDib->Width(),m_pDib->Height());
			if(rect.top!=top)
			{
				pData+=((rect.top-top)*l)<<nSkip;
			}
			if(rect.left!=left)
			{
				pData+=(rect.left-left)<<nSkip;
			}
			XU32*pColor=(XU32*)m_pChar->bits.baseAddr;//m_pDib->GetBuffer();
			pColor+=rect.top*wb;
			for(y=rect.top;y<rect.bottom;y++)
			{
				XU32*pSrt=(XU32*)pData;
				for(x=rect.left;x<rect.right;x++)
				{
					XU8*srt=(XU8*)pSrt;
					XU8*dst=(XU8*)(pColor+x);
#ifndef _ARGB_PIXEL_
					dst[2]=srt[3];
					dst[1]=srt[0];
					dst[0]=srt[1];
					dst[3]=srt[2];
#else
					dst[0]=srt[3];
					dst[1]=srt[2];
					dst[2]=srt[1];
					dst[3]=srt[0];
#endif
					pSrt+=nSet;
				}
				pData+=l<<nSkip;
				pColor+=wb;
			}
		}
	}
}

ByteArrayObject* BitmapDataObject::AS3_getPixels(RectangleObject*r)
{
	LockBitmap(false);
	ByteArrayClass*pClass=((ShellToplevel*)toplevel())->getByteArrayClass();
	if(IsEmptyBits())
		return pClass->CreateArray(0);

	AS3RECTANGLEDATA&data=r->GetData();
	XRect rect(data.left,data.top,data.left+data.width,data.top+data.height);
	int x,y,l=(rect.right-rect.left);//,i;	
	int wi=m_pChar->bits.width,hi=m_pChar->bits.height;//
	int nSize=wi*hi;//m_pChar->bits.width*m_pChar->bits.height;//rect.Width()*rect.Height()*sizeof(XU32);
	ByteArrayObject*pArray=pClass->CreateArray(nSize);
	if(pArray)
	{
		pArray->SetLength(l*rect.Height()*4);

		if(!m_pChar->bits.nImgDiv)//m_sbitmap.nImgDiv)
		{
			int top=rect.top;
			int left=rect.left;
			rect&=XRect(0,0,wi,hi);//m_pChar->bits.width,m_pChar->bits.height);//m_pDib->Width(),m_pDib->Height());
			XU32*pData=(XU32*)pArray->GetByteArray().GetBuffer();
			if(top!=rect.top)
			{
				pData+=(rect.top-top)*l;
			}
			if(left!=rect.left)
			{
				pData+=(rect.left-left);
			}
			XU32*pColor=(XU32*)m_pChar->bits.baseAddr;//m_pDib->GetBuffer();
			pColor+=rect.top*wi;//m_pDib->Width();
			
			
			
			for(y=rect.top;y<rect.bottom;y++)
			{
				XU32*pSrt=(XU32*)pData;
				for(x=rect.left;x<rect.right;x++)
				{
					XU8*dst=(XU8*)pSrt;
					XU8*srt=(XU8*)(pColor+x);
#ifndef _ARGB_PIXEL_
					dst[0]=srt[3];
					dst[1]=srt[0];
					dst[2]=srt[1];
					dst[3]=srt[2];
#else
					dst[0]=srt[3];
					dst[1]=srt[2];
					dst[2]=srt[1];
					dst[3]=srt[0];
#endif
					pSrt++;

				}
				pData+=l;
				pColor+=wi;//m_pDib->Width();
			}	
		}
		else
		{
			int nDiv=m_pChar->bits.nImgDiv;
			int i,nSkip=(1<<nDiv)-1;
			this->DivRect(rect,nDiv);
			int top=rect.top;
			int left=rect.left;
			XU32*pData=(XU32*)pArray->GetByteArray().GetBuffer();
			rect&=XRect(0,0,wi,hi);
			if(top!=rect.top)
			{
				pData+=((rect.top-top)*l)<<nDiv;
			}
			if(left!=rect.left)
			{
				pData+=(rect.left-left)<<nDiv;
			}

			rect&=XRect(0,0,wi,hi);
			XU32*pColor=(XU32*)m_pChar->bits.baseAddr;
			pColor+=rect.top*wi;
			int lc=(rect.right-rect.left)<<(nDiv+2);
			
			for(y=rect.top;y<rect.bottom;y++)
			{
				XU32*pSrt=(XU32*)pData;
				for(x=rect.left;x<rect.right;x++)
				{
					XU8*dst=(XU8*)pSrt;
					XU8*srt=(XU8*)(pColor+x);
#ifndef _ARGB_PIXEL_
					dst[0]=srt[3];
					dst[1]=srt[0];
					dst[2]=srt[1];
					dst[3]=srt[2];
#else
					dst[0]=srt[3];
					dst[1]=srt[2];
					dst[2]=srt[1];
					dst[3]=srt[0];
#endif
					XU32 c=*pSrt;
					pSrt++;
					for(i=0;i<nSkip;i++)					
						pSrt[i]=c;
					pSrt+=nSkip;

				}
				pSrt=pData;
				pData+=l;
				for(i=0;i<nSkip;i++)
				{
					memcpy(pData,pSrt,lc);
					pData+=l;
				}
				pColor+=wi;
			}			
		}
	}
	return pArray;
}

bool BitmapDataObject::HitTest(XPoint&pt,uint32_t alpha,BitmapDataObject*pBmp,PointObject*second,uint32_t alpha2)
{
	LockBitmap(false);
	//ExpandDummy(XFALSE);
	if(!pBmp||IsEmptyBits())//!m_pDib)
		return false;

	pBmp->LockBitmap(false);
	//pBmp->ExpandDummy(XFALSE);
	if(pBmp->IsEmptyBits()) return false;
	//if(!pBmp->m_bTransparent) {alpha2=alpha=0;}
	if(!pBmp->IsTransparent()) {alpha2=alpha=0;}
	XPoint spt(0,0);
	int nDiv=m_pChar->bits.nImgDiv;
	if(second)
	{
		AS3POINTDATA&data=second->GetData();
		spt.x=data.x;
		spt.y=data.y;
		if(nDiv)
		{
			spt.x>>=nDiv;
			spt.y>>=nDiv;
		}
	}
	int w=m_pChar->bits.width;
	int h=m_pChar->bits.height;
	int w1=pBmp->m_pChar->bits.width;
	int h1=pBmp->m_pChar->bits.height;
	XRect r1(pt.x,pt.y,pt.x+w,pt.y+h);
	XRect r2(spt.x,spt.y,spt.x+w1,spt.y+h1);
	XRect rr=r1;
	rr&=r2;
	if(!rr.IsRectEmpty())
	{
		if(!alpha||!alpha2) return true;//var.iData32=XTRUE;
		else
		{
			XDrawDib dib1;
			dib1.CreateFrom(&pBmp->m_pChar->bits,true);
			if(!alpha)
			{
				rr-=r2;
				return dib1.HitTest(rr,alpha2)!=0;
			}
			else if(!alpha2)
			{
				rr-=r2;
				return dib1.HitTest(rr,alpha)!=0;
			}
			else
			{
				XU32*ps1=(XU32*)m_pChar->bits.baseAddr;//m_pDib->GetBuffer();
				XU32*ps2=(XU32*)pBmp->m_pChar->bits.baseAddr;//m_pDib->GetBuffer();
				ps1+=(rr.top-r1.top)*w+(rr.left-r1.left);
				ps2+=(rr.top-r2.top)*w1+(rr.left-r2.left);
				int x,y,dw=rr.Width();
				//if(dw>60)
				//	int v=0;
				for(y=rr.top;y<rr.bottom;y++)
				{
					for(x=0;x<dw;x++)														 
					{
						if(((XU8*)&ps1[x])[3]<alpha) continue;
						if(((XU8*)&ps2[x])[3]<alpha2) continue;
						return true;
						//var.iData32=XTRUE;
						//cnt.pStack->Push(var);
						//return;
					}
					ps1+=w;
					ps2+=w1;
				}
				return false;
			}
		}
	}
	return false;
}

bool BitmapDataObject::AS3_hitTest(PointObject* first,uint32_t ft,Atom seo,PointObject* secend,uint32_t st)
{
	//return false;
	//return true;
	LockBitmap(false);
	//ExpandDummy(XFALSE);
	if(IsEmptyBits()||first==NULL)
		   toplevel()->throwTypeError(kInvalidArgumentError,core()->knull);
	//if(m_pDib)
	
	{
		//if(m_pDib->Width()==1440)
		//	int v=0;
		//ShellCore*c=(ShellCore*)core();
		ScriptObject*pSecond=core()->atomToScriptObject(seo);
		ShellToplevel*t=(ShellToplevel*)toplevel();
		PointClass*ppClass=t->getPointClass();
		RectangleClass*prClass=t->getRectangleClass();
		BitmapClass*pbClass=t->getBitmapClass();
		BitmapDataClass*pbdClass=t->getBitmapDataClass();
		AS3POINTDATA&ptt=first->GetData();
		XPoint pt(ptt.x,ptt.y);
		int nDiv = m_pChar->bits.nImgDiv;
		if(nDiv)
		{
			pt.x>>=nDiv;
			pt.y>>=nDiv;
		}
		//if(ShellCore::IsProtoTypeOf(ppClass,pSecond))
		if(ShellCore::IsTypeClass(pSecond,ppClass))
		{//Point
			AS3POINTDATA&data=((PointObject*)pSecond)->GetData();
			XPoint dpt(data.x-pt.x,data.y-pt.y);
			int w=m_pChar->bits.width,h=m_pChar->bits.height;//m_pDib->Width(),h=m_pDib->Height();
			XRect rect(0,0,w-1,h-1);
			if(rect.PtInRect(dpt))
			{
				//if(!ft)
				//	return true;
					//var.iData32=XTRUE;
				//else
				{//[Note X]
					XU32*pBuf=(XU32*)m_pChar->bits.baseAddr;//m_pDib->GetBuffer();
					pBuf+=dpt.y*w+dpt.x;
					if(((XU8*)pBuf)[3]>ft)
						return true;
					else 
						return false;
						//var.iData32=XTRUE;
				}
					//disp->ToPixel(rect);
					//var.iData32=m_pDib->HitTest(rect,alpha);
			}
		}
		//else if(ShellCore::IsProtoTypeOf(prClass,pSecond))
		else if(ShellCore::IsTypeClass(pSecond,prClass))
		{
			AS3RECTANGLEDATA&data=((RectangleObject*)pSecond)->GetData();
			XRect rect(data.left,data.top,data.left+data.width,data.top+data.height);
			rect.OffsetRect(-pt.x,-pt.y);
			int w=m_pChar->bits.width;//m_pDib->Width();
			int h=m_pChar->bits.height;//m_pDib->Height();
			XRect rt(0,0,w,h);
			if(!rect.IsRectEmpty())
			{
				XDrawDib dib;
				dib.CreateFrom(&m_pChar->bits,true);
				return dib.HitTest(rt,ft)!=0;
			}

		}
		//else if(ShellCore::IsProtoTypeOf(pbClass,pSecond))
		else if(ShellCore::IsTypeClass(pSecond,pbClass))
		{
			//XXObjectBitmap*pBmp=(XXObjectBitmap*)list[2].pObject;
			//if(pBmp->m_pDib==XNULL) break;
			//if(!pBmp->m_bTransparent) alpha=0;
			BitmapDataObject*pBmp=((BitmapObject*)pSecond)->m_pBitmapData;
			if(!pBmp||!pBmp->m_pChar) return false;
			//XPoint spt(0,0);			
			return HitTest(pt,ft,pBmp,secend,st);
			
		}
		//else if(ShellCore::IsProtoTypeOf(pbdClass,pSecond))
		else if(ShellCore::IsTypeClass(pSecond,pbdClass))
		{
			BitmapDataObject*pBmp=(BitmapDataObject*)pSecond;
			if(!pBmp->m_pChar) return false;
			return HitTest(pt,ft,pBmp,secend,st);
		}
		else
			toplevel()->throwArgumentError(kInvalidArgumentError,core()->kEmptyString);
		//pSecond->
	}
	return false;
}
void BitmapDataObject::AS3_lock()
{
	//LockBitmap();
	//if(m_pDib)
	//	m_pDib->Lock();
}
void BitmapDataObject::AS3_merge(BitmapDataObject*pBmp,
								 RectangleObject* r,
								 PointObject*d,
								 uint32_t rm,
								 uint32_t gm,
								 uint32_t bm,
								 uint32_t am)
{
	LockBitmap(true);
	//ExpandDummy(XTRUE);
	//if(m_pDib==NULL||r==NULL||d==NULL||pBmp==NULL)
	if(IsEmptyBits()||r==NULL||d==NULL||pBmp==NULL)
		   toplevel()->throwTypeError(kInvalidArgumentError,core()->knull);
	pBmp->LockBitmap(false);
	//pBmp->ExpandDummy(XFALSE);
	if(!pBmp->IsEmptyBits())//m_pDib&&pBmp->m_pDib)
	{
		AS3POINTDATA&pdata=d->GetData();
		AS3RECTANGLEDATA&data=r->GetData();
		XRect rect(data.left,data.top,data.left+data.width,data.top+data.height);
		XPoint pt(pdata.x,pdata.y);
		SameDib(pBmp);
		int nDiv=m_pChar->bits.nImgDiv;
		if(nDiv)
		{
			DivPoint(pt,nDiv);
			DivRect(rect,nDiv);
		}
#ifndef _ARGB_PIXEL_
		uint32_t tmp=rm;
		rm=bm;
		bm=rm;
#endif
		XDrawDib dib,dib1;
		dib.CreateFrom(&m_pChar->bits,true);
		dib1.CreateFrom(&pBmp->m_pChar->bits,true);
		dib.Mix(dib1,rect,pt,rm,gm,bm,am);
		m_bModify = true;
	}

}
void BitmapDataObject::AS3_noise(int nSeed,uint32_t nLow,uint32_t nHigh,uint32_t nOption,bool nGray)
{
	LockBitmap(true);
	//ExpandDummy(XTRUE);
	if(!IsEmptyBits())//m_pDib)
	{
		if(!(nOption&0xf)) return;
		XGlobal::Seed(nSeed);
		XDrawDib dib;
		dib.CreateFrom(&m_pChar->bits,true);
		dib.Noise(nLow,nHigh,nOption,nGray);
		m_bModify = true;
	}
}
void BitmapDataObject::AS3_paletteMap(BitmapDataObject*pBmp,
									  RectangleObject* r,
									  PointObject*d,
									  ArrayObject*ra,
									  ArrayObject*ga,
									  ArrayObject*ba,
									  ArrayObject*aa)
{
	LockBitmap(true);
	//ExpandDummy(XTRUE);
//#if (__CORE_VERSION__>=0x02081000)
//	if(m_pDib==NULL||r==NULL||d==NULL || pBmp == NULL )
//#else
//	if(m_pDib==NULL||r==NULL||d==NULL||pBmp)
//#endif
	if(IsEmptyBits()||r==NULL||d==NULL||pBmp==NULL)
		   toplevel()->throwTypeError(kInvalidArgumentError,core()->knull);
	//pBmp->ExpandDummy(XFALSE);
	//pBmp->LockBitmap();
	pBmp->LockBitmap(false);
#ifndef _ARGB_PIXEL_
	ArrayObject*tmp=ra;
	ra=ba;
	ba=tmp;
#endif
	if(pBmp->IsEmptyBits())//!pBmp->m_pDib)
		toplevel()->throwTypeError(kInvalidArgumentError,core()->knull);
	//if(m_pDib)
	{
		AS3POINTDATA&pdata=d->GetData();
		AS3RECTANGLEDATA&data=r->GetData();
		XRect rect(data.left,data.top,data.left+data.width,data.top+data.height);
		XPoint dst(pdata.x,pdata.y);
		SameDib(pBmp);
		int nDiv = m_pChar->bits.nImgDiv;
		if(nDiv)
		{
			DivPoint(dst,nDiv);
			DivRect(rect,nDiv);
		}
		XU8Array reds,greens,blues,alphas;
		//reds
		int i,l;
		XU8*pData;
		Atom*pAtom;
		if(ra)
		{
			l=ra->getDenseLength();//get_length();
			reds.SetSize(l);
			XU8*pData=reds.GetData();
			Atom*pAtom=ra->GetAtoms();
			for(i=0;i<l;i++)
				pData[i]=pAtom[i]>>3;
		}
		if(ga)
		{
			l=ga->getDenseLength();//get_length();
			greens.SetSize(l);
			pData=greens.GetData();
			pAtom=ga->GetAtoms();	
			for(i=0;i<l;i++)
				pData[i]=pAtom[i]>>3;
		}
		if(ba)
		{
			l=ba->getDenseLength();//get_length();
			blues.SetSize(l);
			pData=blues.GetData();
			pAtom=ba->GetAtoms();	
			for(i=0;i<l;i++)
				pData[i]=pAtom[i]>>3;
		}
		if(aa)
		{
			l=aa->getDenseLength();//get_length();
			alphas.SetSize(l);
			pData=alphas.GetData();
			pAtom=aa->GetAtoms();	
			for(i=0;i<l;i++)
				pData[i]=pAtom[i]>>3;
		}
		XDrawDib dib,dib1;
		dib.CreateFrom(&m_pChar->bits,true);
		dib1.CreateFrom(&pBmp->m_pChar->bits,true);
		dib.PaletteMap(dib1,rect,dst,reds,greens,blues,alphas);
		m_bModify = true;
	}
}
void BitmapDataObject::AS3_perlinNoise(double,double,uint32_t nSeed,int,bool,bool,uint32_t nOption,bool nGray,ArrayObject*)
{
	LockBitmap(true);
	//ExpandDummy(XTRUE);
	if(!IsEmptyBits())//m_pDib)
	{
		if(!(nOption&0xf)) return;
		XGlobal::Seed(nSeed);
		XDrawDib dib;
		dib.CreateFrom(&m_pChar->bits,true);
		dib.Noise(0,255,nOption,nGray);
		m_bModify = true;
	}
}
int BitmapDataObject::AS3_pixelDissolve(BitmapDataObject*pBmp,
										 RectangleObject* r,
										 PointObject*d,
										 int nSeed,int nPixels,uint32_t nColor)
{
	LockBitmap(true);
	//ExpandDummy(XTRUE);
	//if(m_pDib==NULL||r==NULL||d==NULL||nPixels<0||pBmp==NULL)
	if(IsEmptyBits()||r==NULL||d==NULL||pBmp==NULL)
		   toplevel()->throwTypeError(kInvalidArgumentError,core()->knull);
	pBmp->LockBitmap(false);
	//pBmp->ExpandDummy(XFALSE);
	if(pBmp->IsEmptyBits())//!pBmp->m_pDib)
		toplevel()->throwTypeError(kInvalidArgumentError,core()->knull);
	AS3POINTDATA&pdata=d->GetData();
	AS3RECTANGLEDATA&data=r->GetData();
	SameDib(pBmp);
#ifndef _ARGB_PIXEL_
	SWAPPIXELRB(nColor);
#endif
	XRect rect(data.left,data.top,data.left+data.width,data.top+data.height);
	XPoint dst(pdata.x,pdata.y);
	int nDiv = m_pChar->bits.nImgDiv;
	if(nDiv)//m_sbitmap.nImgDiv)
	{
		DivPoint(dst,nDiv);//m_sbitmap.nImgDiv);
		DivRect(rect,nDiv);//m_sbitmap.nImgDiv);
	}
	if(!IsTransparent())//m_bTransparent)
			nColor|=0xff000000;
	XDrawDib dib;
	dib.CreateFrom(&m_pChar->bits,true);
	if(pBmp==this||pBmp->m_pChar==m_pChar)
	{
		nSeed=dib.Dissolve(rect,dst,nSeed,nPixels,nColor);
	}
	else
	{
		XDrawDib dib1;
		dib1.CreateFrom(&pBmp->m_pChar->bits,true);
		nSeed=dib.Dissolve(dib1,rect,dst,nSeed,nPixels);
	}
	m_bModify = true;
	return nSeed;
}
void BitmapDataObject::AS3_scroll(int x,int y)
{
	LockBitmap(true);
	if(IsEmptyBits()) return;
	//ExpandDummy(XTRUE);
	int nDiv = m_pChar->bits.nImgDiv;
	if(nDiv)
	{
		x>>=nDiv;
		y>>=nDiv;
	}
	XDrawDib dib;
	dib.CreateFrom(&m_pChar->bits,true);
	//if(m_pDib)
	dib.Scroll(x,y);
	m_bModify = true;
}
void BitmapDataObject::AS3_setPixel(int x,int y,uint32_t c)
{
	LockBitmap(true);
	//ExpandDummy(XTRUE);
	if(IsEmptyBits()) return;
	int nDiv = m_pChar->bits.nImgDiv;
	if(nDiv)//m_sbitmap.nImgDiv)
	{
		x>>=nDiv;
		y>>=nDiv;
	}
#ifndef _ARGB_PIXEL_
	SWAPPIXELRB(c);
#endif
	XDrawDib dib;
	dib.CreateFrom(&m_pChar->bits,true);
	//if(m_pDib)
	dib.SetPixel(x,y,c,XFALSE);
	m_bModify = true;
}
void BitmapDataObject::AS3_setPixel32(int x,int y,uint32_t c)
{
	LockBitmap(true);
#ifndef _ARGB_PIXEL_
	SWAPPIXELRB(c);
#endif
	//if(m_pDib)
	XDrawDib dib;
	dib.CreateFrom(&m_pChar->bits,true);
	dib.SetPixel(x,y,c,XTRUE);
	m_bModify = true;
}


uint32_t BitmapDataObject::AS3_threshold(BitmapDataObject*pBmp,
										 RectangleObject*r,
										 PointObject*d,
										 Stringp opt,
										 uint32_t thre,uint32_t nColor,uint32_t mask,bool bCopy)
{
		LockBitmap(true);
		//ExpandDummy(XTRUE);
		if(IsEmptyBits()||pBmp==NULL||r==NULL||d==NULL)
			toplevel()->throwTypeError(kInvalidArgumentError,core()->knull);
		pBmp->LockBitmap(false);
		//pBmp->ExpandDummy(XFALSE);
		if(pBmp->IsEmptyBits())//!pBmp->m_pDib)
			return 0;
		//if(!m_pDib||!pBmp->m_pDib) return 0;
		AS3POINTDATA&pdata=d->GetData();
		AS3RECTANGLEDATA&data=r->GetData();
		SameDib(pBmp);
		XRect rect(data.left,data.top,data.left+data.width,data.top+data.height);
		XPoint dst(pdata.x,pdata.y);
		int nDiv = m_pChar->bits.nImgDiv;
		if(nDiv)//m_sbitmap.nImgDiv)
		{
			DivPoint(dst,nDiv);//m_sbitmap.nImgDiv);
			DivRect(rect,nDiv);//m_sbitmap.nImgDiv);
		}
		XU8 nOpt=0;
		StUTF8String str(opt);
#ifndef _ARGB_PIXEL_
		SWAPPIXELRB(nColor);
#endif
		const char* strTxt=str.c_str();
		switch(strTxt[0])
		{
			case '!':nOpt=5;break;
			case '<':nOpt=strTxt[1]=='='?1:0;break;
			case '=':nOpt=4;break;
			case '>':nOpt=strTxt[1]=='='?3:2;break;
			default:
				toplevel()->throwArgumentError(kInvalidArgumentError,opt);
		}
		XDrawDib dib,dib1;
		dib.CreateFrom(&m_pChar->bits,true);
		dib1.CreateFrom(&pBmp->m_pChar->bits,true);
		return dib.Threshold(dib1,rect,dst,nOpt,thre,mask,nColor,bCopy);

}
void BitmapDataObject::AS3_unlock(RectangleObject*)
{
}

//new adds end

UIntVectorObject* BitmapDataObject::AS3_getVector(RectangleObject *pRect)
{
	/*UIntVectorObject *pVectorObj = NULL;

	if (m_pDib)
	{
		AS3RECTANGLEDATA &data = pRect->GetData();
		XRect rect(data.left, data.top, data.left+data.width, data.top+data.height);

		//错误处理
		if (rect.IsRectEmpty())
		{
			toplevel()->throwArgumentError(kInvalidArgumentError,core()->kEmptyString);
			return NULL;
		}

		UIntVectorClass *pVectorCls = toplevel()->GetuintVectorClass();
		uint32_t vectorSize = rect.Width() * rect.Height() * sizeof(XU32);
		pVectorObj = pVectorCls->newVector(vectorSize);		//内存管理模块自动将内存初始化为0

		if (pVectorObj)
		{
			//位图数据获取
			XU32 *pColor = (XU32*)m_pDib->GetBuffer();
			pColor += rect.top * m_pDib->GetLinePitch();

			int x,y;
			int width = rect.Width();
			int index = 0;
			for (y=rect.top; y<rect.bottom; y++)
			{
				for (x = rect.left; x<rect.right; x++)
				{
					pVectorObj->_setNativeUintProperty(index++, pColor[x]);
				}
				pColor += m_pDib->GetLinePitch();
			}
		}
	}

	return pVectorObj;*/
	return NULL;
}
ObjectVectorObject* BitmapDataObject::AS3_histogram(RectangleObject *pHRect)
{
	/*AS3RECTANGLEDATA &data = pHRect->GetData();
	XRect rect(data.left, data.top, data.left+data.width, data.top+data.height);
	if (rect.IsRectEmpty())
	{
//		return NULL;
		rect.left = rect.top = 0;
		rect.right = m_pDib->m_clip.left;
		rect.bottom = m_pDib->m_clip.bottom;
	}

	ObjectVectorObject *pObjVecObj = NULL;

	if (m_pDib)
	{
		ObjectVectorClass *pObjVecCls = toplevel()->GetobjectVectorClass();
		pObjVecObj = pObjVecCls->newVector(4);
		if (pObjVecObj)
		{	//自动将分配内存清零
			DoubleVectorClass *pDoubleCls = toplevel()->GetdoubleVectorClass();
			DoubleVectorObject *pAlphaVec = pDoubleCls->newVector(256);
			DoubleVectorObject *pRedVec = pDoubleCls->newVector(256);
			DoubleVectorObject *pGreenVec = pDoubleCls->newVector(256);
			DoubleVectorObject *pBlueVec = pDoubleCls->newVector(256);

			union temp
			{
				struct tagArgb
				{
					uint8 b;
					uint8 g;
					uint8 r;
					uint8 a;
				}argb;

				XU32 color;
			}uColor;

			if (pAlphaVec && pRedVec && pGreenVec && pBlueVec)
			{

				XU32 *pColor = (XU32*)m_pDib->GetBuffer();
				pColor += rect.top*m_pDib->GetLinePitch();

				int x,y;
				uint32_t alpha, red, green , blue;
				for (y=rect.top; y<rect.bottom; y++)
				{
					for (x=rect.left; x<rect.right; x++)
					{
						uColor.color = pColor[x];

						alpha = pAlphaVec->_getNativeUintProperty(uColor.argb.a);
						pAlphaVec->_setNativeUintProperty(uColor.argb.a, ++alpha);
						
						red = pRedVec->_getNativeUintProperty(uColor.argb.r);
						pRedVec->_setNativeUintProperty(uColor.argb.r, ++red);

						green = pGreenVec->_getNativeUintProperty(uColor.argb.g);
						pGreenVec->_setNativeUintProperty(uColor.argb.g, ++green);

						blue = pBlueVec->_getNativeUintProperty(uColor.argb.b);
						pBlueVec->_setNativeUintProperty(uColor.argb.b, ++blue);
					}
				}

				pObjVecObj->_setUintProperty(0, pRedVec->atom());
				pObjVecObj->_setUintProperty(1, pGreenVec->atom());
				pObjVecObj->_setUintProperty(2, pBlueVec->atom());
				pObjVecObj->_setUintProperty(3, pAlphaVec->atom());
			}
		}
	}

	return pObjVecObj;*/
	return NULL;
}
void BitmapDataObject::AS3_setVector(RectangleObject *pRect, UIntVectorObject *pInputVector)
{
	/*if (m_pDib)
	{
		AS3RECTANGLEDATA &data = pRect->GetData();
		XRect rect(data.left, data.top, data.left+data.width, data.top+data.height);
		uint32 rectSize = rect.Width() * rect.Height();

		//错误处理
		if (pInputVector->get_length() > rectSize)
		{
			toplevel()->throwRangeError(kOutOfRangeError, core()->uintToString(pInputVector->get_length()), core()->uintToString(rectSize));
			return;
		}

		XU32 *pColor = (XU32 *)m_pDib->GetBuffer();
		pColor += rect.top * m_pDib->GetLinePitch();

		int x ,y;
		int index = 0;
		for (y=rect.top; y<rect.bottom; y++)
		{
			for (x=rect.left; x<rect.right; x++)
			{
				pColor[x] = pInputVector->_getNativeUintProperty(index++);
			}
			pColor += m_pDib->GetLinePitch();
		}
	}*/
	//return NULL;
}

//void BitmapDataObject::AS3_setVector(RectangleObject *pRect, UIntVectorObject *pInputVector)
//{
//	if (m_pDib)
//	{
//		AS3RECTANGLEDATA &data = pRect->GetData();
//		XRect rect(data.left, data.top, data.left+data.width, data.top+data.height);
//		uint32 rectSize = rect.Width() * rect.Height();
//
//		XU32 *pColor = (XU32 *)m_pDib->GetBuffer();
//		pColor += rect.top * m_pDib->GetLinePitch();
//
//		int x ,y;
//		int index = 0;
//		int base = 0;
//		int offset = rect.left;
//		int width = rect.Width();
//		int linePitch = m_pDib->GetLinePitch();
//
//		if (pInputVector->get_length() > rectSize)
//		{
//			while (index<rectSize)
//			{
//				if (offset == rect.right)
//				{
//					base += 1;
//					offset = rect.left;
//				}
//				pColor[base*linePitch + offset] = pInputVector->_getNativeUintProperty(index++);
//			}
//			toplevel()->throwRangeError(kOutOfRangeError, core()->uintToString(pInputVector->get_length()), core()->uintToString(rectSize));
//		}
//		else
//		{
//			for (y=rect.top; y<rect.bottom; y++)
//			{
//				for (x=rect.left; x<rect.right; x++)
//				{
//					pColor[x] = pInputVector->_getNativeUintProperty(index++);
//				}
//				pColor += m_pDib->GetLinePitch();
//			}
//		}
//	}
//}//void BitmapDataObject::AS3_copyPixelsToByteArray(RectangleObject* rect,ByteArrayObject* byteArray){}

//new adds 11
void BitmapDataObject::AS3_drawWithQuality(Atom source, MatrixObject* matrix, ColorTransformObject* colorTransform, Stringp blendMode, RectangleObject* clipRect, bool smoothing, Stringp quality)
{
	//LOGWHERE();
	StageQualityClass*pClass=((ShellToplevel*)toplevel())->getStageQualityClass();
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	if(quality->equals(pClass->getSlotLOW()))
		player->SetQuality(0.0F);
	else if(quality->equals(pClass->getSlotHIGH())||
		    quality->equals(pClass->getSlotBEST())||
			quality->equals(pClass->getSlotHIGH_16X16())||
			quality->equals(pClass->getSlotHIGH_16X16_LINEAR())||
			quality->equals(pClass->getSlotHIGH_8X8())||
			quality->equals(pClass->getSlotHIGH_8X8_LINEAR()))
		player->SetQuality(3.0F);
	else
		player->SetQuality(2.0F);
	AS3_draw(source, matrix, colorTransform, blendMode, clipRect, smoothing);
}
ByteArrayObject* BitmapDataObject::AS3_encode(RectangleObject* rect, ScriptObject* compressor, ByteArrayObject* byteArray)
{
	LOGWHERE();
	return NULL;
}
void BitmapDataObject::AS3_copyPixelsToByteArray(RectangleObject* r,ByteArrayObject* d)
{
	//LOGWHERE();
	if ( r && d ){
		//ByteArrayClass*pClass=((ShellToplevel*)toplevel())->getByteArrayClass();
		//ExpandDummy(XFALSE);
		AS3RECTANGLEDATA&data=r->GetData();
		XRect rect(data.left,data.top,data.left+data.width,data.top+data.height);
//		int x,y,l=(rect.right-rect.left);//,i;	
		uint32_t nSize=rect.Width()*rect.Height()*sizeof(XU32);
		uint32_t n = d->GetLength();
		if(n<nSize)
			d->SetLength(nSize);
		d->AS3_writeBytes( AS3_getPixels(r), 0, nSize );
	}
}

//new adds 11 end
}
