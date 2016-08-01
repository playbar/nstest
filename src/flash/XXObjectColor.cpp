// XXObjectColor.cpp: implementation of the XXObjectColor class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XXObjectColor.h"
#include "ObjectClip.h"
#include "splayer.h"
#include "ObjectCreator.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XXObjectColor::XXObjectColor(ScriptPlayer*pRoot):XXObject(pRoot)
{
	m_pClip=XNULL;
	m_bNoCase=XTRUE;
}

XXObjectColor::~XXObjectColor()
{
   FREEOBJECT(m_pClip);
}

XXObject* XXObjectColor::CreateObject(XSWFCONTEXT *pCnt, XXVARLIST &list,XBOOL)
{
	XXObjectColor*pColor=CreateClass(this,XFALSE,m_pRoot);

	if(pColor)
	{
		pColor->m_pSuperObj=GetSuperObject();
		if(list.GetSize()>0&&list[0].IsObject())
		{
			XXObjectClip*p=(XXObjectClip*)list[0].pObject;
			if(p->IsObject(XXOBJ_CLIP))
			  pColor->m_pClip=(XXObjectClip*)REFOBJECT(p);
		}
	}
	pCnt->pStack->Push((pColor));
	return pColor;
}

void XXObjectColor::Destruct()
{
	FREEOBJECT(m_pClip);
}

XU32 XXObjectColor::Construct(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*)
{
	if(list.GetSize()>0&&list[0].IsObject())
	{
		XXObjectClip*p=(XXObjectClip*)list[0].pObject;
		if(p->IsObject(XXOBJ_CLIP))
		{
		  FREEOBJECT(m_pClip);
		  m_pClip=(XXObjectClip*)REFOBJECT(p);
		}
	}
	pCnt->pStack->Push(this);
	return (XU32)this;
}


XBOOL XXObjectColor::SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	switch(id)
	{
	case _SYSID(getRGB):
	case _SYSID(setRGB):
	case _SYSID(getTransform):
	case _SYSID(setTransform):
		 var=this;
		 var.ToFunction(id);
		 return XTRUE;
	}
	return XXObject::SysGetMember(id,pCnt,var);
}

XBOOL XXObjectColor::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{
	int nSize=list.GetSize();
	switch(id)
	{
	case _SYSID(getRGB):
#ifdef _ARGB_PIXEL_
		{
			int color=m_pClip->GetForeColor();
			SWAPPIXELRB(color);
			pCnt->pStack->Push(color);
		}
#else
		 pCnt->pStack->Push((int)(m_pClip->GetForeColor()));
#endif
		 return XTRUE;
	case _SYSID(setRGB):
		 if(m_pClip&&nSize)
		 {
			XU32 color=list[0].ToInt();
#ifdef _ARGB_PIXEL_
			SWAPPIXELRB(color);
#endif
			if(!(color>>24)) color|=0xff000000;
			m_pClip->SetForeColor(color);
		 }
		 return XTRUE;
	case _SYSID(getTransform):
		if(m_pClip)
		{
		  XXObjectColorTransform*p=m_pRoot->m_pGlobal->CreateColorTransform();
		  if(p)
			m_pClip->GetCXForm(p->cxForm);
		  pCnt->pStack->Push(p);
		}return XTRUE;

	case _SYSID(setTransform):
		if(nSize&&m_pClip&&list[0].IsObject(XXOBJ_COLORTRANSFORM))
		{
			
			XXObjectColorTransform*p=(XXObjectColorTransform*)list[0].pObject;
			m_pClip->SetCXForm(p->cxForm);
		}return XTRUE;

	}
	return XXObject::SysCallMethod(id,pCnt,list);
}


void XXObjectColor::InitObject( )
{
	_SYSCALL(getRGB);
	_SYSCALL(setRGB);
	_SYSCALL(getTransform);
	_SYSCALL(setTransform);
   //AddMember(XXVar(_strGetRGB),XXVar(XXFUNOBJ(getRGB)));
   //AddMember(XXVar(_strSetRGB),XXVar(XXFUNOBJ(setRGB)));
   //AddMember(XXVar(_strGetTransform),XXVar(XXFUNOBJ(getTransform)));
   //AddMember(XXVar(_strSetTransform),XXVar(XXFUNOBJ(setTransform)));
}

