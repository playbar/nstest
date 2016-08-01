// XXObjectBitmap1.cpp: implementation of the XXObjectBitmap class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XXObjectBitmap1.h"
#include "splayer.h"
#include "ObjectCreator.h"
#include "XBitmap.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define XNEEDBITMAPR XNEEDOBJR(m_pDib,cnt)
#define XNEEDBITMAP  XNEEDOBJ(m_pDib)

XXObjectBitmap::XXObjectBitmap(ScriptPlayer*pRoot):XXObject(pRoot)
{
//   m_bNeedDelete=XTRUE;
   //m_pDib=XFALSE;
   //m_bTransparent=XTRUE;
	m_pChar = XNULL;
	m_bModify = false;
}

void XXObjectBitmap::ReleaseChar(_XSCharacter*ch)
{
	/*if(!ch) return;
	if(ch->type!=bitmapEmptyChar)
	{
		ch->bits.UnlockBits();
		if(ch->bits.bitmapID)
		{
			//if(!ch->bits.bitmapID->nLockCount)
			//	ch->bits.PIFree();
			if(!RasterGL::IsLocked(ch->bits.bitmapID))
				ch->bits.PIFree();
		}
		//if(!ch->bits.lockCount)
		//	ch->bits.PIFree();
	}
	else
	{
		RasterGL::UnLockTexture(ch->frame->mTexture);
		//ch->useCount--;
		//if(!ch->useCount)
		if(!RasterGL::IsLocked(ch->frame->mTexture))
			XSWFPlayer::m_pInstance->FreeCharacter(ch);
	}*/
	if(!ch) return;
	ch->bits.ReleaseUse();
	if(ch->bits.useCount<=0)
	{
		if(ch->type==bitmapEmptyChar)
			XSWFPlayer::m_pInstance->FreeCharacter(ch);
		else
			ch->bits.PIFree();
	}
}

XXObjectBitmap::~XXObjectBitmap()
{
	ReleaseChar(m_pChar);
}

XBOOL XXObjectBitmap::SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	TextureInfo info;
	GetBmInfo(info);
	switch(id)
	{
	case _SYSID(copyPixels):
	case _SYSID(loadBitmap):
	case _SYSID(clone):
	case _SYSID(colorTransform):
	case _SYSID(copyChannel):
	case _SYSID(draw):
	case _SYSID(fillRect):
	case _SYSID(floodFill):
	case _SYSID(getColorBoundsRect):
	case _SYSID(hitTest):
	case _SYSID(merge):
	case _SYSID(noise):
	case _SYSID(pixelDissolve):
	case _SYSID(scroll):
	case _SYSID(setPixel):
	case _SYSID(getPixel):
	case _SYSID(setPixel32):
	case _SYSID(getPixel32):
	case _SYSID(threshold):
	case _SYSID(dispose):
		 var=this;
		 var.ToFunction(id);
		 return XTRUE;
	case _SYSID(width):
		//if(m_pDib)
		//	var=m_pDib->Width();
		if(m_pChar)
			var=info.nDiv?(info.w<<info.nDiv):info.w;//m_pChar->bits.width;
		return XTRUE;
	case _SYSID(height):
		//if(m_pDib)
		//	var=m_pDib->Height();
		if(m_pChar)
			var=info.nDiv?(info.h<<info.nDiv):info.h;
			//m_pChar->bits.height;
		return XTRUE;
	case _SYSID(rectangle):
		if(m_pChar)
		{
			XXObjectRectangle*p=m_pRoot->m_pGlobal->CreateRectangle();
			if(p)
			{
				if(info.nDiv)
				{
					p->width=info.w<<info.nDiv;
					p->height=info.h<<info.nDiv;
				}
				else
				{
					p->width=info.w;
					p->height=info.h;
				}
				//p->width=(float)m_pChar->bits.width;
				//p->height=(float)m_pChar->bits.height;
			}
			
			
			var=p;
		}
		return XTRUE;
	case _SYSID(transparent):
		var=this->IsTransparent();//m_bTransparent;
		return XTRUE;
	}
	return XXObject::SysGetMember(id,pCnt,var);
}
XBOOL XXObjectBitmap::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{
	//return XTRUE;
	switch(id)
	{
	case _SYSID(loadBitmap):
	case _SYSID(dispose):
		break;
	default:
		return XTRUE;
	}
	switch(id)
	{
	case _SYSID(dispose):
		if(m_pChar)//m_pDib)
		{
			this->dispose(*pCnt,list);
		//{
		//	XSWFPlayer::m_pInstance->FreeCharacter(m_pChar);
		//	m_pChar = XNULL;
			//delete m_pDib;
			//m_pDib=XNULL;
		}return XTRUE;
	case _SYSID(copyPixels):
		 CopyPixel(list);
		 return XTRUE;
	case _SYSID(loadBitmap):
		 loadBitmap(*pCnt,list);
		 return XTRUE;
	case _SYSID(clone):
		clone(*pCnt,list);
		return XTRUE;
	case _SYSID(colorTransform):
		colorTransform(*pCnt,list);
		return XTRUE;
	case _SYSID(copyChannel):
		copyChannel(*pCnt,list);
		return XTRUE;
	case _SYSID(draw):
		draw(*pCnt,list);
		return XTRUE;
	case _SYSID(fillRect):
		fillRect(*pCnt,list);
		return XTRUE;
	case _SYSID(floodFill):
		floodFill(*pCnt,list);
		return XTRUE;
	case _SYSID(getColorBoundsRect):
		getColorBoundsRect(*pCnt,list);
		return XTRUE;
	case _SYSID(hitTest):
		hitTest(*pCnt,list);
		return XTRUE;
	case _SYSID(merge):
		merge(*pCnt,list);
		return XTRUE;
	case _SYSID(noise):
		noise(*pCnt,list);
		return XTRUE;
	case _SYSID(pixelDissolve):
		pixelDissolve(*pCnt,list);
		return XTRUE;
	case _SYSID(scroll):
		scroll(*pCnt,list);
		return XTRUE;
	case _SYSID(setPixel):
		setPixel(*pCnt,list);
		return XTRUE;
	case _SYSID(getPixel):
		getPixel(*pCnt,list);
		return XTRUE;
	case _SYSID(setPixel32):
		setPixel32(*pCnt,list);
		return XTRUE;
	case _SYSID(getPixel32):
		getPixel32(*pCnt,list);
		return XTRUE;
	case _SYSID(threshold):
		threshold(*pCnt,list);
		return XTRUE;
	}
	return XXObject::SysCallMethod(id,pCnt,list);
}



void XXObjectBitmap::InitObject( )
{
	_SYSCALL(copyPixels);//_SYSCALL();//(_strCopyPixels,XXVar(XXFUNOBJ(copyPixels)));
	_SYSCALL(loadBitmap);//_SYSCALL();//(_strLoadBitmap,XXVar(XXFUNOBJ(loadBitmap)));
	_SYSCALL(clone);//_SYSCALL();//(_strClone,XXVar(XXFUNOBJ(clone)));
	_SYSCALL(colorTransform);//_SYSCALL();//(_strColorTransforms,XXVar(XXFUNOBJ(colorTransform)));
	_SYSCALL(copyChannel);//_SYSCALL();//(_strCopyChannel,XXVar(XXFUNOBJ(copyChannel)));
	_SYSCALL(draw);//_SYSCALL();//(_strDraw,XXVar(XXFUNOBJ(draw)));
	_SYSCALL(dispose);
	_SYSCALL(fillRect);//_SYSCALL();//(_strFillRect,XXVar(XXFUNOBJ(fillRect)));
	_SYSCALL(floodFill);//(_strFloodFill,XXVar(XXFUNOBJ(floodFill)));
	_SYSCALL(getColorBoundsRect);//(_strGetColorBoundsRect,XXVar(XXFUNOBJ(getColorBoundsRect)));
	_SYSCALL(hitTest);//(_strHitTest,XXVar(XXFUNOBJ(hitTest)));
	_SYSCALL(merge);//(_strMerge,XXVar(XXFUNOBJ(merge)));
	_SYSCALL(noise);//(_strNoise,XXVar(XXFUNOBJ(noise)));
	_SYSCALL(pixelDissolve);//(_strPixelDissolve,XXVar(XXFUNOBJ(pixelDissolve)));
	_SYSCALL(scroll);//(_strScroll,XXVar(XXFUNOBJ(scroll)));
	_SYSCALL(setPixel);//(_strSetPixel,XXVar(XXFUNOBJ(setPixel)));
	_SYSCALL(getPixel);//(_strGetPixel,XXVar(XXFUNOBJ(getPixel)));
	_SYSCALL(setPixel32);//(_strSetPixel32,XXVar(XXFUNOBJ(setPixel32)));
	_SYSCALL(getPixel32);//(_strGetPixel32,XXVar(XXFUNOBJ(getPixel32)));
	_SYSCALL(threshold);//(_strThreshold,XXVar(XXFUNOBJ(threshold)));

	_SYSCALL(width);//(_strWidth,XXFUNOBJ(widthGet),XNULL);
	_SYSCALL(height);//(_strHeight,XXFUNOBJ(heightGet),XNULL);
	_SYSCALL(rectangle);//(_strRectangles,XXFUNOBJ(rectangleGet),XNULL);
	_SYSCALL(transparent);//(_strTransparent,XXFUNOBJ(transparentGet),XNULL);
}


void XXObjectBitmap::applyFilter(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XNEEDBITMAPR
	LockBitmap(true);
	CopyPixel(list);
	cnt.pStack->Push(0);//(0));
}
void XXObjectBitmap::clone(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XNEEDBITMAPR
	
	XXObjectBitmap*pBmp=CreateClass(m_bBase?this:m_pSuper,XNULL,XFALSE);
	if(pBmp)
	{
		pBmp->m_pChar = m_pChar;//m_pDib=m_pDib->Clone();
		pBmp->m_bAttach = true;
		m_pChar->bits.AddUse();
		//this->m_bAttach = true;
		//_XSCharacter*ch = m_pChar;
		/*if(ch->type==bitmapEmptyChar)
			RasterGL::LockTexture(ch->frame->mTexture);
			//ch->useCount++;
		else
			ch->bits.LockBits();*/
//		pBmp->m_bNeedDelete=XTRUE;
	}
	cnt.pStack->Push((pBmp));
}
void XXObjectBitmap::colorTransform(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XNEEDBITMAP
	
	if(list.GetSize()<2) return;
	if(list[0].IsObject(XXOBJ_RECTANGLE)&&
	   list[1].IsObject(XXOBJ_COLORTRANSFORM))
	{
		LockBitmap(true);
		if(IsEmptyBits()) return;
		XRect rect;
		//XCXForm form;
		((XXObjectRectangle*)list[0].pObject)->GetRect(rect,XFALSE);
		int nDiv = m_pChar->bits.nImgDiv;//frame->nImgDiv;
		if(nDiv)
		{
			rect.left>>=nDiv;
			rect.right>>=nDiv;
			rect.top>>=nDiv;
			rect.bottom>>=nDiv;
		}
		XDrawDib dib;
		dib.CreateFrom(&m_pChar->bits,true);
		//m_pChar->frame->Transform(rect.left,rect.top,rect.Width(),rect.Height(),
		//	((XXObjectColorTransform*)list[1].pObject)->cxForm);
		//XDrawDib dib,*m_pDib=&dib;
		//dib.CreateFrom(&m_pChar->bits,true);
		dib.Transform(rect,((XXObjectColorTransform*)list[1].pObject)->cxForm);	
		m_bModify = true;
		//((XXObjectColorTransform*)list[1].pObject)->GetCXForm(form);
	}
}
void XXObjectBitmap::copyChannel(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XNEEDBITMAP
	if(list.GetSize()<5) return;
	if(!list[0].IsObject(XXOBJ_BITMAP)||
		list[1].IsObject(XXOBJ_RECTANGLE)||
		list[2].IsObject(XXOBJ_POINT)) return;
	XXObjectBitmap*pBmp=(XXObjectBitmap*)list[0].pObject;
	LockBitmap(true);
	if(IsEmptyBits()||pBmp->IsEmptyBits())
		return;
	//LockBitmap(true);
	pBmp->LockBitmap(false);
	XRect rect;
	((XXObjectRectangle*)list[1].pObject)->GetRect(rect,XFALSE);
	XPoint dst;
	((XXObjectPoint*)list[2].pObject)->GetPoint(dst);
	//TextureInfo info;
	//pBmp->GetBmInfo(info);
	int nDiv=m_pChar->bits.nImgDiv;//m_pChar->frame->nImgDiv;
	if(nDiv)
	{
		dst.x>>=nDiv;
		dst.y>>=nDiv;
	}
	if(pBmp->m_pChar->bits.nImgDiv)
	{
		int nDiv = pBmp->m_pChar->bits.nImgDiv;
		rect.left>>=nDiv;
		rect.right>>=nDiv;
		rect.top>>=nDiv;
		rect.right>>=nDiv;
	}
	//m_pChar->frame->CopyChannel(info,rect,dst,list[3].ToInt(),list[4].ToInt());
	XDrawDib dib;
	dib.CreateFrom(&m_pChar->bits,true);
	XDrawDib dib1;
	dib1.CreateFrom(&pBmp->m_pChar->bits,true);
	dib.CopyChannel(dib1,
							     rect,
								 dst,
								 list[3].ToInt(),
								 list[4].ToInt());
	m_bModify = true;
}
//DEL void XXObjectBitmap::copyPixels(XSWFCONTEXT&cnt,XXVARLIST&list)
//DEL {
//DEL 	XNEEDBITMAP
//DEL 	CopyPixel(list);
//DEL }
void XXObjectBitmap::dispose(XSWFCONTEXT&cnt,XXVARLIST&list)
{
//	XNEEDBITMAP
//	if(m_bNeedDelete)
	ReleaseChar(m_pChar);
	m_pChar = NULL;
	/*if(m_pChar&&!m_bAttach)
	{
		XSWFPlayer::m_pInstance->FreeCharacter(m_pChar);
		m_pChar = XNULL;
		//delete m_pDib;
		//m_pDib=XNULL;
	}*/
}
void XXObjectBitmap::draw(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XNEEDBITMAP
	LockBitmap(true);
	if(IsEmptyBits()) return;
	int nSize=list.GetSize();
	if(nSize<1) return;
	XBOOL bBitmap=list[0].IsObject(XXOBJ_BITMAP);
	if(!bBitmap)
	{
		if(!list[0].IsObject(XXOBJ_CLIP)) return;
		if(!list[0].pObject||!list[0].pObject->m_pObject) return;	
	}
	else if(!list[0].pObject) return;
	//LockBitmap(true);

	//SObject*pObj=list[0].pObject->m_pObject;
	STransform x;
	MATRIX *pMatrix=XNULL;
	if(nSize>=2&&list[1].IsObject(XXOBJ_MATRIX))
	{
		x.mat=((XXObjectMatrix*)list[1].pObject)->matrix;
		x.mat.tx/=20;
		x.mat.ty/=20;
		pMatrix=&x.mat;
	}
	else
		::MatrixIdentity(&x.mat);
	XCXForm *pForm=XNULL;	
	if(nSize>=3&&list[2].IsObject(XXOBJ_COLORTRANSFORM))
	{
		x.cxform=((XXObjectColorTransform*)list[2].pObject)->cxForm;
		pForm=&x.cxform;
	}
	else
		x.cxform.Clear();
	XRect rc(0,0,m_pChar->bits.width,m_pChar->bits.height);//m_pDib->Width(),m_pDib->Height());
	if(nSize>=5&&list[4].IsObject(XXOBJ_RECTANGLE))
	{
		((XXObjectRectangle*)list[5].pObject)->GetRect(rc,XFALSE);

	}
	XBOOL bSmooth=XFALSE;
	if(nSize>=6)
		bSmooth=list[6].ToLogic();
	int nDiv = m_pChar->bits.nImgDiv;//frame->nImgDiv;
	if(nDiv)
	{
		rc.left >>= nDiv;
		rc.right >>= nDiv;
		rc.top >>= nDiv;
		rc.bottom >>= nDiv;
		MATRIX mat;
		::MatrixIdentity(&mat);
		mat.a = mat.d = fixed_1>>nDiv;
		::MatrixConcat(&x.mat,&mat,&x.mat);
	}
	if(bBitmap)
	{
		XXObjectBitmap*pBmp=(XXObjectBitmap*)list[0].pObject;
		//if(!pBmp->IsEmpty())
		{
			XDrawDib dib;//,*m_pDib=&dib;
			dib.CreateFrom(&m_pChar->bits,true);
			XDrawDib dib1;
			dib1.CreateFrom(&pBmp->m_pChar->bits,true);
			pBmp->LockBitmap(false);//,true);
			TextureInfo info;
			pBmp->GetBmInfo(info);
			//m_pChar->frame->Draw(info,pMatrix,pForm,rc,bSmooth);
			dib.Draw(dib1,
					 pMatrix,pForm,rc,bSmooth);
		}
	}
	else
	{
		SObject*pObj=list[0].pObject->m_pObject;


		LockBitmap(true);
		if(IsEmptyBits()) return;
		SBitmapCore* bm=&m_pChar->bits;
		//bm->preAlpha = true;
		int w=bm->width,h=bm->height;
		FrameTexture*pFrame=FrameTexture::GetFrameTexture(w,h);//pDst->m_pChar->frame;
		if(!pFrame)
			return;
		//RasterGL::sharedRasterGL()->UpdateTexture(pFrame->mTexture->id,bm->baseAddr,Texture2DPixelFormat_RGBA8888,0,0,w,h);
		//if(m_pObject->character->tag==288)
		//	int v=0;
		//if(pDib->Width()==83&&pDib->Height()==24)
		//	int v=0;

		SRECT rect,*clipRect=NULL;

		ScriptPlayer*player=m_pObject->thread?m_pObject->thread->player:NULL;
		if(player)
			player->doCommand(false);
		XBOOL bVisible=m_pObject->IsVisible();
		m_pObject->SetVisible(XTRUE);
		
		if(!rc.IsRectEmpty())
		{
			//AS3RECTANGLEDATA&data=clip->GetData();
			rect.xmin=rc.left*onePoint;
			rect.ymin=rc.top*onePoint;
			rect.xmax=rc.right*onePoint;
			rect.ymax=rc.right*onePoint;
			clipRect=&rect;
		}
		MATRIX cMat,invMat;
		XSWFPlayer::m_pInstance->display.GetCameraMatrix(cMat);
		::MatrixInvert(&cMat,&invMat);
		/*STransform x;
		if(pMatrix)
			pMatrix->GetMatrix(&x.mat,true);
		else
			::MatrixIdentity(&x.mat);
		if(pTransform)
			pTransform->GetCXForm(x.cxform);
		else
			x.cxform.Clear();
		x.cxform.nBlendMode = mode;*/
		//MATRIX mat;
		//::MatrixIdentity(&mat);
		::MatrixConcat(&x.mat,&invMat,&x.mat);
		//::MatrixConcat(&x.mat,&disp->camera.mat,&x.mat);
		//x.mat.a/=20;
		//x.mat.d/=20;
		//pDib->Reset();//Clear(XRect(rect.xmin,rect.ymin,rect.xmax,rect.ymax),0xff000000);
		if(pFrame->mHeight>=bm->height)
		{
			//pFrame->clear();
			RasterGL::sharedRasterGL()->UpdateTexture(pFrame->mTexture->id,bm->baseAddr,Texture2DPixelFormat_RGBA8888,0,0,w,h);
			pFrame->beginPaint(true,true);
			//SRECT* old=pObj->scrollRect;
			//pObj->scrollRect = clipRect;
			pObj->Modify();
			pObj->CalcDrawUpdate(&x.mat,NULL,XFALSE);
			if(clipRect==NULL)
			//{
				pObj->Draw(RasterGL::sharedRasterGL(),x,true,NULL);
			//}
			else
			{
				ClipMask mask;
				ClipMask* pMask = &mask;
				pMask = new ClipMask();
				pMask->beginPaint();
				pMask->setInverted( true );
				pMask->setupClip();
				float color[]={1,1,1,1};
				//SRECT rr={0,RectWidth(clipRect),0,RectHeight(clipRect)};
				//MATRIX idMat;
				//::MatrixIdentity(&idMat);
				RasterGL::sharedRasterGL()->DrawRectangle(*clipRect,invMat,color);//DrawShapeList(clipData,true);
				pMask->setupDraw();
				pObj->Draw(RasterGL::sharedRasterGL(),x,true,NULL);
				pMask->endPaint();
				pFrame->endPaint();
				
			}
			pFrame->GetPixels(0,0,w,h,(XU8*)bm->baseAddr,false);
			//pObj->scrollRect=old;
		}
		else
		{
			MATRIX mat=x.mat;
			MATRIX tMat;
			::MatrixIdentity(&tMat);
			//::MatrixIdentity(&idMat);
			int dy;
			int cw=bm->width;
			
			for(dy=0;dy<bm->height;dy+=pFrame->mHeight)
			{
				//for(dx=0;dx<bm->height;dx+=pFrame->mWidth)
				{
					//tMat.tx = -dx*20;
					tMat.ty = -dy*20;
					::MatrixConcat(&mat,&tMat,&x.mat);
					//int cw=pFrame->mWidth;
					int ch=pFrame->mHeight;
					//if(dx+cw>bm->width) cw=bm->width-dx;
					if(dy+ch>bm->height) ch=bm->height-dy;
					SRECT cr={0,cw,0,ch};
					XU8* dst=(XU8*)bm->baseAddr+bm->rowBytes*dy;//+dx*4;
					RasterGL::sharedRasterGL()->UpdateTexture(pFrame->mTexture->id,dst,Texture2DPixelFormat_RGBA8888,0,0,cw,ch);
					pFrame->beginPaint(true,true);
					pObj->Modify();
					pObj->CalcDrawUpdate(&x.mat,NULL,XFALSE);
					if(clipRect)
					{
						SRECT clip={clipRect->xmin/20,clipRect->xmax/20,clipRect->xmin/20+tMat.ty/20,clipRect->xmax+tMat.ty/20};
						::RectIntersect(&cr,&clip,&cr);
						if(RectIsEmpty(&cr)) {pFrame->endPaint();continue;}

						ClipMask mask;
						ClipMask* pMask = &mask;
						pMask = new ClipMask();
						pMask->beginPaint();
						pMask->setInverted( true );
						pMask->setupClip();
						float color[]={1,1,1,1};
						//SRECT rr={0,RectWidth(clipRect),0,RectHeight(clipRect)};
						//MATRIX mat;
						//::MatrixIdentity(&mat);
						RasterGL::sharedRasterGL()->DrawRectangle(cr,invMat,color);//DrawShapeList(clipData,true);
						pMask->setupDraw();
						pObj->Draw(RasterGL::sharedRasterGL(),x,true,NULL);
					}
					else
					{
						pObj->Draw(RasterGL::sharedRasterGL(),x,true,NULL);
					}
					pFrame->endPaint();					
					pFrame->GetPixels(0,0,cw,ch,dst,false);
				}
			}
		}
			//pObj->scrollRect=old;
			//pObj->FreeCache();
		::FreeCache(pObj);
		m_pObject->SetVisible(bVisible);
		pObj->Modify();
		//MATRIX mat;
		::MatrixIdentity(&cMat);
		pObj->GetTotalMatrix(cMat,XTRUE);
		pObj->CalcDrawUpdate(&cMat,NULL,XFALSE);
		
		m_bModify = true;
		//SetModify();
		//FrameTexture*pFrame=m_pChar->frame;
		//pFrame->beginPaint(true);
		//pObj->CalcDrawUpdate(&x.mat,NULL,XFALSE);
		//pObj->Draw(RasterGL::sharedRasterGL(),x,true,NULL);
		//pFrame->endPaint();

		//XRaster raster;
		//XBitBuffer bits;
		//bits.bitmap=m_pDib;
		//SRECT rect={rc.left,rc.right,rc.top,rc.bottom};

		//DisplayList*disp=m_pRoot->display;
		//disp->pRaster=&raster;
		//::MatrixConcat(&x.mat,&disp->camera.mat,&x.mat);
		//raster.Attach(&bits,&rect,0,1);
		//raster.BeginPaint();
		//pObj->CalcDrawUpdate(&x.mat,NULL,XFALSE);
   		//pObj->DrawDraw(&raster,x,XNULL,XNULL,XFALSE,0);
		
		//raster.PaintBits();
		//pObj->FreeCache();
		//::FreeCache(pObj);
		//bits.bitmap=XNULL;
		//pObj->Modify();
		//disp->pRaster=&disp->raster;
	}
	/*if(nSize<=0||!list[0].IsObject(XXOBJ_BITMAP)) return;
	XXObjectBitmap*pBmp=(XXObjectBitmap*)list[0].pObject;
	if(pBmp->m_pDib==XNULL) return;
	MATRIX *pMatrix=XNULL;
	XCXForm *pForm=XNULL;
	XRect rc;
	if(nSize>=2&&list[1].IsObject(XXOBJ_MATRIX))
	{
		//pMatrix=&matrix;
		pMatrix=&((XXObjectMatrix*)list[1].pObject)->matrix;
	}
	if(nSize>=3&&list[2].IsObject(XXOBJ_COLORTRANSFORM))
		pForm=&((XXObjectColorTransform*)list[2].pObject)->cxForm;
	if(nSize>=5&&list[4].IsObject(XXOBJ_RECTANGLE))
	{
		((XXObjectRectangle*)list[4].pObject)->GetRect(rc,XTRUE);
	}
	else 
	{
		rc.left=0;
		rc.right=0;
		rc.right=pBmp->m_pDib->Width()<<ANTIZOOM;
		rc.bottom=pBmp->m_pDib->Height()<<ANTIZOOM;
	}
	m_pDib->Draw(*pBmp->m_pDib,
		pMatrix,pForm,rc,XFALSE); */
	m_bModify = true;
}
void XXObjectBitmap::fillRect(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XNEEDBITMAP
	if(list.GetSize()<2) return;
	if(!list[0].IsObject(XXOBJ_RECTANGLE)) return;
	LockBitmap(true);
	if(IsEmptyBits()) return;
	XRect rect;
	((XXObjectRectangle*)list[0].pObject)->GetRect(rect,XFALSE);
	int nDiv = m_pChar->bits.nImgDiv;
	if(nDiv)
	{
		rect.left>>=nDiv;
		rect.right>>=nDiv;
		rect.top>>=nDiv;
		rect.right>>=nDiv;
	}
	XU32 nColor=list[1].ToInt();
	if(!IsTransparent())
		nColor|=0xff000000;
	//m_pChar->frame->DrawRect(rect.left,rect.top,rect.Width(),rect.Height(),nColor);
	XDrawDib dib;
	dib.CreateFrom(&m_pChar->bits,true);
	dib.FillRect(rect,nColor,XFALSE);
	m_bModify = true;
}
void XXObjectBitmap::floodFill(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XNEEDBITMAP
	if(list.GetSize()<3) return;
	LockBitmap(true);
	int x=list[0].ToInt();
	int y=list[1].ToInt();
	int nDiv = m_pChar->bits.nImgDiv;
	if(nDiv)//m_pChar->frame->nImgDiv)
	{
		x>>=nDiv;
		y>>=nDiv;
	}
	XU32 nColor=list[2].ToInt();
	if(!IsTransparent())
		nColor|=0xff000000;
#ifndef _ARGB_PIXEL_
	SWAPPIXELRB(nColor);
#endif
	//m_pChar->frame->FloodFill(x,y,nColor);
	XDrawDib dib;
	dib.CreateFrom(&m_pChar->bits,true);
	dib.FloodFill(x,y,nColor);
	m_bModify = true;
}
void XXObjectBitmap::generateFilterRect(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XNEEDBITMAPR
	if(list.GetSize()<1||!
		list[0].IsObject(XXOBJ_RECTANGLE)) 
	{
		cnt.pStack->PushConst((XOCT_UNDEFINE));
		return;
	}
	XXObjectRectangle*pr=((XXObjectRectangle*)list[0].pObject)->Clone();
	cnt.pStack->Push((pr));
}
void XXObjectBitmap::getColorBoundsRect(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XNEEDBITMAPR
	LockBitmap(false);
	if(IsEmptyBits()) return;
	//TextureInfo info;
	//GetBmInfo(info);
	XXObjectRectangle*p=m_pRoot->m_pGlobal->CreateRectangle();
	if(p&&list.GetSize()>=2)
	{
		XU32 mask=list[0].ToInt();
		XU32 color=list[1].ToInt();
		if(!IsTransparent())
			color|=0xff000000;
		XBOOL bFind=list.GetSize()>=3?list[2].ToLogic():XTRUE;
		XRect rect;
		XDrawDib dib;
		dib.CreateFrom(&m_pChar->bits,true);
		//m_pChar->frame->GetDib(dib,true);
		//dib.CreateFrom(&m_pChar->bits,true);
		XDrawDib* m_pDib=&dib;
		m_pDib->GetColorBounds(rect,mask,color,bFind);
		int nDiv = m_pChar->bits.nImgDiv;
		if(nDiv)//m_pChar->frame->nImgDiv)
		{
			rect.left<<=nDiv;
			rect.right<<=nDiv;
			rect.top<<=nDiv;
			rect.bottom<<=nDiv;
		}
		p->left=(float)rect.left;
		p->top=(float)rect.top;
		p->width=(float)rect.Width();
		p->height=(float)rect.Height();
	}
	cnt.pStack->Push((p));
	
}
void XXObjectBitmap::getPixel(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XNEEDBITMAPR
	XU32 color=0;
	LockBitmap(false);
	if(list.GetSize()>=2)
	{
		int x=list[0].ToInt();
		int y=list[1].ToInt();
		//color=m_pDib->GetPixel(x,y);
		int nDiv = m_pChar->bits.nImgDiv;
		if(nDiv)//m_pChar->frame->nImgDiv)
		{
			x>>=nDiv;
			y>>=nDiv;
		}
		if(x>=0&&y<=0&&x<m_pChar->bits.width&&y<m_pChar->bits.height)
		{
			RGB8*pRgb=(RGB8*)m_pChar->bits.baseAddr;
			pRgb+=m_pChar->bits.width*y+x;
			//ch->bits.GetRGBPixel8(x,y,&c);
			pRgb->alpha = 0;
			//c.alpha=0;
			color=*((XU32*)pRgb);
#ifndef _ARGB_PIXEL_
			SWAPPIXELRB(color);
#endif		
		}
	}
	cnt.pStack->Push(XXVar(color));
}
void XXObjectBitmap::getPixel32(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	LockBitmap(true);
	XU32 color = 0;
	if(list.GetSize()>=2)
	{
		int x=list[0].ToInt();
		int y=list[1].ToInt();
		//color=m_pDib->GetPixel(x,y);
		int nDiv = m_pChar->bits.nImgDiv;
		if(nDiv)//m_pChar->frame->nImgDiv)
		{
			x>>=nDiv;
			y>>=nDiv;
		}
		if(x>=0&&y<=0&&x<m_pChar->bits.width&&y<m_pChar->bits.height)
		{
			RGB8*pRgb=(RGB8*)m_pChar->bits.baseAddr;
			pRgb+=m_pChar->bits.width*y+x;
			//ch->bits.GetRGBPixel8(x,y,&c);
			//pRgb->alpha = 0;
			//c.alpha=0;
			if(pRgb->alpha)
				color=*((XU32*)pRgb);
#ifndef _ARGB_PIXEL_
			SWAPPIXELRB(color);
#endif		
		}
	}
	cnt.pStack->Push(XXVar(color));
}
void XXObjectBitmap::hitTest(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	LockBitmap(false);
	if(IsEmptyBits()) 
	{
		cnt.pStack->PushBool(XFALSE);
		return;
	}
	XXVar var;//*pVar=XXVar::CreateBool(XFALSE);
	var.ToLogic();
	int nSize=list.GetSize();
//	DisplayList*disp=m_pRoot->display;
	if(nSize>=3&&list[0].IsObject(XXOBJ_POINT)&&list[2].IsObject())
	{
		XPoint pt;
		((XXObjectPoint*)list[0].pObject)->GetPoint(pt);
		TextureInfo info;
		GetBmInfo(info);
		if(info.nDiv)
		{
			pt.x>>=info.nDiv;
			pt.y>>=info.nDiv;
		}
		int alpha=list[1].ToInt();
		if(!IsTransparent()) alpha=0;
		
		switch(list[2].pObject->GetObjectType())
		{
		case XXOBJ_POINT:
			{
				XPoint dpt;
				((XXObjectPoint*)list[2].pObject)->GetPoint(dpt);
				int w=info.w,h=info.h;
				XRect rect(pt.x,pt.y,pt.x+w,pt.y+h);
				if(info.nDiv)
				{
					rect.left>>=info.nDiv;
					rect.right>>=info.nDiv;
					rect.top>>=info.nDiv;
					rect.bottom>>=info.nDiv;
				}
				if(rect.PtInRect(dpt))
				{
					if(!alpha)
						var.iData32=XTRUE;
					else
					{
						RGB8 color;
					//raster->GetPixel(info,dpt.x,dpt.y,&color);
						m_pChar->bits.GetRGBPixel8(dpt.x,dpt.y,&color);
						//m_pChar->frame->GetPixels(dpt.x,dpt.y,1,1,(XU8*)&color,true);
						if(color.alpha>=alpha)
							var.iData32=XTRUE;
					}
					//disp->ToPixel(rect);
					//var.iData32=m_pDib->HitTest(rect,alpha);
				}
			}break;
		case XXOBJ_RECTANGLE:
			{
				XRect rect;
				((XXObjectRectangle*)list[2].pObject)->GetRect(rect,XFALSE);
				if(info.nDiv)
				{
					rect.left>>=info.nDiv;
					rect.right>>=info.nDiv;
					rect.top>>=info.nDiv;
					rect.bottom>>=info.nDiv;
				}
				//disp->ToPixel(rect);
				//disp->ToPixel(pt);
				int w=info.w;
				int h=info.h;
				XRect rt(pt.x,pt.y,pt.x+w,pt.y+h);
				rt&=rect;
				XDrawDib dib;
				dib.CreateFrom(&m_pChar->bits,true);
				if(!rect.IsRectEmpty())
					var.iData32=dib.HitTest(rt,alpha);//m_pChar->frame->HitTest(rt,alpha);
			}break;
		case XXOBJ_BITMAP:
			{
				XXObjectBitmap*pBmp=(XXObjectBitmap*)list[2].pObject;
				if(pBmp==XNULL) break;
				pBmp->LockBitmap(false);
				if(pBmp->IsEmptyBits()) break;
				if(!pBmp->IsTransparent()) alpha=0;
				int alpha2=alpha;
				XPoint spt(0,0);
				if(pBmp->IsTransparent()&&nSize>=5)
					alpha2=list[4].ToInt();
				if(nSize>=4&&list[3].IsObject(XXOBJ_POINT))
					((XXObjectPoint*)list[3].pObject)->GetPoint(spt);
				TextureInfo si;
				pBmp->GetBmInfo(si);
				if(si.nDiv)
				{
					spt.x>>=si.nDiv;
					spt.y>>=si.nDiv;
				}
				//disp->ToPixel(pt);
				//disp->ToPixel(spt);
				int w=info.w;
				int h=info.h;
				int w1=si.w;
				int h1=si.h;
				XRect r1(pt.x,pt.y,pt.x+w,pt.y+h);
				XRect r2(spt.x,spt.y,spt.x+w1,spt.y+h1);
				XRect rr=r1;
				rr&=r2;
				if(!rr.IsRectEmpty())
				{
					if(!alpha||!alpha2) var.iData32=XTRUE;
					else
					{
						if(!alpha)
						{
							rr-=r2;
							XDrawDib dib1;
							dib1.CreateFrom(&pBmp->m_pChar->bits,true);
							var=//pBmp->m_pChar->frame->HitTest(rr,alpha2);
								dib1.HitTest(rr,alpha2);
						}
						else if(!alpha2)
						{
							rr-=r2;
							XDrawDib dib;
							dib.CreateFrom(&m_pChar->bits,true);
							var=//m_pChar->frame->HitTest(rr,alpha);
								dib.HitTest(rr,alpha);
						}
						else
						{
							XDrawDib dib,dib1;
							dib.CreateFrom(&m_pChar->bits,true);
							dib1.CreateFrom(&pBmp->m_pChar->bits,true);
							//m_pChar->frame->GetDib(dib);
							//pBmp->m_pChar->frame->GetDib(dib1);
							XU32*ps1=(XU32*)dib.GetBuffer();
							XU32*ps2=(XU32*)dib1.GetBuffer();
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
									var.iData32=XTRUE;
									cnt.pStack->Push(var);
									//return true;
									//var.iData32=XTRUE;
									//cnt.pStack->Push(var);
									//return;
								}
								ps1+=w;
								ps2+=w1;
							}
							//return false;
							/*XU32*ps1=(XU32*)m_pDib->GetBuffer();
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
									var.iData32=XTRUE;
									cnt.pStack->Push(var);
									return;
								}
								ps1+=w;
								ps2+=w1;
							}*/
						}
					}
				}

				//XRect r1(pt.x,pt.y,w,h);
				//XRect r2(spt.x,spt.y,w1,h1);
				//r1&=r2;
				//if(!r1.IsRectEmpty())
				//var.iData32=m_pDib->HitTest(r1,alpha)&&
				//			  pBmp->m_pDib->HitTest(r1,alpha);
				
			}break;
		}
	}
	cnt.pStack->Push(var);
}

static int nDebugCount=0;

void XXObjectBitmap::loadBitmap(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//if(nDebugCount>=6) return;
	//nDebugCount++;
	ScriptThread*thread=CLIPTHREAD(cnt.baseThread);
	XXObjectBitmap*pBitmap=CreateClass(m_bBase?this:m_pSuper,XFALSE,m_pRoot);
	//pBitmap->m_pDib=XNULL;
	if(pBitmap&&list.GetSize()>0&&thread)
	{
		list[0].ToString(XFALSE);
		//XDTDefineBitmap*pDT=(XDTDefineBitmap*)cnt.pPlayer->Import(list[0].strData);
		SCharacter*p=thread->player->ImportChar(list[0].strTxt);
		if(p&&p->type==bitsChar)//pDT&&pDT->GetClassType()==_objBitmap)
		{
			//threadBuildBits(p);
			XSWFPlayer::m_pInstance->CreateIfEmpty(p);
			//p->bits.CreateTexture();
			//p->bits.ReleaseData();
			p->bits.LockBits();
			if(pBitmap->m_pChar)
				pBitmap->ReleaseChar(pBitmap->m_pChar);
			pBitmap->m_pChar = p;
			/*if(p->bits.HasBits())
			{
				p->bits.LockBits();
				m_bTransparent=p->bits.transparent;
				pBitmap->m_pDib=new XDrawDib;
				if(pBitmap->m_pDib)
					pBitmap->m_pDib->CreateFrom(&p->bits,false);
				p->bits.UnlockBits();
				//p->bits.PIFree();
			}*/
			//pBitmap->m_pDib=pDT;
			//pBitmap->m_bNeedDelete=XFALSE;
		}
		//cnt.GetVariable(str,var);
		//str.EmptyNoFree();
	}
	cnt.pStack->Push((pBitmap));
}
void XXObjectBitmap::merge(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XNEEDBITMAP
	LockBitmap(true);
	if(IsEmptyBits()) return;
	int nSize=list.GetSize();
	if(nSize<7||!list[0].IsObject(XXOBJ_BITMAP)||
	   !list[1].IsObject(XXOBJ_RECTANGLE)||
	   !list[2].IsObject(XXOBJ_POINT)) return;
	XXObjectBitmap*pBmp=(XXObjectBitmap*)list[0].pObject;
	if(!pBmp) return;
	pBmp->LockBitmap(false);
	if(pBmp->IsEmptyBits()) return;
	//if(!pBmp->m_pDib) return;
	XRect rect;
	((XXObjectRectangle*)list[1].pObject)->GetRect(rect,XFALSE);
	XPoint pt;
	((XXObjectPoint*)list[2].pObject)->GetPoint(pt);
	int rm=list[3].ToInt();
	int gm=list[4].ToInt();
	int bm=list[5].ToInt();
	int am=list[6].ToInt();
	TextureInfo si;
	pBmp->GetBmInfo(si);
	int nDiv = m_pChar->bits.nImgDiv;
	if(nDiv)
	{
		pt.x>>=nDiv;
		pt.y>>=nDiv;
	}
	if(si.nDiv)
	{
		rect.left>>=si.nDiv;
		rect.top>>=si.nDiv;
		rect.right>>=si.nDiv;
		rect.bottom>>=si.nDiv;
	}
//	m_pChar->frame->Merge(/*pBmp==this?NULL:pBmp->m_pChar->frame.pBuffer*/si,rect,pt,rm,gm,bm,am);
	XDrawDib dib,dib1;
	dib.CreateFrom(&m_pChar->bits,true);
	dib1.CreateFrom(&pBmp->m_pChar->bits,true);
	dib.Mix(dib1,rect,pt,rm,gm,bm,am);
	m_bModify = true;
}
void XXObjectBitmap::noise(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XNEEDBITMAP
	LockBitmap(true);
	if(IsEmptyBits()) return;
	int nSize=list.GetSize();
	if(nSize<=0) return;
	int nSeed=list[0].ToInt();
	int nLow=nSize>1?list[1].ToInt():0;
	int nHigh=nSize>2?list[2].ToInt():255;
	int nOption=nSize>3?list[3].ToInt():7;
	int nGray=nSize>4?list[4].ToLogic():XFALSE;
	if(!(nOption&0xf)) return;
	XGlobal::Seed(nSeed);
	XDrawDib dib;
	dib.CreateFrom(&m_pChar->bits,true);
	//m_pChar->frame->Noise(nLow,nHigh,nOption,nGray);
	dib.Noise(nLow,nHigh,nOption,nGray);
	m_bModify = true;
	//	RAND_MAX
}

void XXObjectBitmap::paletteMap(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XNEEDBITMAP
	LockBitmap(true);
	if(IsEmptyBits()) return;
	int nSize=list.GetSize();
	if(nSize<3||!list[0].IsObject(XXOBJ_BITMAP)||
		!list[1].IsObject(XXOBJ_RECTANGLE)||
		!list[2].IsObject(XXOBJ_POINT)) return;
	XXObjectBitmap*pBmp=(XXObjectBitmap*)list[0].pObject;
	//if(!pBmp->m_pDib) return;
	XRect rect;
	((XXObjectRectangle*)list[1].pObject)->GetRect(rect,XFALSE);
	XPoint dst;
	((XXObjectPoint*)list[2].pObject)->GetPoint(dst);
	int nDiv = m_pChar->bits.nImgDiv;
	if(nDiv)//m_pChar->frame->nImgDiv)
	{
		dst.x>>=nDiv;
		dst.y>>=nDiv;
	}
	if(nSize<=3)
	{
		if(!pBmp) return;
		//TextureInfo info;
		//pBmp->GetBmInfo(info);
		XDrawDib dib,dib1;
		dib.CreateFrom(&m_pChar->bits,true);
		dib1.CreateFrom(&pBmp->m_pChar->bits,true);
		//m_pChar->frame->Copy(info,dst.x,dst.y,rect);
		dib.Copy(dib1,dst.x,dst.y,rect);
		m_bModify = true;
		return;
	}
	XU8Array reds,blues,greens,alphas;
	if(nSize>3&&list[3].IsObject(XXOBJ_ARRAY))
		((XXObjectArray*)list[3].pObject)->GetData(reds);
	if(nSize>4&&list[4].IsObject(XXOBJ_ARRAY))
		((XXObjectArray*)list[4].pObject)->GetData(greens);
	if(nSize>5&&list[5].IsObject(XXOBJ_ARRAY))
		((XXObjectArray*)list[5].pObject)->GetData(blues);
	if(nSize>6&&list[6].IsObject(XXOBJ_ARRAY))
		((XXObjectArray*)list[6].pObject)->GetData(alphas);

	if(pBmp==this||pBmp==NULL||pBmp->m_pChar==m_pChar) 
	{
		if(nDiv)
		{
			rect.left>>=nDiv;
			rect.right>>=nDiv;
			rect.top>>=nDiv;
			rect.bottom>>=nDiv;
		}
		XDrawDib dib;
		dib.CreateFrom(&m_pChar->bits,true);
		dib.PaletteMap(dib,rect,dst,
				reds,greens,blues,alphas);
	}
	else
	{
		pBmp->LockBitmap(false);
		int nDiv = pBmp->m_pChar->bits.nImgDiv;
		if(nDiv)
		{
			rect.left>>=nDiv;
			rect.right>>=nDiv;
			rect.top>>=nDiv;
			rect.bottom>>=nDiv;
		}
		XDrawDib dib,dib1;
		dib.CreateFrom(&m_pChar->bits,true);
		dib1.CreateFrom(&pBmp->m_pChar->bits,true);
		dib.PaletteMap(dib1,rect,dst,
				reds,greens,blues,alphas);
	}

	m_bModify = true;
	//m_pDib->PaletteMap(*pBmp->m_pDib,rect,dst,reds,greens,blues,alphas);

}
void XXObjectBitmap::perlinNose(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XNEEDBITMAP
	LockBitmap(true);
	if(IsEmptyBits()) return;
	int nSize=list.GetSize();
	if(nSize<6) return;
	int nSeed=list[3].ToInt();
	int nOption=nSize>6?list[6].ToInt():7;
	int nGray=nSize>7?list[7].ToLogic():XFALSE;
	if(!(nOption&0xf)) return;
	XGlobal::Seed(nSeed);
	XDrawDib dib;
	dib.CreateFrom(&m_pChar->bits,true);
	dib.Noise(0,255,nOption,nGray);
	m_bModify = true;
	//m_pDib->Noise(0,255,nOption,nGray);
}
void XXObjectBitmap::pixelDissolve(XSWFCONTEXT&cnt,XXVARLIST&list)
{			   
	//XNEEDBITMAPR
	LockBitmap(true);
	if(IsEmptyBits()) return;
	int nSize=list.GetSize();
	if(nSize<3||!list[0].IsObject(XXOBJ_BITMAP)||
		!list[1].IsObject(XXOBJ_RECTANGLE)||
		!list[2].IsObject(XXOBJ_POINT)) 
	{
		cnt.pStack->PushConst((XOCT_UNDEFINE));
		return;
	}
	XXObjectBitmap*pBmp=(XXObjectBitmap*)list[0].pObject;
	//if(!pBmp->m_pDib) return;
	XRect rect;
	((XXObjectRectangle*)list[1].pObject)->GetRect(rect,XFALSE);
	XPoint dst;
	((XXObjectPoint*)list[2].pObject)->GetPoint(dst);
	int nSeed=nSize>3?list[3].ToInt():0;
	
	XU32 nColor=nSize>5?list[5].ToInt():0;
	if(!IsTransparent())
			nColor|=0xff000000;
#ifndef _ARGB_PIXEL_
	SWAPPIXELRB(nColor);
#endif
	int nDiv = m_pChar->bits.nImgDiv;
	if(nDiv)//m_pChar->frame->nImgDiv)
	{
		dst.x>>=nDiv;
		dst.y>>=nDiv;
		rect.left>>=nDiv;
		rect.right>>=nDiv;
		rect.top>>=nDiv;
		rect.bottom>>=nDiv;
	}
	TextureInfo info;
	GetBmInfo(info);
	XDrawDib dib;
	dib.CreateFrom(&m_pChar->bits,true);
	if(pBmp==this||pBmp==XNULL)
	{
		int nPixels=nSize>4?list[4].ToInt():(info.w*info.h/30);
		nSeed=dib.Dissolve(rect,dst,nSeed,nPixels,nColor);
	}
		//nSeed=m_pDib->Dissolve(rect,dst,nSeed,nPixels,nColor);
	else
	{
		//TextureInfo info;
		//pBmp->GetBmInfo(info);
		if(pBmp==this||pBmp->m_pChar==m_pChar)
		{
			int nPixels=nSize>4?list[4].ToInt():(info.w*info.h/30);
			nSeed=dib.Dissolve(dib,rect,dst,nSeed,nPixels);
		}
		else
		{
			//pBmp->GetBmInfo(info);
			pBmp->LockBitmap(false);
			int nPixels=nSize>4?list[4].ToInt():(info.w*info.h/30);
			XDrawDib dib1;
			dib1.CreateFrom(&pBmp->m_pChar->bits,true);
			nSeed=dib.Dissolve(dib1,rect,dst,nSeed,nPixels);
		}
	}
	cnt.pStack->Push((nSeed));
	m_bModify = true;
}
void XXObjectBitmap::scroll(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XNEEDBITMAP
	if(list.GetSize()<2) return;
	int sx=list[0].ToInt();
	int sy=list[1].ToInt();
	//if(sx==0&&sy==0) return;
	LockBitmap(true);
	if(IsEmptyBits()) return;
	int nDiv = m_pChar->bits.nImgDiv;
	if(nDiv)
	{
		sx>>=nDiv;
		sy>>=nDiv;
	}
	if(sx==0&&sy==0) return;
	XDrawDib dib;
	dib.CreateFrom(&m_pChar->bits,true);
	//m_pChar->frame->Scroll(sx,sy);
	dib.Scroll(sx,sy);
}
void XXObjectBitmap::setPixel(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XNEEDBITMAP
	if(list.GetSize()<3) return;
	int x=list[0].ToInt();
	int y=list[1].ToInt();
	XU32 nColor=list[2].ToInt();
	if(!IsTransparent())
		nColor|=0xff000000;
#ifndef _ARGB_PIXEL_
	SWAPPIXELRB(nColor);
#endif
	//m_pDib->SetPixel(x,y,nColor,XFALSE);
	LockBitmap(true);
	if(!IsEmptyBits())//HasBits())
	{
		int nDiv = m_pChar->bits.nImgDiv;
		if(nDiv)//m_pChar->frame->nImgDiv)
		{
			x>>=nDiv;
			y>>=nDiv;
		}
		XDrawDib dib;
		dib.CreateFrom(&m_pChar->bits,true);
		//dib.SetPixel(x,y,c,XTRUE);
		dib.SetPixel(x,y,nColor,XTRUE);
		m_bModify = true;
	}
}
void XXObjectBitmap::setPixel32(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	if(list.GetSize()<3) return;
	int x=list[0].ToInt();
	int y=list[1].ToInt();
	XU32 nColor=list[2].ToInt();
	LockBitmap(true);//,true);
#ifndef _ARGB_PIXEL_
	SWAPPIXELRB( nColor );
#endif
	if(!IsTransparent())
		nColor|=0xff000000;
	if(!IsEmptyBits())//HasBits())
	{
		int nDiv = m_pChar->bits.nImgDiv;
		if(nDiv)//m_pChar->frame->nImgDiv)
		{
			x>>=nDiv;
			y>>=nDiv;
		}
		XDrawDib dib;
		dib.CreateFrom(&m_pChar->bits,true);
		dib.SetPixel(x,y,nColor,XTRUE);
		m_bModify = true;
		//m_pChar->frame->SetPixel(x,y,nColor,XTRUE);
	}
}
void XXObjectBitmap::threshold(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	LockBitmap(true);
	if(IsEmptyBits()) return;
	int v=0;
	int nSize=list.GetSize();
	if(nSize>=5&&
	   list[0].IsObject(XXOBJ_BITMAP)&&
	   list[1].IsObject(XXOBJ_RECTANGLE)&&
	   list[2].IsObject(XXOBJ_POINT))
	{
		XRect rect;
		XXObjectBitmap*pBmp=(XXObjectBitmap*)list[0].pObject;
		pBmp->LockBitmap(false);
		if(pBmp&&!pBmp->IsEmptyBits())
		{
			((XXObjectRectangle*)list[1].pObject)->GetRect(rect,XFALSE);
			XPoint dst;
			((XXObjectPoint*)list[2].pObject)->GetPoint(dst);
			list[3].ToString(XFALSE);
			XU8 nOpt=0;
			XU32 thre=list[4].ToInt();
			XU32 nColor=nSize>5?list[5].ToInt():0;
			XU32 mask=nSize>6?list[6].ToInt():0xffffffff;
			XU8 bCopy=nSize>7?list[7].ToLogic():XFALSE;
			switch(list[3].strTxt[0])
			{
			case '!':nOpt=5;break;
			case '<':nOpt=list[3].strTxt[1]=='='?1:0;break;
			case '=':nOpt=4;break;
			case '>':nOpt=list[3].strTxt[1]=='='?3:2;break;
			}
			TextureInfo info,si;
			GetBmInfo(info);
			if(info.nDiv)
			{
				dst.x>>=info.nDiv;
				dst.y>>=info.nDiv;
			}
			pBmp->GetBmInfo(si);
			if(si.nDiv)
			{
				rect.left>>=si.nDiv;
				rect.top>>=si.nDiv;
				rect.right>>=si.nDiv;
				rect.bottom>>=si.nDiv;
			}
			 //m_pChar->frame->Threshold(pBmp->m_pChar->frame,rect,dst,nOpt,thre,mask,nColor,bCopy);
			XDrawDib dib,dib1;
			dib.CreateFrom(&m_pChar->bits,true);
			dib1.CreateFrom(&pBmp->m_pChar->bits,true);
			dib.Threshold(dib1,rect,dst,nOpt,thre,mask,nColor,bCopy);
			m_bModify = true;
		}
	}
	cnt.pStack->Push((v));
}




XXObject* XXObjectBitmap::CreateObject(XSWFCONTEXT *pCnt, XXVARLIST &list,XBOOL)
{
	XXObjectBitmap*pObj=CreateClass(this,XFALSE,m_pRoot);
	if(pObj)
	{
		pObj->m_pSuperObj=GetSuperObject();
		pObj->Construct(pCnt,list,this);
	}
	else
		pCnt->pStack->Push((pObj));	
    return pObj;
	return NULL;
}

void XXObjectBitmap::CopyPixel(XXVARLIST &list)
{
	LockBitmap(true);
	if(IsEmptyBits()) return;
	
	int nSize=list.GetSize();
	XXObjectBitmap*pSrt=XNULL;
	XRect rect;
	XPoint dst,adst;
	//XXObjectRectangle*pRect=XNULL;
	//XXObjectPoint*pDst=XNULL;
	XXObjectBitmap*pAlpha=XNULL;
	//XXObjectPoint*pDstA=XNULL;
	if(nSize>=3)
	{
		if(list[0].IsObject(XXOBJ_BITMAP)&&
		   list[1].IsObject(XXOBJ_RECTANGLE)&&
		   list[2].IsObject(XXOBJ_POINT))
		{
			pSrt=(XXObjectBitmap*)list[0].pObject;
			XXObjectRectangle*pRect=(XXObjectRectangle*)list[1].pObject;
			XXObjectPoint*pDst=(XXObjectPoint*)list[2].pObject;
			pRect->GetRect(rect,XFALSE);
			pDst->GetPoint(dst);
		}
	}
	if(!pSrt) return;
	pSrt->LockBitmap(false);
	if(pSrt->IsEmptyBits()) return;
	if(nSize>=5)
	{
	   if(list[3].IsObject(XXOBJ_BITMAP)&&
		  list[4].IsObject(XXOBJ_POINT))
	   {
		   pAlpha=(XXObjectBitmap*)list[3].pObject;
		   XXObjectPoint*pDstA=(XXObjectPoint*)list[4].pObject;
		   pDstA->GetPoint(adst);
	   }
	}
	TextureInfo info,si;
	GetBmInfo(info);
	if(info.nDiv)
	{
		dst.x>>=info.nDiv;
		dst.y>>=info.nDiv;
	}
	pSrt->GetBmInfo(si);
	if(si.nDiv)
	{
		rect.left>>=si.nDiv;
		rect.top>>=si.nDiv;
		rect.right>>=si.nDiv;
		rect.bottom>>=si.nDiv;
	}
	XDrawDib dib,dib1;
	dib.CreateFrom(&m_pChar->bits,true);
	dib1.CreateFrom(&pSrt->m_pChar->bits,true);
	if(pAlpha)//&&!pAlpha->IsEmptyBits())
	{
		pAlpha->LockBitmap(false);
		XDrawDib diba;
		diba.CreateFrom(&pAlpha->m_pChar->bits,true);
		int nDiv = 1;
		TextureInfo ai;
		pAlpha->GetBmInfo(ai);
		if(ai.nDiv)
			nDiv = 2<<ai.nDiv;
		//m_pChar->frame->Copy(si,dst.x,dst.y,rect,ai,adst.x/nDiv,adst.y/nDiv);
		dib.Copy(dib1,dst.x,dst.y,
							  rect,
							  diba,
							  adst.x,adst.y);
							  
	}
	else
	{
		//m_pChar->frame->Copy(si,dst.x,dst.y,rect);
		dib.Copy(dib1, dst.x,dst.y, rect);
	}
	m_bModify = true;
}

XU32 XXObjectBitmap::Construct(XSWFCONTEXT *pCnt, XXVARLIST &list, XXObject *)
{
	int nSize=list.GetSize();
	if(nSize>=2)
	{
		int w=list[0].ToInt();
		int h=list[1].ToInt();
		int w0=w,h0=h;
		int nDiv = 0;
		while(w>MAXTEXTURE&&h>MAXTEXTURE)
		{
			nDiv++;
			w>>=1;
			h>>=1;
		}
		if(w&&h)
		{
			m_pChar = XSWFPlayer::m_pInstance->CreateCharacter();
			//m_pChar->frame = new FrameTexture();
			bool transparent=false;
			if(nSize>2)
				transparent=list[2].ToLogic();
			m_pChar->bits.PICreate(w,h,0);
			m_pChar->bits.transparent = transparent;
			//m_pChar->frame->init(w,h,transparent);
			m_pChar->type = bitmapEmptyChar;
			{
				_XSCharacter* ch = m_pChar;
				ch->rbounds.xmin = 0;
				ch->rbounds.ymin = 0;
				ch->rbounds.xmax = w0*20;
				ch->rbounds.ymax = h0*20;
			}
			
			XU32 fillColor = 0;
			if(nSize>3)
				fillColor = list[3].ToInt();
			if(!transparent)
					fillColor|=0xff000000;
			//SRECT rect={0,0,w,h};
#ifndef _ARGB_PIXEL_
			SWAPPIXELRB(fillColor);
#endif
			XDrawDib dib;
			dib.CreateFrom(&m_pChar->bits,true);
			dib.Clear(fillColor);//(0,0,w,h,fillColor);
		}
	}			  
	pCnt->pStack->Push(this);
	return (XU32)this;
}


void XXObjectBitmap::CreateTexture()
{
	if(!m_pChar) return;
	//RestoreIfNeed();
	SBitmapCore* bm=&m_pChar->bits;
	if(bm->bitmapID)
	{
		if(!m_bModify) return;
		bm->CreateTexture();
	}
	else
	{
		if(bm->HasBits())
			bm->CreateTexture();
		else
		{
			if(m_pChar->data)
				XSWFPlayer::m_pInstance->CreateIfEmpty(m_pChar,true);
		}
	}
	m_bModify = false;
}

void XXObjectBitmap::LockBitmap(bool forWrite)
{
	//RestoreIfNeed();
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
	//XSWFPlayer* player = XSWFPlayer::m_pInstance;
	//if(!m_pChar) return;
	////BitmapCore* bits = &m_pChar->bits;
	////m_pChar->bits.m_nLockTime = player->m_nRunTime;
	///*if(!m_bAttach&&HasBits())
	//{
	//	
	//	return;
	//}*/
	////XSWFPlayer*player=XSWFPlayer::m_pInstance;
	//RasterGL* raster=RasterGL::sharedRasterGL();
	//if(m_pChar->type!=bitmapEmptyChar) //return;
	//{

	//	SBitmapCore*bits = &m_pChar->bits;
	//	//m_pChar->bits.m_nLockTime = player->m_nRunTime;
	//	if(!bits->bitmapID)
	//	{
	//		if(m_pChar->data)
	//			player->CreateIfEmpty(m_pChar);
	//		//if(!bits->bitmapID&&bits->HasBits())
	//		//	bits->CreateTexture();
	//		//bits->ReleaseData();
	//		//m_pChar->bits.m_nLockTime = player->m_nRunTime;
	//	}
	//}
	//if(forWrite&&m_bAttach)
	//{
	//	//if(m_bAttach)
	//	{
	//		if(m_pChar->type==bitmapEmptyChar)
	//		{
	//			if(m_pChar->frame->mTexture->nUseCount==1)//useCount==1)
	//			{
	//				m_bAttach = false;
	//				return;
	//			}
	//		}
	//		SCharacter*ch = player->CreateCharacter();
	//		//ch->bits.PIInit();
	//		ch->type = bitmapEmptyChar;
	//		ch->record.Init();
	//		//int w,h,id;
	//		TextureInfo info;
	//		GetBmInfo(info);
	//		//ch->frame.w=info.w;
	//		//ch->frame.h=info.h;
	//		ch->frame = new FrameTexture();
	//		ch->frame->nImgDiv = info.nDiv;
	//		//ch->frame.pBuffer->transparent = info.transparent;
	//		ch->frame->init(info.w,info.h,info.transparent);
	//		{
	//			ch->rbounds.xmin = 0;
	//			ch->rbounds.ymin = 0;
	//			ch->rbounds.xmax = (info.w*20)<<info.nDiv;
	//			ch->rbounds.ymax = (info.h*20)<<info.nDiv;
	//		}
	//		if(m_pChar->type==bitmapEmptyChar)
	//		{
	//			ch->frame->CopyTexture(m_pChar->frame);
	//			//ch->frame->nImgDiv=m_pChar->frame->nImgDiv;
	//		}
	//		else
	//		{
	//			ch->frame->DrawBitmap(info);
	//			//ch->frame->nImgDiv=m_pChar->bits.nImgDiv;
	//		}
	//		
	//		ReleaseChar(m_pChar);
	//		m_pChar = ch;
	//		m_bAttach = false;
	//		//AddBitmap();
	//	}
	//	/*else
	//	{
	//		if(!m_pChar->bits.HasBits())
	//		{
	//			if(m_pChar->data)
	//				player->CreateIfEmpty(m_pChar);
	//			else if(m_pChar->bits.bitmapID)
	//			{
	//				m_pChar->bits.PICreate();
	//				raster->ReadTexture(&m_pChar->bits,m_pChar->bits.bitmapID);
	//			}
	//			m_pChar->record.ReleaseAll();
	//			if(m_pChar->bits.bitmapID)
	//			{
	//				raster->DeleteTexture(m_pChar->bits.bitmapID);
	//				m_pChar->bits.bitmapID = 0;
	//			}
	//		}
	//	}*/
	//	if(m_pObject)
	//		m_pObject->Modify();
	//	//FormatBitmap();
	//}
	////if(fmt)
	////	FormatBitmap();
}
