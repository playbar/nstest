#include "StdAfxflash.h"
#include "sobject.h"
#include "sdisplay.h"
#include "XDrawObj.h"
#include "ShellCore.h"
#include "splayer.h"

using namespace avmplus;
#include "AS3DisplayObject.h"
#include "AS3Graphics.h"
#include "AS3BitmapData.h"
#include "AS3Matrix.h"
#include "avm2.h"
namespace avmshell{
GraphicsClass::GraphicsClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())GraphicsObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* GraphicsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) GraphicsObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
GraphicsObject::GraphicsObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_pCmds=NULL;
	m_pDisplay=NULL;
	m_nCurX=0;
	m_nCurY=0;
	m_pDisplayList=NULL;
	m_nThick=0;
	lastAlpha=0;
	m_hasLineOrFill=0;
}

GraphicsObject* GraphicsClass::CreateGraphics(DisplayObjectObject*pObject)
{
	GraphicsObject*pObj=(GraphicsObject*)createInstance(ivtable(),prototype);
	if(pObject->m_pObject)
		pObj->m_pDisplay=pObject;
	if(pObject&&pObject->m_pObject)
		pObj->m_pDisplayList=pObject->m_pObject->display;
	else
		pObj->m_pDisplayList=NULL;
	return pObj;
}

void GraphicsObject::Release()
{
	if(IsRunning())
	{
	if(m_pCmds)
		FreeCommands(m_pCmds);	
	}
	m_pCmds=NULL;
}
///////////////////////////////////////////////////////////////
void GraphicsObject::FreeCommands(_XCOMMAND*cmds,bool bAll)
{
	if(!m_pDisplay) 
		return;
	_XCOMMAND*pCmd=cmds;
	ShapeAlloc* pAlloc = ShapeAlloc::GetInst();
	DisplayList*display=GetDisplay();
	while(pCmd)
	{
		switch(pCmd->nCommand)
		{
		case XDrawObj::DC_BEGINFILLBITMAP:
			 AvmAssert(false);
			 break;
		case XDrawObj::DC_BEGINFILLBITMAP_EX:
			 if(pCmd->pColor)
				 pAlloc->FreeColor(pCmd->pColor);
				 //delete pCmd->pColor;
				//display->FreeColor(pCmd->pColor);
			 if(pCmd->nAttachData)
			 {
				 BitmapDataObject*pBmp=(BitmapDataObject*)pCmd->nAttachData;
				 ((ShellToplevel*)toplevel())->RemoveObject(pBmp);
				 //pBmp->DecrementRef();
			 }
			 break;
		case XDrawObj::DC_BEGINFILLGRAD:
		case XDrawObj::DC_BEGINLINEGRAD:
		case XDrawObj::DC_LINESTYLE:
		case XDrawObj::DC_BEGINFILL:
			 if(pCmd->pColor)
				 pAlloc->FreeColor(pCmd->pColor);
				 //delete pCmd->pColor;
				//display->FreeColor(pCmd->pColor);
				// delete pCmd->pColor;
			 break;
		case XDrawObj::DC_CIRCLE:
		case XDrawObj::DC_ELLIPSE:
		case XDrawObj::DC_RECT:
		case XDrawObj::DC_ROUNDRECT:
		case XDrawObj::DC_MOVETO:
		case XDrawObj::DC_CURVE:
			 if(pCmd->pCurve)
				 display->FreeCurve(pCmd->pCurve);
				 //delete pCmd->pCurve;
			 break;
		}
		_XCOMMAND*p=pCmd;
		pCmd=pCmd->pNext;
		display->FreeCommand(p);
		if(!bAll) break;
		//delete p;
	}

}

void GraphicsObject::ClearAllActor()
{
	return;
	_XCOMMAND*pFill=NULL;
	_XCOMMAND*pLine=NULL;
	_XCOMMAND*pCmd=m_pCmds;
	while(pCmd)
	{
		switch(pCmd->nCommand)
		{
		case XDrawObj::DC_BEGINFILLBITMAP:		
		case XDrawObj::DC_BEGINFILLBITMAP_EX:
		case XDrawObj::DC_BEGINFILLGRAD:
		case XDrawObj::DC_BEGINLINEGRAD:		
		case XDrawObj::DC_BEGINFILL:
			 pFill=pCmd;
			 break;
		case XDrawObj::DC_ENDFILL:
			 pFill=NULL;break;
		case XDrawObj::DC_ENDLINE:
			 pLine=NULL;break;
		case XDrawObj::DC_LINESTYLE:
			 pLine=pCmd;
			 break;
		}
		pCmd=pCmd->pNext;
	}	
	pCmd=m_pCmds;
	while(pCmd)
	{
		_XCOMMAND*pNext=pCmd->pNext;
		if(pCmd!=pLine&&pCmd!=pFill)
		{
			FreeCommands(pCmd,false);
		}
		pCmd=pNext;
	}
	if(pFill)
	{
		m_pCmds=pFill;
		m_pCmds->pNext=pLine;
		if(pLine)
			pLine->pNext=NULL;
	}
	else
	{
		m_pCmds=pLine;
		if(pLine)
			pLine->pNext=NULL;
	}
}

void GraphicsObject::AddCommand(_XCOMMAND*pCmd)
{
	int iCount=0;
	pCmd->pNext=XNULL;
	_XCOMMAND**pc=&m_pCmds;
	for(;;)
	{
		iCount++;
		_XCOMMAND*p=*pc;
		if(p==XNULL)
		{
			*pc=pCmd;
			break;
		}
		else
			pc=&p->pNext;
	}
	//StUTF8String ss(m_pDisplay->vtable->traits->name());
	//if(strcmp(ss.c_str(),"ListBase")==0)
	//	int v=0;
	//if(iCount>=12)
	//	int v=0;
}

/*inline DisplayList* GraphicsObject::GetDisplay()
{
	//if(!m_pDisplay||!m_pDisplay->m_pObject) return NULL;
	//return m_pDisplay->m_pObject->display;
	ShellCore*c=(ShellCore*)core();
	return &c->GetPlayer()->display;
}*/
//////////////////////////////////////////////////////////
//Native Method start...
void GraphicsObject::AS3_beginBitmapFill(BitmapDataObject*bmp,MatrixObject*mat,bool rep,bool smooth)
{
	//Add your act code here...
	if(!m_pDisplay||!m_pDisplay->m_pObject) return;
	DisplayList* display=GetDisplay();
	_XCOMMAND*pCmd=display->CreateCommand();//new _XCOMMAND;
	if(!pCmd) return;
	//RColor* pColor=display->CreateColor();
	GLColor* pColor = ShapeAlloc::GetInst()->AllocColor();
	if(!pColor) 
	{
		display->FreeCommand(pCmd);
		return;
	}

	//m_bFill=XTRUE;
	ShellToplevel*top=(ShellToplevel*)toplevel();
	top->PushObject(bmp);
	//bmp->IncrementRef();
	//pCmd->pColor->frame.repeat = rep?1:0;
	pCmd->nAttachData=(XU32)bmp;
	pCmd->nLineData = 0;
	pColor->colorType = glColorBitmap;
	//if(bmp->m_pDib&&bmp->m_pDib->m_nWidth==200)
	//	int v=0;
	if(mat)
	{
		mat->GetMatrix(&pColor->frame.savedMat,true);
		MATRIX matz={fixed_1*20,0,0,fixed_1*20,0,0};
		::MatrixConcat(&matz,&pColor->frame.savedMat,&pColor->frame.savedMat);
	}
	else
	{
		::MatrixIdentity(&pColor->frame.savedMat);
		MATRIX&mat=pColor->frame.savedMat;
		mat.a*=20;
		mat.d*=20;
	}
	//pColor->bm.savedMat=*pMatrix;

	pColor->rgb.alpha = 255;
	pColor->rgb.red = 0;
	pColor->rgb.green = pColor->rgb.blue = 0;
	pColor->frame.bitsStyle=rep?fillBits:(fillBits|fillBitsClip);
	pColor->frame.repeat = rep?1:0;
	pCmd->pColor=pColor;
//	pColor->cacheValid=false;
	pCmd->nCommand=XDrawObj::DC_BEGINFILLBITMAP_EX;

	lastAlpha=bmp->m_pChar->bits.transparent?0:1;

	AddCommand(pCmd);
	m_hasLineOrFill|=HASFILL;
}


void GraphicsObject::AS3_beginFill(uint32_t nColor,double alpha)
{
	//StUTF8String ss(m_pDisplay->vtable->traits->name());
	//if(strcmp(ss.c_str(),"ListBase")==0)
	//	int v=0;
	if(!m_pDisplay) 
		return;
	if(alpha<=0&&m_pCmds) 
	{
		if(m_hasLineOrFill&HASFILL)
			AS3_endFill();
		return;
	}
	_beginFill(nColor,alpha,0);
	//m_hasLineOrFill|=HASFILL;
}

void GraphicsObject::_beginFill(uint32_t nColor,double alpha,int nWidth)
{
	//Add your act code here..
	if(!m_pDisplay) 
		return;
	if(alpha<=0&&m_pCmds) return;
	if(!m_pDisplay||!m_pDisplay->m_pObject) return;
	DisplayList* display=m_pDisplay->m_pObject->display;
	_XCOMMAND*pCmd=display->CreateCommand();//new _XCOMMAND;
	if(!pCmd) return;
	//RColor* pColor=display->CreateColor();
	GLColor* pColor = ShapeAlloc::GetInst()->AllocColor();
	if(!pColor) 
	{//delete pCmd;
		display->FreeCommand(pCmd);
		return;
	}
	if(!nWidth)
		lastAlpha=alpha;
	//m_bFill=XTRUE;

	pCmd->nLineData=nWidth;//(XU32)pBitmap->RefObject();
	
	pColor->colorType = glColorSolid;//colorBitmap;
	//pColor->bm.savedMat=*pMatrix;

	alpha=alpha*255;
	if(alpha<0) alpha=0;
	else if(alpha>255) alpha=255;

	pColor->rgb.alpha=(XU16)(alpha);//nColor>>24);
	pColor->rgb.red=(XU16)((nColor>>16)&0xff);
	pColor->rgb.green=(XU16)((nColor>>8)&0xff);
	pColor->rgb.blue=(XU16)(nColor&0xff);

	pCmd->pColor=pColor;

	pCmd->nCommand=nWidth?XDrawObj::DC_LINESTYLE:XDrawObj::DC_BEGINFILL;
	AddCommand(pCmd);
	m_hasLineOrFill|=nWidth?HASLINE:HASFILL;
	return;

}

void GraphicsObject::AS3_beginGradientFill(Stringp strType,ArrayObject*colors,ArrayObject*alphas,ArrayObject*ratios,MatrixObject*matrix,Stringp spreadMothod,Stringp interMode,double fRatio)
{
	//lastAlpha=0;
	if(!m_pDisplay) 
		return;
	_beginFill(strType,colors,alphas,ratios,matrix,spreadMothod,interMode,fRatio,0);
}

inline void GraphicsObject::_beginFill(Stringp strType,ArrayObject*colorsa,ArrayObject*alphas,ArrayObject*ratios,MatrixObject*matrix,Stringp spreadMothod,Stringp interMode,double fRatio,int nWidth)
{
	//Add your act code here...
	if(!m_pDisplay||!m_pDisplay->m_pObject||!colorsa) return;

	XU32 nColors[fillMaxGradientColors];
	int  nColorNum=0;
	int i,nc=colorsa->getDenseLength();
	AvmCore*cc=core();
	for(i=0;i<nc;i++)
	{
		Atom at=colorsa->getIntProperty(i);
		if(atomKind(at)==kStringType)
		{
			Stringp strColor=AvmCore::atomToString(at);
			StUTF8String ss(strColor);
			XString8 strColors8(ss.c_str());
			XString8Array list;
			list.SplitString(strColors8,',');
			for(int j=0;j<list.GetSize();j++)
			{
				char*str=list[j];
				if(str[0]=='#')
				{
					XString8 str(str+1);
					nColors[nColorNum]=str.ToInt(16);
				}
				else
				{
					nColors[nColorNum]=list[j].ToInt(10);
				}
				nColorNum++;
				if(nColorNum>=fillMaxGradientColors) break;
			}
		}
		else
		{
//#if (__CORE_VERSION__>=0x02072000)
			nColors[nColorNum]=cc->integer(at);
//#else
//			nColors[nColorNum]=cc->integer_u(at);
//#endif
			nColorNum++;
		}
		if(nColorNum>=fillMaxGradientColors) break;
	}
	if(nColorNum<=0) return;

	DisplayList* display=m_pDisplay->m_pObject->display;
	_XCOMMAND*pCmd=display->CreateCommand();//new _XCOMMAND;
	if(!pCmd) return;
	//RColor* pColor=display->CreateColor();
	GLColor* pColor = ShapeAlloc::GetInst()->AllocColor();
	if(!pColor) {display->FreeCommand(pCmd);return;}

	//int nColors=0;

	//Atom at=colorsa->getAtomProperty(0)
	//int type=
	//m_bFill=XTRUE;
	int nType=fillLinearGradient;
	if(strType)
	{
		StUTF8String strT(strType);
		switch(strT.c_str()[0])
		{
		case 'R':
		case 'r': nType=fillRadialGradient;
		}
	}

	pColor->colorType = glColorGradient;
	pColor->grad.gradStyle = nType;//nType?fillRadialGradient:fillLinearGradient;
// 	pColor->grad.colorRamp = 0;
	if(matrix)
		matrix->GetMatrix(&pColor->grad.savedMat,true);
	else
		::MatrixIdentity(&pColor->grad.savedMat);
	
	pColor->grad.ramp.nColors=nColorNum;//(U8)colors->getDenseLength();//GetSize();
	pColor->grad.ramp.spreadMode=0;
	if(spreadMothod)
	{
		StUTF8String s(spreadMothod);
		switch(s.c_str()[2])
		{
		case 'F':
		case 'f':
			pColor->grad.ramp.spreadMode=1;break;
		case 'p':
		case 'P':
			pColor->grad.ramp.spreadMode=2;break;
		}
	}
	pColor->grad.ramp.interMode=0;//nInterMode;
	if(interMode)
	{
		StUTF8String s(interMode);
		switch(s.c_str()[0])
		{
		case 'l':
		case 'L':pColor->grad.ramp.interMode=1;break;
		}
	}

	XU32 j,l=nColorNum;//colors->getDenseLength();

	
//	pColor->transparent=false;
	if(l>fillMaxGradientColors)
		l=fillMaxGradientColors;

	
	for ( j = 0; j < l; j++ ) 
	{
		pColor->grad.ramp.colorRatio[j] = (ratios&&j<ratios->getDenseLength())?cc->number(ratios->getIntProperty(j)):j*255/(l-1);
		//SRGB c;// = GetColor(getAlpha);
		//Atom at=colors->getIntProperty(j);

		//Stringp ss=(Stringp)(at&~7);

		int all=nColors[j];//cc->integer_u(at);
		int transparency=(alphas&&j<alphas->getDenseLength())?(cc->number(alphas->getIntProperty(j))*255):255;
		if(transparency>255) transparency=255;
		//if ( transparency < 255 )
		//	pColor->transparent = true;
		pColor->grad.ramp.color[j].rgb.red = all>>16;
		pColor->grad.ramp.color[j].rgb.green=(all>>8)&0xff;
		pColor->grad.ramp.color[j].rgb.blue=all&0xff;
		pColor->grad.ramp.color[j].rgb.transparency=(U8)transparency;
	}

	pCmd->nLineData=nWidth;
	pCmd->pColor=pColor;
	pCmd->nCommand=nWidth?XDrawObj::DC_BEGINLINEGRAD:XDrawObj::DC_BEGINFILLGRAD;

	AddCommand(pCmd);
	if(!nWidth)
		lastAlpha=0;//pColor->transparent?0:1;
	m_hasLineOrFill|=nWidth?HASLINE:HASFILL;
}

void GraphicsObject::AS3_clear()
{
	//Add your act code here...
	if(!m_pDisplay) 
		return;
	SObject*pObj=GetSObject();
	m_nThick=0;
	FreeCommands(m_pCmds);
	m_pCmds=NULL;
	
	if(pObj)
	{
		pObj->mShapeRecord.ReleaseAll();
		if(pObj->drawn)
			pObj->display->InvalidateRect(&pObj->devBounds);
//#if (__CORE_VERSION__>=0x02076000)
		if(pObj->character&&pObj->character->tag==0)
			RectSetEmpty(&pObj->character->rbounds);
//#endif
		pObj->Modify();
	}
	lastAlpha=0;
	m_hasLineOrFill=0;
}

inline _XSObject* GraphicsObject::GetSObject()
{
	if(!m_pDisplay||!m_pDisplay->m_pObject) return NULL;
	return m_pDisplay->m_pObject;
}


void GraphicsObject::AS3_curveTo(double cx,double cy,double ax,double ay)
{
	//Add your act code here...
	if(MathUtils::isNaN(cx)||MathUtils::isNaN(cy)
		||MathUtils::isNaN(ax)||MathUtils::isNaN(ay))
	if(!m_pDisplay) 
		return;
	if(!m_hasLineOrFill) return;
//#if (__CORE_VERSION__>=0x02070100)
	if(m_nCurX==ax*20&&
	   m_nCurY==ay*20) return;
//#endif
	DisplayList* display=GetDisplay();
	if(!display) return;
	_XCOMMAND*pCmd=display->CreateCommand();//new _XCOMMAND;
	if(!pCmd) return;
	SObject*rootObject=GetSObject();
	
	P_CURVE pc=display->CreateCurve();//new _XCURVE;
	if(!pc)
	{
		display->FreeCommand(pCmd);
		//delete pCmd;
		return;
	}
	
	rootObject->UpdateBounds(m_nCurX,m_nCurY,m_nThick);

	pc->isLine=XFALSE;
	pc->anchor1.x=m_nCurX;
	pc->anchor1.y=m_nCurY;
	pc->control.x=cx*20;
	pc->control.y=cy*20;
	pc->anchor2.x=ax*20;
	pc->anchor2.y=ay*20;

	rootObject->UpdateBounds(pc->anchor2.x,pc->anchor2.y,m_nThick);
	rootObject->UpdateBounds(pc->control.x,pc->control.y,m_nThick);
	

	pCmd->pCurve=pc;
	pCmd->nCommand=XDrawObj::DC_CURVE;

	//m_pDrawObj->AddCommand(pCmd);	
	AddCommand(pCmd);

	m_nCurX=pc->anchor2.x;
	m_nCurY=pc->anchor2.y;
	rootObject->Modify();
	rootObject->mShapeRecord.ReleaseAll();
}

void GraphicsObject::AS3_drawCircle(double x,double y,double r)
{
	//Add your act code here...
	if(MathUtils::isNaN(x)||MathUtils::isNaN(y)||MathUtils::isNaN(r))
		return;
	if(!m_pDisplay) 
		return;
	if(!m_hasLineOrFill) return;
//#if (__CORE_VERSION__>=0x02070100)
	if(r<=0) return;
//#endif
	DisplayList* display=GetDisplay();
	if(!display) return;
	_XCOMMAND*pCmd=display->CreateCommand();//new _XCOMMAND;
	if(!pCmd) return;
	SObject*rootObject=GetSObject();
	
	P_CURVE pc=display->CreateCurve();//new _XCURVE;
	if(!pc)
	{
		display->FreeCommand(pCmd);
		//delete pCmd;
		return;
	}
	
	//rootObject->UpdateBounds(m_nCurX,m_nCurY);

	pc->anchor1.x=x*20;
	pc->anchor1.y=y*20;
	pc->control.x=r*20;
	pc->control.y=pc->control.x;
	pc->anchor2.x=0;
	pc->anchor2.y=0;

	//if(pc->anchor1.x<-3000||
	//	pc->anchor1.y<-3000||
	//	pc->control.x<-3000)
	//	int v=0;

//#if (__CORE_VERSION__>=0x02080000)
	rootObject->UpdateBounds(pc->anchor1.x-r*20,pc->anchor1.y-r*20,pc->control.x*2,pc->control.y*2,m_nThick);
//#else
//	rootObject->UpdateBounds(pc->anchor1.x,pc->anchor1.y,pc->control.x,pc->control.y,m_nThick);
//#endif
	//rootObject->UpdateBounds(pc->anchor1.x-pc->control.x,pc->anchor1.y-pc->control.x,m_nThick);
	//rootObject->UpdateBounds(pc->anchor1.x+pc->control.x,pc->anchor1.y-pc->control.y,m_nThick);
	//rootObject->UpdateBounds(pc->anchor1.x+pc->control.x,pc->anchor1.y+pc->control.y,m_nThick);
	//rootObject->UpdateBounds(pc->anchor1.x-pc->control.x,pc->anchor1.y+pc->control.y,m_nThick);
	

	pCmd->pCurve=pc;
	pCmd->nCommand=XDrawObj::DC_CIRCLE;

	//m_pDrawObj->AddCommand(pCmd);	
	AddCommand(pCmd);

	m_nCurX=pc->anchor1.x+pc->control.x;
	m_nCurY=pc->anchor1.y;
	rootObject->Modify();
	rootObject->mShapeRecord.ReleaseAll();
}

void GraphicsObject::AS3_drawEllipse(double x,double y,double w,double h)
{
	//Add your act code here...
	if(MathUtils::isNaN(x)||MathUtils::isNaN(y)||MathUtils::isNaN(w)||MathUtils::isNaN(h))
		return;
	if(!m_pDisplay) 
		return;
	if(!m_hasLineOrFill) return;
//#if (__CORE_VERSION__>=0x02070100)
	if(w<=0||h<=0) return;
//#endif
	DisplayList* display=GetDisplay();
	if(!display) return;
	_XCOMMAND*pCmd=display->CreateCommand();//new _XCOMMAND;
	if(!pCmd) return;
	SObject*rootObject=GetSObject();
	
	P_CURVE pc=display->CreateCurve();//new _XCURVE;
	if(!pc)
	{
		display->FreeCommand(pCmd);
		//delete pCmd;
		return;
	}
	
	//rootObject->UpdateBounds(m_nCurX,m_nCurY);

	pc->anchor1.x=x*20;
	pc->anchor1.y=y*20;
	pc->control.x=w*20;
	pc->control.y=h*20;
	pc->anchor2.x=0;
	pc->anchor2.y=0;

	rootObject->UpdateBounds(pc->anchor1.x,pc->anchor1.y,pc->control.x,pc->control.y,m_nThick);
	//rootObject->UpdateBounds(pc->anchor1.x,pc->anchor1.y,m_nThick);
	//rootObject->UpdateBounds(pc->anchor1.x+pc->control.x,pc->anchor1.y,m_nThick);
	//rootObject->UpdateBounds(pc->anchor1.x+pc->control.x,pc->anchor1.y+pc->control.y,m_nThick);
	//rootObject->UpdateBounds(pc->anchor1.x,pc->anchor1.y+pc->control.y,m_nThick);
	//rootObject->UpdateBounds(pc->anchor1.x-pc->control.x,pc->anchor1.y-pc->control.x);
	//rootObject->UpdateBounds(pc->anchor1.x+pc->control.x,pc->anchor1.y-pc->control.y);
	//rootObject->UpdateBounds(pc->anchor1.x+pc->control.x,pc->anchor1.y+pc->control.y);
	//rootObject->UpdateBounds(pc->anchor1.x-pc->control.x,pc->anchor1.y+pc->control.y);
	

	pCmd->pCurve=pc;
	pCmd->nCommand=XDrawObj::DC_ELLIPSE;

	//m_pDrawObj->AddCommand(pCmd);	
	AddCommand(pCmd);

	m_nCurX=pc->anchor1.x+pc->control.x;
	m_nCurY=pc->anchor1.y+pc->control.y/2;
	rootObject->Modify();
	rootObject->mShapeRecord.ReleaseAll();
}

void GraphicsObject::AS3_drawRect(double x,double y,double w,double h)
{
	//Add your act code here...
		//Add your act code here..
	//StUTF8String ss(m_pDisplay->vtable->traits->name());
	//if(strcmp(ss.c_str(),"ListBase")==0)
	//	int v=0;.
	//if(w==520)
	//	w=50;
		//int v=0;
	//if(x==9&&y==6)
	//	int v=0;
	if(MathUtils::isNaN(x)||MathUtils::isNaN(y)||MathUtils::isNaN(w)||MathUtils::isNaN(h))
		return;
	if(!m_pDisplay) 
		return;
	if(!m_hasLineOrFill) return;
//#if (__CORE_VERSION__>=0x02070100)
	if(w<=0||h<=0) return;
//#endif
	DisplayList* display=GetDisplay();
	if(!display) return;
	_XCOMMAND*pCmd=display->CreateCommand();//new _XCOMMAND;
	if(!pCmd) return;
	SObject*rootObject=GetSObject();
	
	P_CURVE pc=display->CreateCurve();//new _XCURVE;
	if(!pc)
	{
		display->FreeCommand(pCmd);
		//delete pCmd;
		return;
	}
	
	//rootObject->UpdateBounds(m_nCurX,m_nCurY);

	pc->anchor1.x=x*20;
	pc->anchor1.y=y*20;
	pc->control.x=w*20;
	pc->control.y=h*20;
	pc->anchor2.x=0;
	pc->anchor2.y=0;

	if(lastAlpha>=1&&rootObject->IsOverBounds(pc->anchor1.x,pc->anchor1.y,pc->control.x,pc->control.y,m_nThick))
		ClearAllActor();

	rootObject->UpdateBounds(pc->anchor1.x,pc->anchor1.y,pc->control.x,pc->control.y,m_nThick);
	//rootObject->UpdateBounds(pc->anchor1.x,pc->anchor1.y,m_nThick);
	//rootObject->UpdateBounds(pc->anchor1.x+pc->control.x,pc->anchor1.y,m_nThick);
	//rootObject->UpdateBounds(pc->anchor1.x+pc->control.x,pc->anchor1.y+pc->control.y,m_nThick);
	//rootObject->UpdateBounds(pc->anchor1.x,pc->anchor1.y+pc->control.y,m_nThick);
	//rootObject->UpdateBounds(pc->anchor1.x-pc->control.x,pc->anchor1.y-pc->control.x);
	//rootObject->UpdateBounds(pc->anchor1.x+pc->control.x,pc->anchor1.y-pc->control.y);
	//rootObject->UpdateBounds(pc->anchor1.x+pc->control.x,pc->anchor1.y+pc->control.y);
	//rootObject->UpdateBounds(pc->anchor1.x-pc->control.x,pc->anchor1.y+pc->control.y);
	

	pCmd->pCurve=pc;
	pCmd->nCommand=XDrawObj::DC_RECT;

	//m_pDrawObj->AddCommand(pCmd);	
	AddCommand(pCmd);

	m_nCurX=pc->anchor1.x;
	m_nCurY=pc->anchor1.y;
	rootObject->Modify();
	rootObject->mShapeRecord.ReleaseAll();
}

void GraphicsObject::AS3_drawRoundRect(double x,double y,double w,double h,double rx,double ry)
{
	//Add your act code here...	
	if(MathUtils::isNaN(x)||
		MathUtils::isNaN(y)||
		MathUtils::isNaN(w)||
		MathUtils::isNaN(h)||
		MathUtils::isNaN(rx)||
		MathUtils::isNaN(ry)) return;
	if(!m_pDisplay) 
		return;
	if(!m_hasLineOrFill) return;
//#if (__CORE_VERSION__>=0x02070100)
	if(w<=0||h<=0) return;
//#endif
	DisplayList* display=GetDisplay();
	if(!display) return;
	_XCOMMAND*pCmd=display->CreateCommand();//new _XCOMMAND;
	if(!pCmd) return;
	SObject*rootObject=GetSObject();
	
	P_CURVE pc=display->CreateCurve();//new _XCURVE;
	if(!pc)
	{
		display->FreeCommand(pCmd);
		//delete pCmd;
		return;
	}
	
	//if(w==102)
	//	int v=0;
	//rootObject->UpdateBounds(m_nCurX,m_nCurY);

	pc->anchor1.x=x*20;
	pc->anchor1.y=y*20;
	pc->control.x=w*20;
	pc->control.y=h*20;
	if(rx>w) rx=w;
	if(ry>h) ry=h;
	pc->anchor2.x=rx*20;
	pc->anchor2.y=ry*20;

	if(lastAlpha>=1&&
		rootObject->IsOverBounds(pc->anchor1.x+pc->anchor2.x,
								 pc->anchor1.y+pc->anchor2.y,
								 pc->control.x-pc->anchor2.x,
								 pc->control.y-pc->anchor2.y,m_nThick))
		ClearAllActor();

	rootObject->UpdateBounds(pc->anchor1.x,pc->anchor1.y,pc->control.x,pc->control.y,m_nThick);
	//rootObject->UpdateBounds(pc->anchor1.x,pc->anchor1.y,m_nThick);
	//rootObject->UpdateBounds(pc->anchor1.x+pc->control.x,pc->anchor1.y,m_nThick);
	//rootObject->UpdateBounds(pc->anchor1.x+pc->control.x,pc->anchor1.y+pc->control.y,m_nThick);
	//rootObject->UpdateBounds(pc->anchor1.x,pc->anchor1.y+pc->control.y,m_nThick);
	

	pCmd->pCurve=pc;
	if(rx<=0||ry<=0)
		pCmd->nCommand=XDrawObj::DC_RECT;
	else if(rx>=w&&ry>=h)
		pCmd->nCommand=XDrawObj::DC_ELLIPSE;
	else
		pCmd->nCommand=XDrawObj::DC_ROUNDRECT;

	//m_pDrawObj->AddCommand(pCmd);	
	AddCommand(pCmd);

	m_nCurX=pc->anchor1.x+pc->control.x;
	m_nCurY=pc->anchor1.y+pc->control.y-pc->anchor2.y/2;
	rootObject->Modify();
	rootObject->mShapeRecord.ReleaseAll();
}

void GraphicsObject::AS3_endFill()
{
	//Add your act code here...
	if(!m_pDisplay) 
		return;
	if(!(m_hasLineOrFill&HASFILL)) return;
	//StUTF8String ss(m_pDisplay->vtable->traits->name());
	//if(strcmp(ss.c_str(),"ListBase")==0)
	//	int v=0;
	DisplayList*display=GetDisplay();
	if(!display) return;
	_XCOMMAND*pCmd=display->CreateCommand();//new _XCOMMAND;
	if(!pCmd) return;
	
	pCmd->nAttachData=0;
	pCmd->pColor=0;

	//m_bFill=XFALSE;
	lastAlpha=0;

	pCmd->nCommand=XDrawObj::DC_ENDFILL;

	//m_pDrawObj->AddCommand(pCmd);
	AddCommand(pCmd);
	m_hasLineOrFill&=~HASFILL;
}

void GraphicsObject::AS3_lineGradientStyle(Stringp strType,ArrayObject*colors,ArrayObject*alphas,ArrayObject*ratios,MatrixObject*matrix,
										   Stringp spreadMothod,Stringp interMode,double fRatio)
{
	//Add your act code here..
	if(!m_pDisplay) 
		return;	
//#if (__CORE_VERSION__>=0x02081000)
	_beginFill(strType,colors,alphas,ratios,matrix,spreadMothod,interMode,fRatio,m_nThick / 20 );
//#else
//	_beginFill(strType,colors,alphas,ratios,matrix,spreadMothod,interMode,fRatio,1);
//#endif
}

void GraphicsObject::AS3_lineStyle(double thick,uint32_t c,double alpha,bool pixelHinting,Stringp scaleMode,Stringp caps,Stringp jonts,double miterLimit)
{
	if( thick > 255)
		thick = 255;
	//Add your act code here...	
	//if(thick<=0||alpha<=0) return;
	if(!m_pDisplay) 
		return;
	if(alpha<=0)
	{
		if((m_hasLineOrFill&HASLINE))
		{
			DisplayList*display=GetDisplay();
			if(!display) return;
			_XCOMMAND*pCmd=display->CreateCommand();//new _XCOMMAND;
			if(!pCmd) return;
			
			pCmd->nAttachData=0;
			pCmd->nLineData=0;
			pCmd->pColor=0;
			pCmd->nCommand=XDrawObj::DC_ENDLINE;
			//m_pDrawObj->AddCommand(pCmd);
			AddCommand(pCmd);
			m_hasLineOrFill&=~HASLINE;
		}
		return;
	}
	m_nThick = thick * 20;
	
	_beginFill(c,alpha,XMAX(m_nThick,1));
}

void GraphicsObject::AS3_lineTo(double x,double y)
{
	if(MathUtils::isNaN(x)||MathUtils::isNaN(y)) return;
	if(!m_pDisplay) 
		return;
	if(!m_hasLineOrFill) 
		return;
	
	_lineTo(x*20,y*20,false);
}

void GraphicsObject::_lineTo(int x,int y,bool bMove)
{
	//Add your act code here...
	if(!m_pDisplay) 
		return;
	DisplayList* display=GetDisplay();
	if(!display) return;
//#if (__CORE_VERSION__>=0x02070100)
	if(m_nCurX==x&&
	   m_nCurY==y) return;
//#endif
	SObject*rootObject=GetSObject();
	_XCOMMAND*pCmd=display->CreateCommand();//new _XCOMMAND;
	if(!pCmd) return;
	
/*	if(display->antialias)
	{
		x<<=2;
		y<<=2;
	}*/

	P_CURVE pc=display->CreateCurve();//new _XCURVE;
	if(!pc)
	{
		//delete pCmd;
		display->FreeCommand(pCmd);
		return;
	}
	
	rootObject->UpdateBounds(m_nCurX,m_nCurY,m_nThick);
	rootObject->UpdateBounds(x,y,m_nThick);

	pc->isLine=true;
	pc->anchor1.x=m_nCurX;
	pc->anchor1.y=m_nCurY;
	pc->anchor2.x=x;
	pc->anchor2.y=y;

	PointAverage(&pc->anchor1, &pc->anchor2, &pc->control);

	pCmd->pCurve=pc;
	pCmd->nCommand=bMove?XDrawObj::DC_MOVETO:XDrawObj::DC_CURVE;

//	m_pDrawObj->AddCommand(pCmd);

	m_nCurX=x;
	m_nCurY=y;
//	m_pDrawObj->Modify();

	AddCommand(pCmd);
	rootObject->Modify();
	rootObject->mShapeRecord.ReleaseAll();
}

void GraphicsObject::AS3_moveTo(double x,double y)
{
	//Add your act code here...
	if(MathUtils::isNaN(x)||MathUtils::isNaN(y))
		return;
	if(!m_pDisplay) 
		return;
	if(!m_hasLineOrFill)
		return;
	_lineTo(x*20,y*20,true);
}

//new adds
void GraphicsObject::AS3_beginShaderFill(ShaderObject* pShader, MatrixObject* pMatrix)
{
	if(!m_pDisplay) 
		return;	
}
void GraphicsObject::AS3_copyFrom(GraphicsObject *pSourceGraphics)
{
	if(!m_pDisplay) 
		return;
}
//void GraphicsObject::AS3_drawGraphicsData(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
//{
//
//}
void GraphicsObject::AS3_drawPath(ObjectVectorObject *pCommands, DoubleVectorObject *pData, Stringp winding)
{
	if(!m_pDisplay) 
		return;
}
void GraphicsObject::AS3_drawTriangles(DoubleVectorObject *pVertices, IntVectorObject *pIndices, DoubleVectorObject *pUvtData, Stringp culling)
{
	if(!m_pDisplay) 
		return;
}
void GraphicsObject::AS3_lineBitmapStyle(BitmapDataObject *pBitmap, MatrixObject* pMatrix, bool repeat, bool smooth)
{
	if(!m_pDisplay) 
		return;
}
void GraphicsObject::AS3_lineShaderStyle(ShaderObject *pShader, MatrixObject* pMatrix)
{
	if(!m_pDisplay) 
		return;
}
//new adds end


//new adds 11
void GraphicsObject::AS3_cubicCurveTo(double controlX1, double controlY1, double controlX2, double controlY2, double anchorX, double anchorY)
{
	LOGWHERE();

}
IGraphicsDataVectorObject* GraphicsObject::AS3_readGraphicsData(bool recurse)
{
	LOGWHERE();
	return NULL;
}
//new adds 11 end
}
