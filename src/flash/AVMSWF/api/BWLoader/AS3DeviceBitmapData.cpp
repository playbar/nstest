#include "StdAfxflash.h"
#include "XDrawDib.h"
#include "Raster_gl.h"
#include "ShellCore.h"
#include "sobject.h"
#include "splay.h"
#include "splayer.h"
using namespace avmplus;
#include "AS3DeviceBitmapData.h"
#include "AS3Rectangle.h"
#include "AS3Point.h"
#include "AS3Matrix.h"
#include "AS3ColorTransform.h"
#include "AS3DisplayObject.h"
#include "AS3ByteArray.h"
#include "AS3ByteArray.h"
#include "AS3Bitmap.h"
#include "AS3BlendMode.h"
#include "avm2.h"


#define MINDIVSIZE 192
#define SCALEONE 1024
namespace avmshell{
BXDeviceBitmapDataClass::BXDeviceBitmapDataClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())BXDeviceBitmapDataObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* BXDeviceBitmapDataClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) BXDeviceBitmapDataObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}



BXDeviceBitmapDataObject* BXDeviceBitmapDataClass::AS3_GetDeviceBitmap(int x,int y,int w,int h)
{
	BXDeviceBitmapDataObject*pObj=(BXDeviceBitmapDataObject*)createInstance(ivtable(),prototype);
	pObj->InitDevice(x,y,w,h);
	pObj->m_nAntiX=x<<4;
	pObj->m_nAntiY=y<<4;
	return pObj;
}

BXDeviceBitmapDataObject* BXDeviceBitmapDataClass::AS3_GetDeviceBitmapObj(DisplayObjectObject*pObj)
{
	//return XNULL;
	/*SRECT rect;
	if(pObj&&pObj->m_pObject)
	{
		MATRIX*pMatrix=XNULL;	
		pObj->m_pObject->GetBoundBox(rect,pMatrix,false);
		MATRIX mat,mat0;
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		int ax=0,ay=0;
		SRECT r1;
		pObj->m_pObject->GetTotalMatrix(mat,true);
		{
			
			::MatrixIdentity(&mat0);
			mat0.a=fixed_1<<(4-player->display.antiShift);
			mat0.d=fixed_1<<(4-player->display.antiShift);
			::MatrixConcat(&mat,&mat0,&mat0);
			::MatrixTransformRect(&mat0,&rect,&r1);
			ax=r1.xmin;
			ay=r1.ymin;
		}
		::MatrixIdentity(&mat0);
		//mat0.a=fixed_1>>player->display.antiShift;
		//mat0.d=fixed_1>>player->display.antiShift;
		//::MatrixConcat(&mat,&mat0,&mat);
		//::MatrixTransformRect(&mat,&rect,&rect);
		//m_totalMatrix=mat;

		//
		//player->display.ToPixel(rect);
		//BXDeviceBitmapDataObject*pObj=(BXDeviceBitmapDataObject*)createInstance(ivtable(),prototype);
		BXDeviceBitmapDataObject*pObject=(BXDeviceBitmapDataObject*)createInstance(ivtable(),prototype);
		rect.xmin=(r1.xmin+8)>>4;
		rect.ymin=(r1.ymin+8)>>4;
		rect.xmax=(r1.xmax+8)>>4;
		rect.ymax=(r1.ymax+8)>>4;
		//r1.xmax>>=4;
		//r1.xmin>>=4;
		//r1.ymin>>=4;
		//r1.ymax>>=4;
		
		pObject->InitDevice(rect.xmin,rect.ymin,rect.xmax-rect.xmin+1,rect.ymax-rect.ymin+1);//,y,w,h);
		rect.xmin<<=4;
		rect.ymin<<=4;
		ax-=r1.xmin-rect.xmin;
		ay-=r1.ymin-rect.ymin;
		pObject->m_nAntiX=ax;
		pObject->m_nAntiY=ay;
		
		//BXDeviceBitmapDataObject*pObject=AS3_GetDeviceBitmap(rect.xmin,rect.ymin,rect.xmax-rect.xmin,rect.ymax-rect.ymin);
		//if(pObject)
		//	pObject->m_totalMatrix=mat;
		return pObject;
	}*/
	return XNULL;
}

void BXDeviceBitmapDataObject::InitDevice(int x,int y,int w,int h)
{
	//return;
	/*if(x<0) x=0;
	if(y<0) y=0;
	if(w<=0) w=m_pRaster->bits->width();
	if(h<=0) h=m_pRaster->bits->height();
	if(w+x>m_pRaster->bits->width())
		w=m_pRaster->bits->width()-x;
	if(h+y>m_pRaster->bits->height())
		h=m_pRaster->bits->height()-y;
	m_nWidth=w;
	m_nHeight=h;
	m_nOffX=x;
	m_nOffY=y;
	m_pAddr=m_pRaster->bits->baseAddess()+m_nScaleLine*m_nOffY+m_nOffX*4;
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	m_nScaleX=(player->display.camera.mat.a*20)>>(player->display.antiShift+6);
	m_nScaleY=(player->display.camera.mat.d*20)>>(player->display.antiShift+6);
	player->display.Lock();
	m_nOffsetX=0,m_nOffsetY=0;
	m_nNoScaleX=0,m_nNoScaleY=0;
	m_bIsLocked=true;*/
}

BXDeviceBitmapDataObject::BXDeviceBitmapDataObject(VTable *vtable, ScriptObject* proto, int capacity): FlashObject(vtable, proto, 0)
{
	//m_nWidth=0;
	//m_nHeight=0;
	/*m_pRaster=XNULL;
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	//m_nScaleX=fixed_1;
	m_nOffX=0;
	m_nOffY=0;
	m_nAntiX=0;
	m_nAntiY=0;
	m_pRaster=&player->display.raster;
	m_nWidth=m_pRaster->bits->width();
	m_nHeight=m_pRaster->bits->height();
	m_nScaleLine=m_pRaster->bits->scanLine();
	m_bIsLocked=false;
	m_pAddr=m_pRaster->bits->baseAddess();//+(m_pRaster->bits->scanLine)*m_nHeight+w*4;
	//::MatrixIdentity(&m_totalMatrix);*/
}

void BXDeviceBitmapDataObject::Release()
{
	/*if(IsRunning()&&m_bIsLocked)
	{
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		player->display.UnLock();
	}*/
	//DisplayObjectObject::Release();
}
int BXDeviceBitmapDataObject::AS3_height_get()
{
	return m_nHeight;
}
RectangleObject* BXDeviceBitmapDataObject::AS3_rect_get()
{
	//RectangleClass*pClass=((ShellToplevel*)toplevel())->getRectangleClass();
	//return pClass->CreateRectangle(0,0,m_nWidth,m_nHeight);//Width(),Height());
	return XNULL;
}

int BXDeviceBitmapDataObject::AS3_width_get()
{
	//return m_pDib?m_pDib->Width():
	return m_nWidth;

}



void BXDeviceBitmapDataObject::AS3_applyFilter(BitmapDataObject*,RectangleObject*,PointObject*,BitmapFilterObject*)
{
	//not support filters
}




void BXDeviceBitmapDataObject::Clear()
{
		char*pSrc=m_pAddr;
		//int by=m_nOffY,ey=m_nOffY+m_nHeight;
		//int bx=m_nOffX,ex=m_nOffX+m_nWidth;
		//int copyLen=m_nScaleLine;
		/*if(by<0)
		{
			pSrc+=m_nScaleLine*(-by);		
			by=0;
		}
		if(ey>m_pRaster->bits->height())		
			ey=m_pRaster->bits->height();
		if(bx<0)
		{
			pSrc+=(-bx)*4;
			bx=0;
		}
		if(ex>m_pRaster->bits->width())
		{
			ex=m_pRaster->bits->width();
		}*/
		/*int copyLen=m_nWidth*4;
		if(copyLen>0)
		{
			for(int y=0;y<m_nHeight;y++)
			{
				memset(pSrc,0xff,copyLen);
				pSrc+=m_nScaleLine;
			}
		}*/
		//XSWFPlayer*player=((ShellCore*)core())->GetPlayer();

		//XRect rr(m_nOffX,m_nOffY,m_nOffX+m_nWidth,m_nOffY+m_nHeight);
		//memset(player->display.bits->bitmap->GetAddress(),0xff,
		//	player->bits.width()*player->bits.height()*4);
		//player->display.raster.bits->bitmap->Clear(rr,0xff000000);//new XRect(m_nOffX,m_nOffY,m_nOffX+m_nWidth,m_nOffY+m_nHeight));
		//m_pRaster->bits->bitmap->FillRect(rr,0xff000000,false);
		//player->DirectDraw(rr);
}

BitmapDataObject* BXDeviceBitmapDataObject::AS3_clone()
{
	/*ShellToplevel*top=(ShellToplevel*)toplevel();
	BitmapDataClass*pClass=top->getBitmapDataClass();
	BitmapDataObject*pObj=pClass->CreateBitmapData(m_nWidth,m_nHeight,XFALSE);
	if(pObj->m_pDib!=XNULL)
	{
		char*pSrc=m_pAddr;
		char*pDst=(char*)pObj->m_pDib->GetAddress();
		int dLine=m_nWidth*4;
		//int by=m_nOffY,ey=m_nOffY+m_nHeight;
		//int bx=m_nOffX,ex=m_nOffX+m_nWidth;
		int copyLen=dLine;

		if(copyLen>0)
		{
			for(int y=0;y<m_nHeight;y++)
			{
				memcpy(pDst,pSrc,copyLen);
				pSrc+=m_nScaleLine;
				pDst+=dLine;
			}
		}
	}
	return pObj*/;
	return NULL;
}
void BXDeviceBitmapDataObject::AS3_colorTransform(RectangleObject*r,ColorTransformObject*c)
{
	/*LockBitmap();
	ExpandDummy(XTRUE);
	if(m_pDib)
	{
		AS3RECTANGLEDATA&data=r->GetData();		
		XRect rect(data.left,data.top,data.left+data.width,data.top+data.height);
		XCXForm form;
		c->GetCXForm(form);
		if(m_sbitmap.nImgDiv)
		{
			rect.left>>=m_sbitmap.nImgDiv;
			rect.right>>=m_sbitmap.nImgDiv;
			rect.top>>=m_sbitmap.nImgDiv;
			rect.bottom>>=m_sbitmap.nImgDiv;
		}
		m_pDib->Transform(rect,form);
//#endif
	}*/
}
Atom BXDeviceBitmapDataObject::AS3_compare(BitmapDataObject*src)
{

	/*src->LockBitmap();
	src->ExpandDummy(XFALSE);
	AvmCore* c=core();
	if(src->m_pDib==NULL)
	{
		//if(m_pDib==src->m_pDib)
		//	return zeroIntAtom;
	//	else
			return c->intToAtom(-3);
	}
	if(m_nWidth!=src->m_pDib->Width())
		return c->intToAtom(-3);
	else if(m_nHeight!=src->m_pDib->Height())
		return c->intToAtom(-4);
	else
	{
		int w=m_nWidth,h=m_nHeight;
		BitmapDataObject*pObj=AS3_clone();
		AvmAssert(pObj!=NULL&&pObj->m_pDib!=NULL);
		bool bSame=true;
		int x,t=w*h;
		XU32*pDst=(XU32*)pObj->m_pDib->GetBuffer();
		XU32*pSrc=(XU32*)src->m_pDib->GetBuffer();
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
						p1[0]-=p2[3];
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
	}*/
	return kAvmThunkUndefined;
}



void BXDeviceBitmapDataObject::AS3_copyChannel(BitmapDataObject*src,RectangleObject*r,PointObject*d,uint32_t sc,uint32_t dc)
{
	/*src->LockBitmap();
	src->ExpandDummy(XFALSE);
	if(m_pDib&&src->m_pDib)
	{
		AS3RECTANGLEDATA&data=r->GetData();		
		XRect rect(data.left,data.top,data.left+data.width,data.top+data.height);
		AS3POINTDATA&p=d->GetData();
		XPoint dest(p.x,p.y);
		SameDib(src);
		if(m_sbitmap.nImgDiv)
		{
			DivPoint(dest,m_sbitmap.nImgDiv);
			DivRect(rect,m_sbitmap.nImgDiv);
		}
		bool bAlpha=m_pDib->CopyChannel(*src->m_pDib,
							 rect,
							 dest,
							 (XU8)sc,
							 (XU8)dc);
		if(bAlpha)
			m_pDib->preAlpha();
	}*/
}
void BXDeviceBitmapDataObject::AS3_copyPixels(BitmapDataObject*src,
									 RectangleObject*r,
									 PointObject*d,
									 BitmapDataObject*a,
									 PointObject*ap,bool mea)
{
	/*src->LockBitmap();
	if(a)
		a->LockBitmap();
		
	//if(m_pDib->m_pDib&&src->m_pDib)
	{
		AS3RECTANGLEDATA&data=r->GetData();
		XRect rect(data.left,data.top,data.left+data.width,data.top+data.height);
		Transform(rect,false);
		AS3POINTDATA&dst0=d->GetData();
		SPOINT dst={dst0.x,dst0.y};
		Transform(dst);		
		if(a&&a->m_pDib)
		{
//#if (__CORE_VERSION__>=0x02080000)
			if(ap)
			{
				AS3POINTDATA&adst=ap->GetData();
				m_pRaster->bits->bitmap->Copy(*src->m_pDib,
							  dst.x,dst.y,
							  rect,
							  *a->m_pDib,
							  adst.x,adst.y);
			}
			else
			{
				m_pRaster->bits->bitmap->Copy(*src->m_pDib,
							  dst.x,dst.y,
							  rect,
							  *a->m_pDib,
							  0,0);
			}

		}
		else
		{
			//if(src->m_pDib->Width()==84&&
			//	src->m_pDib->Height()==63)
			//	int v=0;
			//if(a)
			//	int v=0;
			if(mea)
			{
				//return;
				//if(src->m_pDib->m_b
				m_pRaster->bits->bitmap->Overlay(*src->m_pDib,dst.x,dst.y,rect);
			}
			else
			{
				//return;
				m_pRaster->bits->bitmap->Copy(*src->m_pDib,
						 dst.x,
						 dst.y,
						 rect);
			}
		}
	}*/

}

void BXDeviceBitmapDataObject::AS3_dispose()
{
	return;
}


void BXDeviceBitmapDataObject::LimitRect(XRect&rc)
{
			if(rc.left<m_nOffX)
			{
				//rc.right-=(m_nOffX-rc.left);
				rc.left=m_nOffX;
			}
			if(rc.top<m_nOffY)
			{
				//rc.bottom=(m_nOffY-rc.top);
				rc.top=m_nOffY;
			}
			if(rc.right>m_nOffX+m_nWidth)	rc.right=m_nOffX+m_nWidth;
			if(rc.bottom>m_nOffY+m_nHeight) rc.bottom=m_nOffY+m_nHeight;
}

void BXDeviceBitmapDataObject::Draw(BitmapDataObject*pSrc,MATRIX&mat,ColorTransformObject*pTransform,Stringp blendMode,XRect&rc,bool sm)
{
	/*pSrc->LockBitmap();
	pSrc->ExpandDummy(XTRUE);
	if(pSrc->m_pDib)
	{
		mat.tx=m_nAntiX>>(4-m_pRaster->antiShift);//<<disp->antiShift;
		mat.ty=m_nAntiY>>(4-m_pRaster->antiShift);//<<disp->antiShift;
		
		
		XCXForm form,*pForm=NULL;
		if(pTransform)
		{
			pTransform->GetCXForm(form);
			pForm=&form;
		}
		//else
		//	form.Clear();
		//AS3RECTANGLEDATA&data=clip->GetData();
		
		if(pSrc->m_sbitmap.nImgDiv>0)
		{
			int nDiv=pSrc->m_sbitmap.nImgDiv;//-pDst->m_sbitmap.nImgDiv;
			
			{
				MATRIX mat0;
				::MatrixIdentity(&mat0);
				mat0.a<<=pSrc->m_sbitmap.nImgDiv;
				mat0.d<<=pSrc->m_sbitmap.nImgDiv;
				::MatrixConcat(&mat0,&mat,&mat);
			}
		}
		
		
	
		m_pRaster->bits->bitmap->Draw(*pSrc->m_pDib,&mat,pForm,rc,sm);
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		player->DirectDraw(rc);
		//pDst->m_pDib->Draw(*m_pDib,
		//				   pMat,pForm,rc,sm);
	}*/
}


void BXDeviceBitmapDataObject::AS3_draw(Atom s,MatrixObject*m,ColorTransformObject* t,Stringp b,RectangleObject*r,bool sm)
{
	//return;
	/*if(b!=NULL)
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
		MATRIX mat;
		::MatrixIdentity(&mat);
		mat.a=m_nScaleX<<6;
		mat.d=m_nScaleY<<6;
		XRect rcc(m_nOffX,m_nOffY,m_nOffX+m_nWidth,m_nOffY+m_nHeight);
		if(r)
		{
			AS3RECTANGLEDATA&data=r->GetData();
			rcc.left=data.left;
			rcc.top=data.top;
			rcc.right=(data.left+data.width);
			rcc.bottom=(data.top+data.height);
			Transform(rcc,true);
			LimitRect(rcc);
		}
		if(m)
		{
			MATRIX mat0;
			m->GetMatrix(&mat0,false);
			::MatrixConcat(&mat,&mat0,&mat);
		}
		if(rcc.IsRectEmpty()) return;
	
		
		
			//x=(x*m_nScaleX)>>(FIX16OFF-6);
	  

		DisplayObjectObject*pDraw=(DisplayObjectObject*)core()->atomToScriptObject(s);
		ShellToplevel*top=(ShellToplevel*)toplevel();
		//if(ShellCore::IsProtoTypeOf(top->getDisplayObjectClass(),pDraw))
		if(ShellCore::IsTypeClass(pDraw,top->getDisplayObjectClass()))
		{
			XRect rc;
			pDraw->DrawDevice(this,mat,t,b,rcc,sm,rc);
			XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
			//XRect rccc(0,0,620,1280);
			player->DirectDraw(rcc);			
		}
		//else if(ShellCore::IsProtoTypeOf(top->getBXDeviceBitmapDataClass(),pDraw))
		else if(ShellCore::IsTypeClass(pDraw,top->getBitmapDataClass()))
		{
			BitmapDataObject* pBitmap=(BitmapDataObject*)pDraw;
			pBitmap->ExpandDummy(XTRUE);
			//pBitmap->Draw(this,m,t,b,r,sm);
			this->Draw(pBitmap,mat,t,b,rcc,sm);
			XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
			player->DirectDraw(rcc);
		}
		else
			top->throwArgumentError(kInvalidArgumentError,core()->kEmptyString);

		//f(m_pDib->Width()==166&&m_pDib->Height()==24)*/

	
}
void BXDeviceBitmapDataObject::AS3_fillRect(RectangleObject*r,uint32_t c)
{
	//if(m_pDib)
	/*{
		AS3RECTANGLEDATA&data=r->GetData();
		XRect rc(data.left,data.top,data.left+data.width,data.top+data.height);
		Transform(rc,true);
		LimitRect(rc);
		if(rc.IsRectEmpty())
			return;
			//toplevel()->throwArgumentError(kInvalidArgumentError,core()->kEmptyString);
		c|=0xff000000;
#ifdef _ARGB_PIXEL_
		SWAPPIXELRB(c);
#endif
		m_pRaster->bits->bitmap->FillRect(rc,c,XFALSE);
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		player->DirectDraw(rc);
	}*/
}
void BXDeviceBitmapDataObject::AS3_floodFill(int x,int y,uint32_t c)
{
	/*{

		c|=0xff000000;
#ifdef _ARGB_PIXEL_
		SWAPPIXELRB(c);
#endif
		m_pRaster->bits->bitmap->FloodFill(x,y,c);
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		XRect rr(m_nOffX,m_nOffY,m_nOffX+m_nWidth,m_nOffY+m_nHeight);
		player->DirectDraw(rr);
	}*/
}
RectangleObject* BXDeviceBitmapDataObject::AS3_generateFilterRect(RectangleObject*r,BitmapFilterObject*)
{
	return XNULL;
}
RectangleObject* BXDeviceBitmapDataObject::AS3_getColorBoundsRect(uint32_t mask,uint32_t color,bool bFind)
{
	//LockBitmap();	
	/*RectangleClass*pClass=((ShellToplevel*)toplevel())->getRectangleClass();
	RectangleObject*pRect=pClass->CreateRectangle(0,0,0,0);
	XRect rect;
	
	//if(m_pDib)
	{
		
		m_pRaster->bits->bitmap->GetColorBounds(rect,mask,color,bFind);
		
		
	}
	
	pRect->constructor(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top);

	return pRect;*/
	return XNULL;
}
uint32_t BXDeviceBitmapDataObject::AS3_getPixel(int x,int y)
{
	
	/*{
		x+=m_nOffX;
		y+=m_nOffY;
		XU32 c=m_pRaster->bits->bitmap->GetPixel(x,y);//&0xffffff;
#ifdef _ARGB_PIXEL_
		SWAPPIXELRB(c);
#endif
		return c&0xffffff;
	}*/

	return 0;
}
uint32_t BXDeviceBitmapDataObject::AS3_getPixel32(int x,int y)
{
	
	/*{
		x+=m_nOffX;
		y+=m_nOffY;
		XU32 c=m_pRaster->bits->bitmap->GetPixel(x,y);
#ifdef _ARGB_PIXEL_
		SWAPPIXELRB(c);
#endif
		if(!(c>>24))
			return 0;
		return c;
	}*/
	return 0;
}

void BXDeviceBitmapDataObject::AS3_setPixels(RectangleObject*r,ByteArrayObject*cs)
{
	/*LockBitmap();
	ExpandDummy(XTRUE);
	if(m_pDib==NULL||r==NULL||cs==NULL)
		   toplevel()->throwTypeError(kInvalidArgumentError,core()->knull);
	if(m_pDib)
	{
		//int l=cs->get_length();
		//Atom*pAtom=cs->GetAtoms();
		AS3RECTANGLEDATA&data=r->GetData();
		XRect rect(data.left,data.top,data.left+data.width,data.top+data.height);		
		XU32*pData=(XU32*)cs->GetByteArray().GetBuffer();
		if(!m_sbitmap.nImgDiv)
		{
			int x,y,l=(rect.right-rect.left);//,i;		
			if(cs->GetByteArray().GetLength()<l*rect.Height()*4)
				toplevel()->throwError(kOutOfRangeError);
			int top=rect.top;
			int left=rect.left;
			rect&=XRect(0,0,m_pDib->Width(),m_pDib->Height());
			if(rect.top!=top)
			{
				pData+=(rect.top-top)*l;
			}
			if(rect.left!=left)
			{
				pData+=(rect.left-left);
			}
			XU32*pColor=(XU32*)m_pDib->GetBuffer();
			pColor+=rect.top*m_pDib->Width();
			for(y=rect.top;y<rect.bottom;y++)
			{
				XU32*pSrt=(XU32*)pData;
				for(x=rect.left;x<rect.right;x++)
				{
					XU8*srt=(XU8*)pSrt;
					XU8*dst=(XU8*)(pColor+x);
	#ifdef _ARGB_PIXEL_
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
				pColor+=m_pDib->Width();
			}
		}
		
	}*/
}

ByteArrayObject* BXDeviceBitmapDataObject::AS3_getPixels(RectangleObject*r)
{
	//LockBitmap();
	/*ExpandDummy(XFALSE);
	ByteArrayClass*pClass=((ShellToplevel*)toplevel())->getByteArrayClass();
	AS3RECTANGLEDATA&data=r->GetData();
	XRect rect(data.left,data.top,data.left+data.width,data.top+data.height);
	int x,y,l=(rect.right-rect.left);//,i;	
	int nSize=rect.Width()*rect.Height()*sizeof(XU32);
	ByteArrayObject*pArray=pClass->CreateArray(nSize);
	if(m_pDib&&pArray)
	{
		pArray->SetLength(l*rect.Height()*4);

		if(!m_sbitmap.nImgDiv)
		{
			int top=rect.top;
			int left=rect.left;
			rect&=XRect(0,0,m_pDib->Width(),m_pDib->Height());
			XU32*pData=(XU32*)pArray->GetByteArray().GetBuffer();
			if(top!=rect.top)
			{
				pData+=(rect.top-top)*l;
			}
			if(left!=rect.left)
			{
				pData+=(rect.left-left);
			}
			XU32*pColor=(XU32*)m_pDib->GetBuffer();
			pColor+=rect.top*m_pDib->Width();
			
			
			
			for(y=rect.top;y<rect.bottom;y++)
			{
				XU32*pSrt=(XU32*)pData;
				for(x=rect.left;x<rect.right;x++)
				{
					XU8*dst=(XU8*)pSrt;
					XU8*srt=(XU8*)(pColor+x);
	#ifdef _ARGB_PIXEL_
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
				pColor+=m_pDib->Width();
			}	
		}
		else
		{
			int i,nSkip=(1<<m_sbitmap.nImgDiv)-1;
			this->DivRect(rect,m_sbitmap.nImgDiv);
			int top=rect.top;
			int left=rect.left;
			XU32*pData=(XU32*)pArray->GetByteArray().GetBuffer();
			rect&=XRect(0,0,m_pDib->Width(),m_pDib->Height());
			if(top!=rect.top)
			{
				pData+=((rect.top-top)*l)<<m_sbitmap.nImgDiv;
			}
			if(left!=rect.left)
			{
				pData+=(rect.left-left)<<m_sbitmap.nImgDiv;
			}

			rect&=XRect(0,0,m_pDib->Width(),m_pDib->Height());
			XU32*pColor=(XU32*)m_pDib->GetBuffer();
			pColor+=rect.top*m_pDib->Width();
			int lc=(rect.right-rect.left)<<(m_sbitmap.nImgDiv+2);
			
			for(y=rect.top;y<rect.bottom;y++)
			{
				XU32*pSrt=(XU32*)pData;
				for(x=rect.left;x<rect.right;x++)
				{
					XU8*dst=(XU8*)pSrt;
					XU8*srt=(XU8*)(pColor+x);
	#ifdef _ARGB_PIXEL_
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
				pColor+=m_pDib->Width();
			}			
		}
	}
	return pArray;*/
	return XNULL;
}

bool BXDeviceBitmapDataObject::HitTest(XPoint&pt,uint32_t alpha,BitmapDataObject*pBmp,PointObject*second,uint32_t alpha2)
{
	/*LockBitmap();
	ExpandDummy(XFALSE);
	if(!pBmp||!m_pDib)
		return false;

	pBmp->LockBitmap();
	pBmp->ExpandDummy(XFALSE);
	if(!pBmp->m_pDib) return false;
	if(!pBmp->m_bTransparent) {alpha2=alpha=0;}
	XPoint spt(0,0);
	if(second)
	{
		AS3POINTDATA&data=second->GetData();
		spt.x=data.x;
		spt.y=data.y;
		if(m_sbitmap.nImgDiv)
		{
			spt.x>>=m_sbitmap.nImgDiv;
			spt.y>>=m_sbitmap.nImgDiv;
		}
	}
	int w=m_pDib->Width();
	int h=m_pDib->Height();
	int w1=pBmp->m_pDib->Width();
	int h1=pBmp->m_pDib->Height();
	XRect r1(pt.x,pt.y,pt.x+w,pt.y+h);
	XRect r2(spt.x,spt.y,spt.x+w1,spt.y+h1);
	XRect rr=r1;
	rr&=r2;
	if(!rr.IsRectEmpty())
	{
		if(!alpha||!alpha2) return true;//var.iData32=XTRUE;
		else
		{
			if(!alpha)
			{
				rr-=r2;
				return pBmp->m_pDib->HitTest(rr,alpha2)!=0;
			}
			else if(!alpha2)
			{
				rr-=r2;
				return pBmp->m_pDib->HitTest(rr,alpha)!=0;
			}
			else
			{
				XU32*ps1=(XU32*)m_pDib->GetBuffer();
				XU32*ps2=(XU32*)pBmp->m_pDib->GetBuffer();
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
	return false;*/
	return true;
}

bool BXDeviceBitmapDataObject::AS3_hitTest(PointObject* first,uint32_t ft,Atom seo,PointObject* secend,uint32_t st)
{
	//return false;
	return true;
	/*LockBitmap();
	ExpandDummy(XFALSE);
	if(m_pDib==NULL||first==NULL)
		   toplevel()->throwTypeError(kInvalidArgumentError,core()->knull);
	if(m_pDib)
	
	{
		//if(m_pDib->Width()==1440)
		//	int v=0;
		//ShellCore*c=(ShellCore*)core();
		ScriptObject*pSecond=core()->atomToScriptObject(seo);
		ShellToplevel*t=(ShellToplevel*)toplevel();
		PointClass*ppClass=t->getPointClass();
		RectangleClass*prClass=t->getRectangleClass();
		BitmapClass*pbClass=t->getBitmapClass();
		BXDeviceBitmapDataClass*pbdClass=t->getBXDeviceBitmapDataClass();
		AS3POINTDATA&ptt=first->GetData();
		XPoint pt(ptt.x,ptt.y);
		if(m_sbitmap.nImgDiv)
		{
			pt.x>>=m_sbitmap.nImgDiv;
			pt.y>>=m_sbitmap.nImgDiv;
		}
		//if(ShellCore::IsProtoTypeOf(ppClass,pSecond))
		if(ShellCore::IsTypeClass(pSecond,ppClass))
		{//Point
			AS3POINTDATA&data=((PointObject*)pSecond)->GetData();
			XPoint dpt(data.x-pt.x,data.y-pt.y);
			int w=m_pDib->Width(),h=m_pDib->Height();
#if (__CORE_VERSION__>=0x02072000)
			XRect rect(0,0,w-1,h-1);
#else
			XRect rect(pt.x,pt.y,pt.x+w-1,pt.y+h-1);
#endif
			if(rect.PtInRect(dpt))
			{
				//if(!ft)
				//	return true;
					//var.iData32=XTRUE;
				//else
				{//[Note X]
					XU32*pBuf=(XU32*)m_pDib->GetBuffer();
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
			int w=m_pDib->Width();
			int h=m_pDib->Height();
#if (__CORE_VERSION__>=0x02072000)
			XRect rt(0,0,w,h);
#else
			XRect rt(pt.x,pt.y,pt.x+w,pt.y+h);
			rt&=rect;
#endif
			if(!rect.IsRectEmpty())
				return m_pDib->HitTest(rt,ft)!=0;

		}
		//else if(ShellCore::IsProtoTypeOf(pbClass,pSecond))
		else if(ShellCore::IsTypeClass(pSecond,pbClass))
		{
			//XXObjectBitmap*pBmp=(XXObjectBitmap*)list[2].pObject;
			//if(pBmp->m_pDib==XNULL) break;
			//if(!pBmp->m_bTransparent) alpha=0;
			BXDeviceBitmapDataObject*pBmp=((BitmapObject*)pSecond)->m_pBXDeviceBitmapData;
			if(!pBmp||!pBmp->m_pDib) return false;
			//XPoint spt(0,0);			
			return HitTest(pt,ft,pBmp,secend,st);
			
		}
		//else if(ShellCore::IsProtoTypeOf(pbdClass,pSecond))
		else if(ShellCore::IsTypeClass(pSecond,pbdClass))
		{
			BXDeviceBitmapDataObject*pBmp=(BXDeviceBitmapDataObject*)pSecond;
			if(!pBmp->m_pDib) return false;
			return HitTest(pt,ft,pBmp,secend,st);
		}
		else
			toplevel()->throwArgumentError(kInvalidArgumentError,core()->kEmptyString);
		//pSecond->
	}
	return false;*/
}

void BXDeviceBitmapDataObject::AS3_merge(BitmapDataObject*pBmp,
								 RectangleObject* r,
								 PointObject*d,
								 uint32_t rm,
								 uint32_t gm,
								 uint32_t bm,
								 uint32_t am)
{
	/*LockBitmap();
	ExpandDummy(XTRUE);
	if(m_pDib==NULL||r==NULL||d==NULL||pBmp==NULL)
		   toplevel()->throwTypeError(kInvalidArgumentError,core()->knull);
	pBmp->LockBitmap();
	pBmp->ExpandDummy(XFALSE);
	if(m_pDib&&pBmp->m_pDib)
	{
		AS3POINTDATA&pdata=d->GetData();
		AS3RECTANGLEDATA&data=r->GetData();
		XRect rect(data.left,data.top,data.left+data.width,data.top+data.height);
		XPoint pt(pdata.x,pdata.y);
		SameDib(pBmp);
		if(m_sbitmap.nImgDiv)
		{
			DivPoint(pt,m_sbitmap.nImgDiv);
			DivRect(rect,m_sbitmap.nImgDiv);
		}
#ifdef _ARGB_PIXEL_
		uint32_t tmp=rm;
		rm=bm;
		bm=rm;
#endif
		m_pDib->Mix(*pBmp->m_pDib,
						 rect,pt,
						 rm,gm,bm,am);
	}*/

}
void BXDeviceBitmapDataObject::AS3_noise(int nSeed,uint32_t nLow,uint32_t nHigh,uint32_t nOption,bool nGray)
{
	//LockBitmap();
	//ExpandDummy(XTRUE);
	//if(m_pDib)
	/*{
		if(!(nOption&0xf)) return;
		XGlobal::Seed(nSeed);
		m_pDib->Noise(nLow,nHigh,nOption,nGray);
	}*/
}
void BXDeviceBitmapDataObject::AS3_paletteMap(BitmapDataObject*pBmp,
									  RectangleObject* r,
									  PointObject*d,
									  ArrayObject*ra,
									  ArrayObject*ga,
									  ArrayObject*ba,
									  ArrayObject*aa)
{
	/*LockBitmap();
	ExpandDummy(XTRUE);
#if (__CORE_VERSION__>=0x02081000)
	if(m_pDib==NULL||r==NULL||d==NULL || pBmp == NULL )
#else
	if(m_pDib==NULL||r==NULL||d==NULL||pBmp)
#endif
		   toplevel()->throwTypeError(kInvalidArgumentError,core()->knull);
	pBmp->ExpandDummy(XFALSE);
	pBmp->LockBitmap();
#ifdef _ARGB_PIXEL_
	ArrayObject*tmp=ra;
	ra=ba;
	ba=tmp;
#endif
	if(!pBmp->m_pDib)
		toplevel()->throwTypeError(kInvalidArgumentError,core()->knull);
	if(m_pDib)
	{
		AS3POINTDATA&pdata=d->GetData();
		AS3RECTANGLEDATA&data=r->GetData();
		XRect rect(data.left,data.top,data.left+data.width,data.top+data.height);
		XPoint dst(pdata.x,pdata.y);
		SameDib(pBmp);
		if(m_sbitmap.nImgDiv)
		{
			DivPoint(dst,m_sbitmap.nImgDiv);
			DivRect(rect,m_sbitmap.nImgDiv);
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
		m_pDib->PaletteMap(*pBmp->m_pDib,rect,dst,reds,greens,blues,alphas);
	}*/
}
void BXDeviceBitmapDataObject::AS3_perlinNoise(double,double,uint32_t nSeed,int,bool,bool,uint32_t nOption,bool nGray,ArrayObject*)
{
	/*LockBitmap();
	ExpandDummy(XTRUE);
	if(m_pDib)
	{
		if(!(nOption&0xf)) return;
		XGlobal::Seed(nSeed);
		m_pDib->Noise(0,255,nOption,nGray);
	}*/
}
int BXDeviceBitmapDataObject::AS3_pixelDissolve(BitmapDataObject*pBmp,
										 RectangleObject* r,
										 PointObject*d,
										 int nSeed,int nPixels,uint32_t nColor)
{
	toplevel()->throwTypeError(kInvalidArgumentError,core()->knull);
	return 0;
	/*LockBitmap();
	ExpandDummy(XTRUE);
	if(m_pDib==NULL||r==NULL||d==NULL||nPixels<0||pBmp==NULL)
		   toplevel()->throwTypeError(kInvalidArgumentError,core()->knull);
	pBmp->LockBitmap();
	pBmp->ExpandDummy(XFALSE);
	if(!pBmp->m_pDib)
		toplevel()->throwTypeError(kInvalidArgumentError,core()->knull);
	AS3POINTDATA&pdata=d->GetData();
	AS3RECTANGLEDATA&data=r->GetData();
	SameDib(pBmp);
#ifdef _ARGB_PIXEL_
	SWAPPIXELRB(nColor);
#endif
	XRect rect(data.left,data.top,data.left+data.width,data.top+data.height);
	XPoint dst(pdata.x,pdata.y);
	if(m_sbitmap.nImgDiv)
	{
		DivPoint(dst,m_sbitmap.nImgDiv);
		DivRect(rect,m_sbitmap.nImgDiv);
	}
	if(!m_bTransparent)
			nColor|=0xff000000;
	if(pBmp==this)
		nSeed=m_pDib->Dissolve(rect,dst,nSeed,nPixels,nColor);
	else
		nSeed=m_pDib->Dissolve(*pBmp->m_pDib,rect,dst,nSeed,nPixels);
	return nSeed;*/
}

inline void BXDeviceBitmapDataObject::Transform(int&x,int&y)
{
	if(m_nScaleX!=SCALEONE)
		x=(x*m_nScaleX)>>(FIX16OFF-6);
	if(m_nScaleY!=SCALEONE)
	y=(y*m_nScaleY)>>(FIX16OFF-6);
	x+=m_nOffX;
	y+=m_nOffY;
	//if(::MatrixIsIdentity(&m_totalMatrix)) return;
	/*MATRIX mat;
	::MatrixIdentity(
	SPOINT p={x,y};
	::MatrixTransformPoint(&m_totalMatrix,&p,&p);
	x=p.x;
	y=p.y;*/
}


inline void BXDeviceBitmapDataObject::Transform(SPOINT&pt)
{
	if(m_nScaleX!=SCALEONE)
		pt.x=(pt.x*m_nScaleX)>>(FIX16OFF-6);
	if(m_nScaleY!=SCALEONE)
	pt.y=(pt.y*m_nScaleY)>>(FIX16OFF-6);
	pt.x+=m_nOffX;
	pt.y+=m_nOffY;
	//if(::MatrixIsIdentity(&m_totalMatrix)) return;
	/*MATRIX mat;
	::MatrixIdentity(
	SPOINT p={x,y};
	::MatrixTransformPoint(&m_totalMatrix,&p,&p);
	x=p.x;
	y=p.y;*/
}

inline void BXDeviceBitmapDataObject::TransformSize(int&x,int&y)
{
	if(m_nScaleX!=SCALEONE)
		x=(x*m_nScaleX)>>(FIX16OFF-6);
	if(m_nScaleY!=SCALEONE)
		y=(y*m_nScaleY)>>(FIX16OFF-6);
	//if(::MatrixIsIdentity(&m_totalMatrix)) return;
	/*MATRIX mat;
	::MatrixIdentity(
	SPOINT p={x,y};
	::MatrixTransformPoint(&m_totalMatrix,&p,&p);
	x=p.x;
	y=p.y;*/
}

inline void BXDeviceBitmapDataObject::Transform(int&x,int&y,int&w,int&h)
{
	if(m_nScaleX!=SCALEONE)
	{
		x=(x*m_nScaleX)>>(FIX16OFF-6);
		w=(w*m_nScaleX)>>(FIX16OFF-6);
	}
	if(m_nScaleY!=SCALEONE)
	{
		y=(y*m_nScaleY)>>(FIX16OFF-6);	
		h=(h*m_nScaleY)>>(FIX16OFF-6);
	}
	x+=m_nOffX;
	y+=m_nOffY;
	/*if(::MatrixIsIdentity(&m_totalMatrix)) return;
	SRECT r={x,y,x+w,y+h};
	::MatrixTransformRect(&m_totalMatrix,&r,&r);
	x=r.xmin;
	y=r.ymin;
	w=r.xmax-r.xmin;
	h=r.ymax-r.ymin;*/
}

inline void BXDeviceBitmapDataObject::Transform(XRect&rc,bool offset)
{
	if(m_nScaleX!=SCALEONE)
	{
		rc.left=(rc.left*m_nScaleX)>>(FIX16OFF-6);
		rc.right=(rc.right*m_nScaleX)>>(FIX16OFF-6);
	}
	if(m_nScaleY!=SCALEONE)
	{
		rc.top=(rc.top*m_nScaleY)>>(FIX16OFF-6);	
		rc.bottom=(rc.bottom*m_nScaleY)>>(FIX16OFF-6);
	}
	if(offset)
	{
		rc.left+=m_nOffX;
		rc.right+=m_nOffX;
		rc.top+=m_nOffY;
		rc.bottom+=m_nOffY;
	}
	/*if(::MatrixIsIdentity(&m_totalMatrix)) return;
	SRECT r={rc.left,rc.top,rc.right,rc.bottom};
	::MatrixTransformRect(&m_totalMatrix,&r,&r);
	rc.left=r.xmin;
	rc.top=r.ymin;
	rc.right=r.xmax;
	rc.bottom=r.ymax;*/
}


void BXDeviceBitmapDataObject::AS3_scroll(int x,int y)
{
	/*m_nNoScaleX+=x;
	m_nNoScaleY+=y;
	TransformSize(x,y);
	m_nOffsetX+=x;
	m_nOffsetY+=y;
	int ox=m_nNoScaleX;
	int oy=m_nNoScaleY;
	TransformSize(ox,oy);
	ox-=m_nOffsetX;
	oy-=m_nOffsetY;
	if(ox<=-1||ox>=1)
	{
		//int v=0;
		x+=ox;
		m_nOffsetX+=ox;
	}
	if(oy<=-1||oy>=1)
	{
		//int v=0;
		y+=oy;
		m_nOffsetY+=oy;
	}
	if(x==0&&y==0) return;
	//x=x*m_totalMatrix.a/fixed_1;
	//y=y*m_totalMatrix.b/fixed_1;
	char*pDst=m_pAddr+y*m_nScaleLine;
	char*pSrc=m_pAddr;
	int copyLen=m_nWidth*4,i;
	if(x<0)
	{
		pSrc-=x*4;
		copyLen+=x*4;
	}
	else if(x>0)
	{
		pDst+=x*4;
		copyLen-=x*4;
	}
	if(y<0)
	{
		pDst-=y*m_nScaleLine;
		pSrc-=y*m_nScaleLine;
		for(i=0;i<m_nHeight+y;i++)
		{
			memcpy(pDst,pSrc,copyLen);
			pDst+=m_nScaleLine;
			pSrc+=m_nScaleLine;
		}
	}
	else if(y>0)
	{
		pDst+=(m_nHeight-y-1)*m_nScaleLine;
		pSrc+=(m_nHeight-y-1)*m_nScaleLine;
		for(i=0;i<m_nHeight-y;i++)
		{
			memcpy(pDst,pSrc,copyLen);
			pDst-=m_nScaleLine;
			pSrc-=m_nScaleLine;
		}
	}
	else
	{
		char*pBuf=new char[copyLen];
		for(i=0;i<m_nHeight+y;i++)
		{
			memcpy(pBuf,pSrc,copyLen);
			memcpy(pDst,pBuf,copyLen);
			pDst+=m_nScaleLine;
			pSrc+=m_nScaleLine;
		}
		delete pBuf;
	}
	//LockBitmap();
	//ExpandDummy(XTRUE);
	//if(m_sbitmap.nImgDiv)
	//{
	//	x>>=m_sbitmap.nImgDiv;
	//	y>>=m_sbitmap.nImgDiv;
//	}
//	if(m_pDib)
//		m_pDib->Scroll(x,y);
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		XRect rr(m_nOffX,m_nOffY,m_nOffX+m_nWidth,m_nOffY+m_nHeight);
		player->DirectDraw(rr);*/
}
void BXDeviceBitmapDataObject::AS3_setPixel(int x,int y,uint32_t c)
{/*
	LockBitmap();
	ExpandDummy(XTRUE);
	if(m_sbitmap.nImgDiv)
	{
		x>>=m_sbitmap.nImgDiv;
		y>>=m_sbitmap.nImgDiv;
	}
#ifdef _ARGB_PIXEL_
	SWAPPIXELRB(c);
#endif
	if(m_pDib)
		m_pDib->SetPixel(x,y,c,XFALSE);*/
}
void BXDeviceBitmapDataObject::AS3_setPixel32(int x,int y,uint32_t c)
{
	/*LockBitmap();
#ifdef _ARGB_PIXEL_
	SWAPPIXELRB(c);
#endif
	if(m_pDib)
		m_pDib->SetPixel(x,y,c,XTRUE);*/
}

void BXDeviceBitmapDataObject::AS3_lock()
{
	//LockBitmap();
	//if(m_pDib)
	//	m_pDib->Lock();
}

void BXDeviceBitmapDataObject::SetObject(void*pObject){}

uint32_t BXDeviceBitmapDataObject::AS3_threshold(BitmapDataObject*pBmp,
										 RectangleObject*r,
										 PointObject*d,
										 Stringp opt,
										 uint32_t thre,uint32_t nColor,uint32_t mask,bool bCopy)
{
		toplevel()->throwTypeError(kInvalidArgumentError,core()->knull);
		return 0;
		/*LockBitmap();
		ExpandDummy(XTRUE);
		if(pBmp==NULL||m_pDib==NULL||r==NULL||d==NULL)
			toplevel()->throwTypeError(kInvalidArgumentError,core()->knull);
		pBmp->LockBitmap();
		pBmp->ExpandDummy(XFALSE);
		if(!pBmp->m_pDib)
			return 0;
		//if(!m_pDib||!pBmp->m_pDib) return 0;
		AS3POINTDATA&pdata=d->GetData();
		AS3RECTANGLEDATA&data=r->GetData();
		SameDib(pBmp);
		XRect rect(data.left,data.top,data.left+data.width,data.top+data.height);
		XPoint dst(pdata.x,pdata.y);
		if(m_sbitmap.nImgDiv)
		{
			DivPoint(dst,m_sbitmap.nImgDiv);
			DivRect(rect,m_sbitmap.nImgDiv);
		}
		XU8 nOpt=0;
		StUTF8String str(opt);
#ifdef _ARGB_PIXEL_
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
		return m_pDib->Threshold(*pBmp->m_pDib,rect,dst,nOpt,thre,mask,nColor,bCopy);*/

}
void BXDeviceBitmapDataObject::AS3_unlock(RectangleObject*)
{
	/*if(m_bIsLocked)
	{
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		player->display.UnLock();
	}*/
}

//new adds end

UIntVectorObject* BXDeviceBitmapDataObject::AS3_getVector(RectangleObject *pRect)
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
ObjectVectorObject* BXDeviceBitmapDataObject::AS3_histogram(RectangleObject *pHRect)
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
void BXDeviceBitmapDataObject::AS3_setVector(RectangleObject *pRect, UIntVectorObject *pInputVector)
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

//void BXDeviceBitmapDataObject::AS3_setVector(RectangleObject *pRect, UIntVectorObject *pInputVector)
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
//}


}
