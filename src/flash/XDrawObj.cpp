// XDrawObj.cpp: implementation of the XDrawObj class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XDrawObj.h"
#include "XXObjectBitmap1.h"
#include "BwShapeToGL.h"
#include "AS3BitmapData.h"
#include "avm2.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const int _fixedSqrt2d=0xb505;
const int _fixedDiv8  =0x61f8;

XDrawObj::XDrawObj(ScriptPlayer* p, BwShapeRecord* m):
SCharacterParser(p,XNULL,0,m)
{
}

XDrawObj::~XDrawObj()
{

}



void XDrawObj::BwAddEdge( BwPath &path, int cx, int cy, int x, int y,bool isLine )
{
	XPOINT pt;
	XPOINT ptcon;
	pt.x = x;
	pt.y = y;
	ptcon.x = cx;
	ptcon.y = cy;
//	MatrixTransformPoint( &mat, &pt, &pt );
//	MatrixTransformPoint( &mat, &ptcon, &ptcon );
	BwEdge* edge=ShapeAlloc::GetInst()->AllocEdge();
	edge->mEndPt = pt;
	edge->mControlPt = ptcon;
	edge->mbLine = isLine;
	path.AddEdge( edge );
	return;
}


#define PTEQ(P1,P2) (P1.x=P2.x&&P1.y==P2.y)
#define PTNEQ(P1,P2) (P1.x!=P2.x&&P1.y!=P2.y)

void XDrawObj::BuildEdges(_XCOMMAND*cmds)
{
	//return;
	pShapeRec->ReleaseAll();
	pShapeRec->AddFillStyle(NULL);
	pShapeRec->AddLineStyle(NULL);


	int iCount = 0;
	int fillStyle = 0;
	int lineStyle = 0;
	thick=1;
	ShapeAlloc*pAlloc=ShapeAlloc::GetInst();
	BwPath*path = pAlloc->AllocPath();
	BwFillStyle* pFill;
	BwLineStyle* pLine;
	mPtEnd.x=0;
	mPtEnd.y=0;
	//BwPath&path=*pPath;
	while(cmds)
	{
		switch(cmds->nCommand)
		{
		case DC_ENDFILL:
			if(fillStyle)//mbHasFillStyle)
			{
				if( !path->EdgeEmpty() )
				{

					if ( path->mStartPt.x < -214748)
					{
						int i = 0;
					}
					if( PTNEQ(path->mStartPt,mPtEnd))//path.StartPt() != mPtEnd.x )
					{
						BwEdge* edge=pAlloc->AllocEdge();
						edge->mEndPt = mPtEnd;
						edge->mControlPt = mPtEnd;
						path->AddEdge( edge );
					}
					pShapeRec->AddPath( path );

					if ( path->mStartPt.x < -214748)
					{
						int i = 0;
					}

					path = pAlloc->AllocPath();
					//path.ClearEdges();
				}
				fillStyle = 0;
				path->LineStyle(lineStyle);
				path->FillRight(fillStyle);
				//mbHasFillStyle = false;
			}
			break;
		case DC_BEGINFILLBITMAP_EX:
		case DC_BEGINFILLGRAD:
		case DC_BEGINFILLBITMAP:
		case DC_BEGINFILL:
			//mbHasFillStyle = true;
			
			if( ! path->EdgeEmpty() )
			{
				if(PTNEQ(path->mStartPt,mPtEnd)&&fillStyle)// path.StartPt() != mPtEnd )
				{
					BwEdge* edge = pAlloc->AllocEdge();
					edge->mEndPt = mPtEnd;
					edge->mControlPt = mPtEnd;
					path->AddEdge( edge );
				}
				pShapeRec->AddPath( path );
				path = pAlloc->AllocPath();//path.ClearEdges();
				
			}
			pFill = GetColor(cmds,cmds->nCommand);
			fillStyle = pShapeRec->mnFills;
			pShapeRec->AddFillStyle(pFill);
			path->LineStyle(lineStyle);
			path->FillRight(fillStyle);
			//color1=GetColor(cmds,layer+depth,cmds->nCommand);
			//if(!color1) 
			//	break;
			//depth++;
			break;

		case DC_MOVETO:
			{
				if( !path->EdgeEmpty() )
				{
					if( PTNEQ(path->mStartPt,mPtEnd)&&fillStyle)//path.StartPt() != mPtEnd )
					{
						BwEdge* edge=pAlloc->AllocEdge();
						edge->mEndPt = mPtEnd;
						edge->mControlPt = mPtEnd;
						path->AddEdge( edge );
					}
					pShapeRec->AddPath( path );
					path = pAlloc->AllocPath();
					path->LineStyle(lineStyle);
					path->FillRight(fillStyle);
					//path.ClearEdges();
				}

				//if( fillStyle&&mbLineBegin)//mbHasFillStyle && mbLineBegin )
				{
					CURVE c=*cmds->pCurve;
					//::MatrixTransformPoint(&mat,&c.anchor1,&c.anchor1);
					path->mStartPt.x = c.anchor2.x;
					path->mStartPt.y = c.anchor2.y;
					//BwEdge* edge=pAlloc->AllocEdge();
					////::MatrixTransformPoint(&mat,&c.anchor2,&c.anchor2);
					//edge->mEndPt.x = c.anchor2.x;
					//edge->mEndPt.y = c.anchor2.y;
					//edge->mbLine = true;
					////::MatrixTransformPoint(&mat,&c.control,&c.control);
					//edge->mControlPt.x = c.control.x;
					//edge->mControlPt.y = c.control.y;
					//path->AddEdge( edge );
					//AddCurve(&c);
					mPtEnd=c.anchor2;
					if ( mPtEnd.x < -214748)
					{
						//int i = 0;
						mPtEnd.x = 0;
						mPtEnd.y = 0;

					}
				}
			}break;
		case DC_LINESTYLE:
		case DC_LINEGRADIENTSTYLE:
			 pLine = this->GetLineStyle(cmds,cmds->nCommand);
			 lineStyle = pShapeRec->mnLines;
			 pShapeRec->AddLineStyle(pLine);
			
			 if( ! path->EdgeEmpty() )
			 {
				if(PTNEQ(path->mStartPt,mPtEnd)&&fillStyle)// path.StartPt() != mPtEnd )
				{
					BwEdge* edge = pAlloc->AllocEdge();
					edge->mEndPt = mPtEnd;
					edge->mControlPt = mPtEnd;
					path->AddEdge( edge );
				}
				pShapeRec->AddPath( path );
				path = pAlloc->AllocPath();//path.ClearEdges()
			 }
			 path->LineStyle( lineStyle );
			 path->FillRight( fillStyle );
			break;
		case DC_RECT:
			{
				if( !path->EdgeEmpty() )
				{
					if( PTNEQ(path->mStartPt,mPtEnd)&&fillStyle)//path.StartPt() != mPtEnd )
					{
						BwEdge* edge=pAlloc->AllocEdge();
						edge->mEndPt = mPtEnd;
						edge->mControlPt = mPtEnd;
						path->AddEdge( edge );
					}
					pShapeRec->AddPath( path );
					path = pAlloc->AllocPath();
					path->LineStyle(lineStyle);
					path->FillRight(fillStyle);
					//path.ClearEdges();
				}
				int x1=cmds->pCurve->anchor1.x;
				int y1=cmds->pCurve->anchor1.y;
				int x2=cmds->pCurve->control.x+x1;
				int y2=cmds->pCurve->control.y+y1;

				//if((x2-x1)/20==520)
				//	int v=0;

				XPOINT pt;
				pt.x = x1;
				pt.y = y1;
				//MatrixTransformPoint(&mat,&pt, &pt );
				path->StartPt( pt );
				BwEdge* edge=pAlloc->AllocEdge();
				pt.x = x2;
				pt.y = y1;
				edge->mbLine = true;
				//MatrixTransformPoint(&mat,&pt, &pt );
				edge->mEndPt = pt;
				edge->mControlPt = pt;
				path->AddEdge( edge );

				pt.x = x2;
				pt.y = y2;
				edge = pAlloc->AllocEdge();
				//MatrixTransformPoint(&mat,&pt, &pt );
				edge->mbLine = true;
				edge->mEndPt = pt;
				edge->mControlPt = pt;
				path->AddEdge( edge );

				pt.x = x1;
				pt.y = y2;
				edge = pAlloc->AllocEdge();
				edge->mbLine = true;
				//MatrixTransformPoint(&mat,&pt, &pt );
				edge->mEndPt = pt;
				edge->mControlPt = pt;
				path->AddEdge( edge );

				pt.x = x1;
				pt.y = y1;
				edge = pAlloc->AllocEdge();
				edge->mbLine = true;
				//MatrixTransformPoint(&mat,&pt, &pt );
				edge->mEndPt = pt;
				edge->mControlPt = pt;
				path->AddEdge( edge );

				//pShapeRec->AddPath( path );
				//path = pAlloc->AllocPath();
				mPtEnd=pt;
				if ( mPtEnd.x < -214748)
				{
					mPtEnd.x = 0;
					mPtEnd.y = 0;
				}
				//path.ClearEdges();
			};
			break;
		case DC_ROUNDRECT:
			{
				if( !path->EdgeEmpty() )
				{
					if( PTNEQ(path->mStartPt,mPtEnd)&&fillStyle)//path.StartPt() != mPtEnd )
					{
						BwEdge* edge=pAlloc->AllocEdge();
						edge->mEndPt = mPtEnd;
						edge->mControlPt = mPtEnd;
						path->AddEdge( edge );
					}
					pShapeRec->AddPath( path );
					path = pAlloc->AllocPath();
					path->LineStyle(lineStyle);
					path->FillRight(fillStyle);
					//path.ClearEdges();
				}
				int cx=cmds->pCurve->anchor1.x;
				int cy=cmds->pCurve->anchor1.y;				
				int rx=cmds->pCurve->anchor2.x/2;
				int ry=cmds->pCurve->anchor2.y/2;
				int w=cmds->pCurve->control.x/2;
				int h=cmds->pCurve->control.y/2;
				w -= rx;
				h -= ry;
				cx=cx+rx+w;
				cy=cy+ry+h;
				int ddx=FixedMul(rx,_fixedDiv8);
				int dmx=FixedMul(rx,_fixedSqrt2d);
				int ddy=FixedMul(ry,_fixedDiv8);
				int dmy=FixedMul(ry,_fixedSqrt2d);
				//BwEdge* edge=pAlloc->AllocEdge();
				XPOINT pt;
				pt.x = cx + w + rx;
				pt.y = cy + h;
				//MatrixTransformPoint(&mat,&pt,&pt);
				path->StartPt( pt );
				BwAddEdge( *path, cx + w + rx, cy + ddy + h, cx + w + dmx, cy + h + dmy,false );
				BwAddEdge( *path, cx+w+ddx, cy+h+ry, cx+w, cy+h+ry,false );
				if(w>0)
				{
					BwAddEdge( *path, cx-w,cy+h+ry, cx-w,cy+h+ry,true );
				}
				BwAddEdge( *path, cx-ddx-w,cy+ry+h,cx-dmx-w,cy+dmy+h,false );
				BwAddEdge( *path, cx-rx-w,cy+ddy+h,cx-rx-w,cy+h,false );

				if(h>0)
				{
					BwAddEdge( *path, cx-w-rx,cy-h,cx-w-rx,cy-h,true );
				}
				BwAddEdge( *path, cx-rx-w,cy-ddy-h,cx-dmx-w,cy-dmy-h,false );
				BwAddEdge( *path, cx-ddx-w,cy-ry-h,cx-w,cy-ry-h,false );
				if(w>0)
				{
					BwAddEdge( *path, cx+w,cy-h-ry, cx+w,cy-h-ry,true);
				}
				BwAddEdge( *path, cx+ddx+w,cy-ry-h,cx+dmx+w,cy-dmy-h,false );
				BwAddEdge( *path, cx+rx+w,cy-ddy-h,cx+rx+w,cy-h,false );
				if(h>0)
				{
					BwAddEdge( *path, cx+w+rx,cy+h,cx+w+rx,cy+h,true );
				}
				//pShapeRec->AddPath( path );
				//path = pAlloc->AllocPath();
				mPtEnd=pt;
				if ( mPtEnd.x < -214748)
				{
					mPtEnd.x = 0;
					mPtEnd.y = 0;
				}
				//path.ClearEdges();
			}
			break;
		case DC_ELLIPSE:
			{
				if( !path->EdgeEmpty() )
				{
					if(PTNEQ(path->mStartPt,mPtEnd)&&fillStyle)// path.StartPt() != mPtEnd )
					{
						BwEdge* edge=pAlloc->AllocEdge();
						edge->mEndPt = mPtEnd;
						edge->mControlPt = mPtEnd;
						path->AddEdge( edge );
					}
					pShapeRec->AddPath( path );
					path = pAlloc->AllocPath();
					path->LineStyle(lineStyle);
					path->FillRight(fillStyle);
					//path.ClearEdges();
				}
				int cx=cmds->pCurve->anchor1.x;
				int cy=cmds->pCurve->anchor1.y;
				int rx=cmds->pCurve->control.x/2;
				int ry=cmds->pCurve->control.y/2;
				cx=cx+rx;
				cy=cy+ry;
				int ddx=FixedMul(rx,_fixedDiv8);
				int dmx=FixedMul(rx,_fixedSqrt2d);
				int ddy=FixedMul(ry,_fixedDiv8);
				int dmy=FixedMul(ry,_fixedSqrt2d);
				CURVE c;
				c.isLine=false;
				c.anchor2.x= cx+rx;
				c.anchor2.y=cy;
				XPOINT pt;
				pt.x = cx + rx;
				pt.y = cy;
				//MatrixTransformPoint(&mat,&pt,&pt);
				path->StartPt( pt );
				BwAddEdge(*path,cx+rx,cy+ddy,cx+dmx,cy+dmy,false);
				BwAddEdge(*path,cx+ddx,cy+ry,cx,cy+ry,false);
				BwAddEdge(*path,cx-ddx,cy+ry,cx-dmx,cy+dmy,false);
				BwAddEdge(*path,cx-rx,cy+ddy,cx-rx,cy,false);
				BwAddEdge(*path,cx-rx,cy-ddy,cx-dmx,cy-dmy,false);
				BwAddEdge(*path,cx-ddx,cy-ry,cx,cy-ry,false);
				BwAddEdge(*path,cx+ddx,cy-ry,cx+dmx,cy-dmy,false);
				BwAddEdge(*path,cx+rx,cy-ddy,cx+rx,cy,false);
				//pShapeRec->AddPath( path );
				//path.ClearEdges();
				//path = pAlloc->AllocPath();
				mPtEnd=pt;
				if ( mPtEnd.x < -214748)
				{
					mPtEnd.x = 0;
					mPtEnd.y = 0;
				}
				//mbLineBegin=false;
			}
			break;
		case DC_CIRCLE:
			{
				if( !path->EdgeEmpty() )
				{
					if(PTNEQ(path->mStartPt,mPtEnd)&&fillStyle)// path.StartPt() != mPtEnd )
					{
						BwEdge* edge=pAlloc->AllocEdge();
						edge->mEndPt = mPtEnd;
						edge->mControlPt = mPtEnd;
						path->AddEdge( edge );
					}
					pShapeRec->AddPath( path );
					if ( path->mStartPt.x < -214748)
					{
						int i = 0;
					}
					path = pAlloc->AllocPath();
					path->LineStyle(lineStyle);
					path->FillRight(fillStyle);
					//path.ClearEdges();
				}
				int cx=cmds->pCurve->anchor1.x;
				int cy=cmds->pCurve->anchor1.y;
				int r=cmds->pCurve->control.x;
				//if(cx<-3000||cy<-3000||r<-3000)
				//	int v=0;
				int dd=FixedMul(r,_fixedDiv8);
				int dm=FixedMul(r,_fixedSqrt2d);
				CURVE c;
				c.isLine=false;
				c.anchor2.x=cx+r;
				c.anchor2.y=cy;
				XPOINT pt;
				pt.x = cx + r;
				pt.y = cy;
				path->StartPt( pt );
				BwAddEdge(*path,cx+r,cy+dd,cx+dm,cy+dm,false);
				BwAddEdge(*path,cx+dd,cy+r,cx,cy+r,false);
				BwAddEdge(*path,cx-dd,cy+r,cx-dm,cy+dm,false);
				BwAddEdge(*path,cx-r,cy+dd,cx-r,cy,false);
				BwAddEdge(*path,cx-r,cy-dd,cx-dm,cy-dm,false);
				BwAddEdge(*path,cx-dd,cy-r,cx,cy-r,false);
				BwAddEdge(*path,cx+dd,cy-r,cx+dm,cy-dm,false);
				BwAddEdge(*path,cx+r,cy-dd,cx+r,cy,false);
				mPtEnd=pt;
				if ( mPtEnd.x < -214748)
				{
					mPtEnd.x = 0;
					mPtEnd.y = 0;
				}
			}
			break;
		case DC_CURVE:
			if( path->IsStartPtEmpty() )
			{
				path->StartPt( mPtEnd );
			}
			CURVE c=*cmds->pCurve;
			BwEdge* edge=pAlloc->AllocEdge();
			//MatrixTransformPoint(&mat,&c.anchor2,&c.anchor2);
			//MatrixTransformPoint(&mat,&c.control,&c.control);
			edge->mControlPt = c.control;
			edge->mEndPt = c.anchor2;
			edge->mbLine = c.isLine;
			path->AddEdge( edge );
			mPtEnd=c.anchor2;
			if ( mPtEnd.x < -214748)
			{
				mPtEnd.x = 0;
				mPtEnd.y = 0;
			}
			break;
		}
		cmds=cmds->pNext;
	}	
	if(!path->EdgeEmpty() /*&& path->mStartPt.x > -21474833*/ )
	{
		LOGWHEREVAL( path->mStartPt.x );
		if(PTNEQ(path->mStartPt,mPtEnd)&&fillStyle)// path.StartPt() != mPtEnd )
		{
			BwEdge* edge=pAlloc->AllocEdge();
			edge->mEndPt = mPtEnd;
			edge->mControlPt = mPtEnd;
			path->AddEdge( edge );
		}
		if ( path->mStartPt.x < -214748)
		{
			int i = 0;
		}
		pShapeRec->AddPath(path);
	}
	else
		pAlloc->FreePath(path);
	if(!pShapeRec->IsEmpty())
	{
		
		pShapeRec->m_bODD = true;
		TessPloy::Tess(pShapeRec,pShapeRec->fMeter,true);
		//pShapeRec->ReleasePaths();
	}
	//if(mbLineBegin&&!PointEqual(&mPtBegin,&mPtEnd))
	//{
	//	CURVE c;
	//	c.isLine=XTRUE;
	//	c.anchor1=mPtBegin;
	//	c.anchor2=mPtEnd;
	//	c.isLine=XTRUE;
	//	::PointAverage(&mPtBegin,&mPtEnd,&c.control);
	//	AddCurve(&c);
	//	mbLineBegin=XFALSE;
	//}

	return;

 }

BwLineStyle* XDrawObj::GetLineStyle(_XCOMMAND *pCmd,int nCmd)
{
	ShapeAlloc*pAlloc = ShapeAlloc::GetInst();
	
	BwLineStyle* pStyle = pAlloc->AllocLineStyle();
	pStyle->mWidth = pCmd->nLineData;
	pStyle->mColor = pCmd->pColor->rgb;
	if(pCmd->pColor->colorType==glColorSolid)
	{
		pStyle->color = NULL;
		
	}
	else
	{
		pStyle->color = GetColor(pCmd,nCmd);
	}
	
	return pStyle;
}

_GLColor* XDrawObj::GetColor(_XCOMMAND *pCmd,int nCmd)
{
	ShapeAlloc*pAlloc = ShapeAlloc::GetInst();
	
	_GLColor* color = pAlloc->AllocColor();//display->CreateColor();
	if ( !color ) 
		return XNULL;
	
	//color->SetUp(raster);
	color->colorType=pCmd->pColor->colorType;
	int j;
	switch(pCmd->pColor->colorType)
	{
	case glColorBitmap://, 	// a bitmap 
		if(!pCmd->nAttachData) break;		
		if(nCmd==DC_BEGINFILLBITMAP_EX)
		{
			 //SBitmapCore*sbitmap=XNULL;
			 BitmapDataObject*pBmp = (BitmapDataObject*)pCmd->nAttachData;
			 //XDrawDib*pDib=player->m_pAVM2->GetDrawDibData((void*)pCmd->nData,(void**)&sbitmap);
			 pBmp->CreateTexture();
				 //pBitmap->GetBitmap();
		//	 if(pBmp->IsFrame())
		//	 {
		//		 color->frame.savedMat=pCmd->pColor->frame.savedMat;
		////		 GetMatrix(&color->bm.savedMat);	// the bitmap matrix
		//		 color->rgb=pCmd->pColor->rgb;
		//		 color->colorType = glFrameBuffer;
		//		 color->frame.smooth=0;
		//		 color->frame.repeat = pCmd->pColor->frame.repeat;
		//		 color->frame.frame = pBmp->m_pChar->frame;//sbitmap;//&pBitmap->m_sbitmap; 
		//		 color->frame.bitsStyle = pCmd->pColor->frame.bitsStyle;//fillStyle;
		//	 }
		//	 else
			 {
				 color->colorType=glColorBitmap;//Bitmap;
				 color->bm.savedMat = pCmd->pColor->frame.savedMat;
				 color->rgb.alpha=0xff;
				 color->rgb.blue=0xff;
				 color->rgb.red=0;
				 color->bm.smooth=0;
				 color->rgb.green=0;
				 color->bm.repeat = pCmd->pColor->frame.repeat;
				 color->bm.bitmap = &pBmp->m_pChar->bits;//&pBitmap->m_sbitmap;
				 color->bm.bitsStyle=fillBitsClip|fillBitsNoSmooth;
			 }

		}
		else
		{
			 XXObjectBitmap*pBmp=(XXObjectBitmap*)pCmd->nAttachData;
			 //XDrawDib*pDib=pBitmap->GetBitmap();
			 pBmp->CreateTexture();
				 //pBitmap->GetBitmap();
			 color->colorType=glColorBitmap;//Bitmap;
			 color->bm.savedMat = pCmd->pColor->frame.savedMat;
			 color->rgb.alpha=0xff;
			 color->rgb.blue=0xff;
			 color->rgb.red=0;
			 color->bm.smooth=0;
			 color->rgb.green=0;
			 color->bm.repeat = pCmd->pColor->frame.repeat;
			 color->bm.bitmap = &pBmp->m_pChar->bits;//&pBitmap->m_sbitmap;
			 color->bm.bitsStyle=fillBitsClip|fillBitsNoSmooth;
			 
	//		 color->frame.savedMat=pCmd->pColor->frame.savedMat;
	////		 GetMatrix(&color->bm.savedMat);	// the bitmap matrix
	//		 color->rgb=pCmd->pColor->rgb;
	//		 color->colorType = glFrameBuffer;
	//		 color->frame.smooth=0;
	//		 color->frame.repeat = pCmd->pColor->frame.repeat;
	//		 color->frame.frame = pBmp->m_pChar->frame;//sbitmap;//&pBitmap->m_sbitmap; 
	//		 color->frame.bitsStyle = pCmd->pColor->frame.bitsStyle;//fillStyle;
		}
		//color->SetBitmapDiv();
		//if(cxform.nBlendMode)
		//	color->bm.bitmap->SetBlendMode(cxform.nBlendMode);
		break;
	case glColorGradient:

		 color->grad.gradStyle = pCmd->pColor->grad.gradStyle;//fillStyle;
 		// color->grad.colorRamp = pCmd->pColor->grad.colorRamp;
		 color->grad.savedMat  = pCmd->pColor->grad.savedMat;

		 color->grad.ramp.nColors	= pCmd->pColor->grad.ramp.nColors;//nColor&0xf;
		 color->grad.ramp.spreadMode= pCmd->pColor->grad.ramp.spreadMode;//(nColor>>6);
		 color->grad.ramp.interMode	= pCmd->pColor->grad.ramp.interMode;//(nColor>>4)&3;
				//GetByte();
		for ( j = 0; j < color->grad.ramp.nColors; j++ ) {
			color->grad.ramp.colorRatio[j] = pCmd->pColor->grad.ramp.colorRatio[j];
			//color->transparent = pCmd->pColor->transparent;//true;
			color->grad.ramp.color[j] = pCmd->pColor->grad.ramp.color[j];
			}
		 break;
	case glColorSolid:
		 color->rgb=pCmd->pColor->rgb;
		 //color->transparent = PreMulAlpha(&color->rgb);
		 break;
	}
	color->SetUp();
	return color;
}


void XDrawObj::BuildEdgesCamera(SBitmapCore*bitmap)
{
	RasterGL*raster = RasterGL::sharedRasterGL();
	if(bitmap->bitmapID)
	{
		raster->UpdateTexture(bitmap->bitmapID->id,
							  bitmap->baseAddr,Texture2DPixelFormat_RGBA8888,0,0,bitmap->width,bitmap->height);
		return;
	}
	if(pShapeRec->pFirst) return;
	
	BuildEdges(bitmap,false);

	return;
}

void XDrawObj::BuildEdges(SBitmapCore*bitmap,bool bRelease)
{
;	
	if(!bitmap->HasBits()&&!bitmap->bitmapID) 
	{
#ifdef _WINEMU
		XSWFPlayer::m_pInstance->Trace(XString16("[Error] Draw Null bitmap!!!!!!!"));
#endif
		return;
	}

	pShapeRec->SetFills(2);
	pShapeRec->AddFillSyle(0,XNULL);
	//{
		ShapeAlloc* pAlloc = ShapeAlloc::GetInst();
		GLColor* color = pAlloc->AllocColor();
		if ( color ) 
		{		  
		  pShapeRec->AddFillSyle(1,color);
		  color->colorType=glColorBitmap;//Bitmap;
		  ::MatrixIdentity(&color->bm.savedMat);//=pCmd->pColor->bm.savedMat;
		  color->bm.savedMat.a=fixed_1*20;
		  color->bm.savedMat.d=fixed_1*20;
		  color->rgb.alpha=0xff;
		  color->rgb.blue=0xff;
		  color->rgb.red=0;
		  color->bm.smooth=0;
		  color->rgb.green=0;
		  color->bm.repeat = 0;
		  color->bm.bitmap = bitmap;//&pBitmap->m_sbitmap;
		  color->bm.bitsStyle=fillBitsClip|fillBitsNoSmooth;
		  if(!bitmap->bitmapID)
		  {
			  bitmap->CreateTexture();
			  if(bRelease) bitmap->ReleaseData();
		  }
		  //pBitmap->m_sbitmap.PICreate(*pDib,bTransparent);
		}
		color->SetUp();
	//}

	int w=(bitmap->width)*20;//pDib->Width()*20;
	int h=(bitmap->height)*20;//pDib->Height()*20;
	if(bitmap->nImgDiv)
	{
		w<<=bitmap->nImgDiv;
		h<<=bitmap->nImgDiv;
	}
	int xx=0;
	int yy=0;
	
	//ShapeAlloc*pAlloc=ShapeAlloc::GetInst();
	GLTranglesOrLines* pData = pAlloc->AllocTrangleOrLines();
	pShapeRec->pFirst = pData;
	pShapeRec->pLast = pData;
	pData->nCount = 0;
	pData->type = GLT_RECTANGLE;
	pData->pNext = NULL;
	GLRectangle* pRect = pAlloc->AllocRectangle();
	pData->rectangle = pRect;
	pRect->pFillStyle = color;
	pRect->pos[0] = xx;
	pRect->pos[1] = yy;
	pRect->pos[2] = w;
	pRect->pos[3] = h;
	/*BwPath* path=pAlloc->AllocPath();
	path->FillRight(1);
	XPOINT pt;
	pt.x = xx;
	pt.y = yy;
	path->StartPt( pt );

	BwEdge* edge=pAlloc->AllocEdge();
	pt.x = xx;
	pt.y = h;
	edge->mbLine = true;
//	MatrixTransformPoint(&mat,&pt,&pt );
	edge->mEndPt = pt;
	edge->mControlPt = pt;
	path->AddEdge( edge );
	edge=pAlloc->AllocEdge();
	pt.x = w;
	pt.y = h;
	edge->mbLine = true;
//	MatrixTransformPoint(&mat,&pt,&pt );
	edge->mEndPt = pt;
	edge->mControlPt = pt;
	path->AddEdge( edge );
	edge=pAlloc->AllocEdge();
	pt.x = w;
	pt.y = yy;
//	MatrixTransformPoint(&mat,&pt,&pt );
	edge->mEndPt = pt;
	edge->mbLine = true;
	edge->mControlPt = pt;
	path->AddEdge( edge );
	edge=pAlloc->AllocEdge();
	edge->mbLine = true;
	pt.x = xx;
	pt.y = yy;
//	MatrixTransformPoint(&mat,&pt,&pt );
	edge->mEndPt = pt;
	edge->mControlPt = pt;
	path->AddEdge( edge );
	pShapeRec->AddPath( path );
	//path.ClearEdges();
	TessPloy::Tess(pShapeRec);*/
	return;
}

void XDrawObj::BuildEdgesImage(_XSCharacter*ch)
{
	if(!ch->record.m_pImage) return;
	pShapeRec->SetFills(2);
	pShapeRec->AddFillSyle(0,XNULL);
	ShapeAlloc* pAlloc = ShapeAlloc::GetInst();
	GLColor* color = pAlloc->AllocColor();
	if(!color) return;
	pShapeRec->AddFillSyle(1,color);
	int w,h;
	_ShapeImage*pImage = ch->record.m_pImage;
	{
		  color->colorType=glColorDib;//Bitmap;
		  ::MatrixIdentity(&color->dib.savedMat);//=pCmd->pColor->bm.savedMat;
		  color->dib.preAlpha = true;
		  color->dib.savedMat.a=fixed_1*20;
		  color->dib.savedMat.d=fixed_1*20;
		  color->rgb.alpha=0xff;
		  color->rgb.blue=0xff;
		  color->rgb.red=0;
		  color->rgb.green=0;
		  color->dib.bitmapID = pImage->id;
		  color->dib.repeat = 0;
		  w=pImage->w;
		  h=pImage->h;
		  color->dib.w = w;
		  color->dib.h = h;
		  //pBitmap->m_sbitmap.PICreate(*pDib,bTransparent);
		  color->SetUp();
	}

	//int w=(bitmap->width)*20;//pDib->Width()*20;
	//int h=(bitmap->height)*20;//pDib->Height()*20;
	int xx=0;
	int yy=0;
	
	//ShapeAlloc*pAlloc=ShapeAlloc::GetInst();
	GLTranglesOrLines* pData = pAlloc->AllocTrangleOrLines();
	pShapeRec->pFirst = pData;
	pShapeRec->pLast = pData;
	pData->nCount = 0;
	pData->type = GLT_RECTANGLE;
	pData->pNext = NULL;
	GLRectangle* pRect = pAlloc->AllocRectangle();
	pData->rectangle = pRect;
	pRect->pFillStyle = color;
	pRect->pos[0] = xx;
	pRect->pos[1] = yy;
	pRect->pos[2] = w*20;
	pRect->pos[3] = h*20;
}
void XDrawObj::BuildEdges(_XSCharacter*ch)
{

	pShapeRec->SetFills(2);
	pShapeRec->AddFillSyle(0,XNULL);
	ShapeAlloc* pAlloc = ShapeAlloc::GetInst();
	GLColor* color = pAlloc->AllocColor();
	if(!color) return;
	pShapeRec->AddFillSyle(1,color);
	int w,h;

	//if(ch->type!=bitmapEmptyChar&&ch->type!=videoEmptyChar)
	{
		  color->colorType=glColorBitmap;//Bitmap;
		  ::MatrixIdentity(&color->bm.savedMat);//=pCmd->pColor->bm.savedMat;
		  color->bm.savedMat.a=fixed_1*20;
		  color->bm.savedMat.d=fixed_1*20;
		  color->rgb.alpha=0xff;
		  color->rgb.blue=0xff;
		  color->rgb.red=0;
		  color->bm.smooth=0;
		  color->rgb.green=0;
		  color->bm.repeat = 0;
		  color->bm.bitmap = &ch->bits;//&pBitmap->m_sbitmap;
		  color->bm.bitsStyle=fillBitsClip|fillBitsNoSmooth;
		  if(!ch->bits.bitmapID)
		  {
			  XSWFPlayer::m_pInstance->CreateIfEmpty(ch);
			  //ch->bits.CreateTexture();
			  //ch->bits.ReleaseData();
		  }
		  w=ch->bits.width;
		  h=ch->bits.height;
		  if(ch->bits.nImgDiv)
		  {
			  w<<=ch->bits.nImgDiv;
			  h<<=ch->bits.nImgDiv;
		  }
		  //pBitmap->m_sbitmap.PICreate(*pDib,bTransparent);
		color->SetUp();
	}
	//else
	//{
	//	 color->colorType=glFrameBuffer;//Bitmap;
	//	  ::MatrixIdentity(&color->frame.savedMat);//=pCmd->pColor->bm.savedMat;
	//	  color->frame.savedMat.a=fixed_1*20;
	//	  color->frame.savedMat.d=fixed_1*20;
	//	  color->rgb.alpha=0xff;
	//	  color->rgb.blue=0xff;
	//	  color->rgb.red=0;
	//	  color->frame.smooth=0;
	//	  color->rgb.green=0;
	//	  color->frame.frame = ch->frame;//&pBitmap->m_sbitmap;
	//	  color->frame.repeat = 0;
	//	  color->frame.bitsStyle=fillBitsClip|fillBitsNoSmooth;
	//	  w=ch->frame->mWidth;
	//	  h=ch->frame->mHeight;
	//	  if(ch->frame->nImgDiv)
	//	  {
	//		  w<<=ch->frame->nImgDiv;
	//		  h<<=ch->frame->nImgDiv;
	//	  }
	//	  //pBitmap->m_sbitmap.PICreate(*pDib,bTransparent);
	//	color->SetUp();
	//}

	//int w=(bitmap->width)*20;//pDib->Width()*20;
	//int h=(bitmap->height)*20;//pDib->Height()*20;
	int xx=0;
	int yy=0;
	
	//ShapeAlloc*pAlloc=ShapeAlloc::GetInst();
	GLTranglesOrLines* pData = pAlloc->AllocTrangleOrLines();
	pShapeRec->pFirst = pData;
	pShapeRec->pLast = pData;
	pData->nCount = 0;
	pData->type = GLT_RECTANGLE;
	pData->pNext = NULL;
	GLRectangle* pRect = pAlloc->AllocRectangle();
	pData->rectangle = pRect;
	pRect->pFillStyle = color;
	pRect->pos[0] = xx;
	pRect->pos[1] = yy;
	pRect->pos[2] = w*20;
	pRect->pos[3] = h*20;
	/*BwPath* path=pAlloc->AllocPath();
	path->FillRight(1);
	XPOINT pt;
	pt.x = xx;
	pt.y = yy;
	path->StartPt( pt );

	BwEdge* edge=pAlloc->AllocEdge();
	pt.x = xx;
	pt.y = h;
	edge->mbLine = true;
//	MatrixTransformPoint(&mat,&pt,&pt );
	edge->mEndPt = pt;
	edge->mControlPt = pt;
	path->AddEdge( edge );
	edge=pAlloc->AllocEdge();
	pt.x = w;
	pt.y = h;
	edge->mbLine = true;
//	MatrixTransformPoint(&mat,&pt,&pt );
	edge->mEndPt = pt;
	edge->mControlPt = pt;
	path->AddEdge( edge );
	edge=pAlloc->AllocEdge();
	pt.x = w;
	pt.y = yy;
//	MatrixTransformPoint(&mat,&pt,&pt );
	edge->mEndPt = pt;
	edge->mbLine = true;
	edge->mControlPt = pt;
	path->AddEdge( edge );
	edge=pAlloc->AllocEdge();
	edge->mbLine = true;
	pt.x = xx;
	pt.y = yy;
//	MatrixTransformPoint(&mat,&pt,&pt );
	edge->mEndPt = pt;
	edge->mControlPt = pt;
	path->AddEdge( edge );
	pShapeRec->AddPath( path );
	//path.ClearEdges();
	TessPloy::Tess(pShapeRec);*/
	return;
}