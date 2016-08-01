// XXObjectTransform.cpp: implementation of the XXObjectTransform class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XXObjectTransform.h"
#include "ObjectCreator.h"
#include "splayer.h"
#include "ObjectClip.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XXObjectTransform::XXObjectTransform(ScriptPlayer*pRoot):XXObject(pRoot)
{
	m_pClip=XNULL;
}

XXObjectTransform::~XXObjectTransform()
{
	FREEOBJECT(m_pClip);
}

XXObject* XXObjectTransform::CreateObject(XSWFCONTEXT *pCnt, XXVARLIST &list,XBOOL)
{
	//return 0;
	XXObjectTransform*p=CreateClass(/*m_bBase?this:m_pSuper*/this,XFALSE,m_pRoot);
	if(p)
		p->m_pSuperObj=GetSuperObject();
	if(p&&list.GetSize()>0&&list[0].IsObject())
	{
		XXObjectClip*pm=(XXObjectClip*)list[0].pObject;
		if(pm->IsObject(XXOBJ_CLIP))
		  p->m_pClip=(XXObjectClip*)REFOBJECT(pm);
	}
	pCnt->pStack->Push((p));
	return p;
}

void XXObjectTransform::SetClip(XXObjectClip*p)
{
	FREEOBJECT(m_pClip);
	m_pClip=(XXObjectClip*)REFOBJECT(p);
}

XBOOL XXObjectTransform::SysGetMember(int id, XSWFCONTEXT *pCnt,XXVar &var)
{
	switch(id)
	{
	case _SYSID(concatenatedColorTransform):
	case _SYSID(colorTransform):
		 if(m_pClip)
		 {
			XXObjectColorTransform*p=m_pRoot->m_pGlobal->CreateColorTransform();
			if(p)
				m_pClip->GetCXForm(p->cxForm);
			var=p;
		 }return XTRUE;
	
	case _SYSID(matrix):
		 if(m_pClip)
		 {
			XXObjectMatrix*p=m_pRoot->m_pGlobal->CreateMatrix();
			if(p)
				m_pClip->GetMatrix(p->matrix);
			var=p;
		 }return XTRUE;
	case _SYSID(concatenatedMatrix):
		 if(m_pClip)
		 {
			XXObjectMatrix*p=m_pRoot->m_pGlobal->CreateMatrix();
			if(p)
				m_pClip->GetTotalMatrix(p->matrix);
			var=p;
		 }
	     return XTRUE;
	case _SYSID(pixelBounds):
		 if(m_pClip)
		 {
			XXObjectRectangle*p=m_pRoot->m_pGlobal->CreateRectangle();
			if(p)
			{	
				XRect rect;
				m_pClip->GetPixelRect((SRECT&)rect);
				p->left=(float)rect.left;///ZOOMSTEP;
				p->top=(float)rect.top;///ZOOMSTEP;
				p->width=(float)rect.Width();///ZOOMSTEP;
				p->height=(float)rect.Height();///ZOOMSTEP;
				RSHIFTF(p->left,ANTIZOOM);
				RSHIFTF(p->top,ANTIZOOM);
				RSHIFTF(p->width,ANTIZOOM);
				RSHIFTF(p->height,ANTIZOOM);
			}
			var=p;
		 }return XTRUE;
	}
    return XXObject::SysGetMember(id,pCnt,var);;
}

XBOOL XXObjectTransform::SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	switch(id)
	{	
	case _SYSID(colorTransform):
		 if(m_pClip)
		 {		
			if(var.IsObject(XXOBJ_COLORTRANSFORM))
			{
				XXObjectColorTransform*p=(XXObjectColorTransform*)var.pObject;				
				m_pClip->SetCXForm(p->cxForm);
			}
		 }return XTRUE;
	
	case _SYSID(matrix):
		 if(m_pClip&&var.IsObject(XXOBJ_MATRIX))
		 {
			XXObjectMatrix*p=(XXObjectMatrix*)var.pObject;
			if(p)
				m_pClip->SetMatrix(p->matrix);
		 }return XTRUE;
	case _SYSID(pixelBounds):
		 if(m_pClip&&var.IsObject())
		 {
			XXObjectRectangle*p=(XXObjectRectangle*)var.pObject;
			XRECT rect;
			double l=p->left;//*ZOOMSTEP;
			double t=p->top;//*ZOOMSTEP;
			rect.left=XROUND(l);
			rect.top=XROUND(t);
			double r=(p->left+p->width);//*ZOOMSTEP;
			double b=(p->top+p->height);//*ZOOMSTEP;
			rect.right=XROUND(r);
			rect.bottom=XROUND(b);
			m_pClip->SetPixelRect((SRECT&)rect);
		 }return XTRUE;
	}	
	return XXObject::SysSetMember(id,pCnt,var);
}

void XXObjectTransform::InitObject()
{
	_SYSCALL(colorTransform);
	_SYSCALL(concatenatedColorTransform);
	_SYSCALL(concatenatedMatrix);
	_SYSCALL(matrix);
	_SYSCALL(pixelBounds);
	//AddProperty(XXVar(_strColorTransforms),XXFUNOBJ(colorTransformGet),XXFUNOBJ(colorTransformSet));
	//AddProperty(XXVar(_strConcatenatedColorTransform),XXFUNOBJ(concatenatedColorTransformGet),XNULL);
	//AddProperty(XXVar(_strMatrixs),XXFUNOBJ(matrixGet),XXFUNOBJ(matrixSet));
	//AddProperty(XXVar(_strConcatenatedMatrix),XXFUNOBJ(concatenatedMatrixGet),XNULL);
	//AddProperty(XXVar(_strPixelBounds),XXFUNOBJ(pixelBoundsGet),XNULL);//XXFUNOBJ(pixelBoundsSet));
}


XU32 XXObjectTransform::Construct(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*)
{
	if(list.GetSize())
	{
		XXObjectClip*pm=(XXObjectClip*)list[0].pObject;
		if(pm->IsObject(XXOBJ_CLIP))
		{
			FREEOBJECT(m_pClip);
			m_pClip=(XXObjectClip*)REFOBJECT(pm);
		}
	}
	pCnt->pStack->Push(this);
	return (XU32)this;
}


//DEL void XXObjectTransform::Destroy()
//DEL {
//DEL 	XXObject::Destroy();
//DEL 	FREEOBJECT(m_pClip);
//DEL }

//DEL void XXObjectTransform::Release()
//DEL {
//DEL 	XXObject::Release();
//DEL 	FREEOBJECT(m_pClip);
//DEL }

void XXObjectTransform::Destruct()
{
	FREEOBJECT(m_pClip);
}
