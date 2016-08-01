// ObjectButton.cpp: implementation of the ObjectButton class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "ObjectButton.h"
#include "ObjectCreator.h"
#include "splayer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//#define XXBUTTONCLASS(TOKEN) XXCLASS(TOKEN,XXObjectButton,XXOBJ_BUTTON);
//XXBUTTONFUN(removeMovieClip);

XBOOL XXObjectButton::SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	if(m_pObject)
	{
		switch(id)
		{
		case _SYSID(_alpha):
			 var=m_pObject->GetAlpha();
			 return XTRUE;
		case _SYSID(blendMode):
			var=m_pObject->blendMode;
			return XTRUE;
		case _SYSID(cacheAsBitmap):
			var=m_pObject->cacheMode;
			return XTRUE;
		case _SYSID(enabled):
			var=m_pObject->IsEnable();
			return XTRUE;
		case _SYSID(filters):
			return XTRUE;
		case _SYSID(_focusrect):
		case _SYSID(_highquality):
		case _SYSID(menu):
		case _SYSID(_quality):
		case _SYSID(scale9Grid):
		case _SYSID(tabEnabled):
		case _SYSID(tabIndex):
		case _SYSID(trackAsMenu):
		case _SYSID(useHandCursor):
			return XTRUE;
		case _SYSID(_name):
			var=m_pObject->name;
			return XTRUE;
		case _SYSID(_parent):
			var=GetParent();
			return XTRUE;
		case _SYSID(_rotation):
			var=m_pObject->GetRotate();
			return XTRUE;		
		case _SYSID(_soundbuftime):
			var=pCnt->pPlayer->nSoundBufferTime;
			return XTRUE;
		case _SYSID(_target):
			pCnt->pPlayer->GetTargetPath(m_pObject,var);
			return XTRUE;
		case _SYSID(_url):
			if(m_pObject->GetPlayer())
			var=m_pObject->GetPlayer()->m_url;
			return XTRUE;		
		case _SYSID(_visible):
			var=m_pObject->IsVisible();
			return XTRUE;
		case _SYSID(_width):
			var=m_pObject->GetWidth();
			return XTRUE;
		case _SYSID(_height):
			var=m_pObject->GetHeight();
			return XTRUE;
		case _SYSID(_x):
			var=m_pObject->GetPosX();
			return XTRUE;
		case _SYSID(_y):
			var=m_pObject->GetPosY();
			return XTRUE;
		case _SYSID(_xmouse):
			{
				double x,y;
				pCnt->pPlayer->GetMousePos(m_pObject,x,y);
				var=x;
			}
			return XTRUE;
		case _SYSID(_ymouse):
			{
				double x,y;
				pCnt->pPlayer->GetMousePos(m_pObject,x,y);
				var=y;
			}
			return XTRUE;
		case _SYSID(_xscale):
			var=m_pObject->GetScaleX();
			return XTRUE;
		case _SYSID(_yscale):
			var=m_pObject->GetScaleY();
			return XTRUE;
		case _SYSID(getDepth):
			var=this;
			var.ToFunction(id);
			return XTRUE;
		}
	}
	return XXObject::SysSetMember(id,pCnt,var);
}
XBOOL XXObjectButton::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{
	switch(id)
	{
	case _SYSID(getDepth):
		if(m_pObject)
			pCnt->pStack->Push(m_pObject->depth);
		return XTRUE;
	}
	return XXObject::SysCallMethod(id,pCnt,list);
}
XBOOL XXObjectButton::SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	if(m_pObject)
	{
		switch(id)
		{
		case _SYSID(_alpha):
			 m_pObject->SetAlpha(var.ToFloat());
			 return XTRUE;
		case _SYSID(blendMode):
			m_pObject->blendMode=var.ToInt();
			return XTRUE;
		case _SYSID(cacheAsBitmap):
			m_pObject->cacheMode=var.ToInt();
			return XTRUE;
		case _SYSID(enabled):
			m_pObject->SetEnable(var.ToLogic());
			//var=m_pObject->IsEnable();
			return XTRUE;
		case _SYSID(filters):
			return XTRUE;
		case _SYSID(_focusrect):
		case _SYSID(_highquality):
		case _SYSID(menu):
		case _SYSID(_quality):
		case _SYSID(scale9Grid):
		case _SYSID(tabEnabled):
		case _SYSID(tabIndex):
		case _SYSID(trackAsMenu):
		case _SYSID(useHandCursor):
			return XTRUE;
		case _SYSID(_name):
			//var=m_pObject->name;
			//var.ToString(XFALSE);
			//FreeStr(m_pObject->name);
			m_pObject->name=var;//CreateStr(var.strTxt);
			return XTRUE;
		case _SYSID(_parent):			
			return XTRUE;
		case _SYSID(_rotation):
			m_pObject->SetRotate(var.ToFloat());
			return XTRUE;		
		case _SYSID(_soundbuftime):
			pCnt->pPlayer->nSoundBufferTime=var.ToInt();
			return XTRUE;
		case _SYSID(_target):
			//pCnt->pPlayer->GetTargetPath(m_pObject,var);
			return XTRUE;
		case _SYSID(_url):
			//var=m_pObject->character->player->url;
			return XTRUE;		
		case _SYSID(_visible):
			m_pObject->SetVisible(var.ToLogic());
			//var=m_pObject->IsVisible();
			return XTRUE;
		case _SYSID(_width):
			m_pObject->SetWidth(var.ToFloat());
			//var=m_pObject->GetWidth();
			return XTRUE;
		case _SYSID(_height):
			m_pObject->SetHeight(var.ToFloat());
			//var=m_pObject->GetHeight();
			return XTRUE;
		case _SYSID(_x):
			m_pObject->SetPosX(var.ToFloat());
			//var=m_pObject->GetPosX();
			return XTRUE;
		case _SYSID(_y):
			m_pObject->SetPosY(var.ToFloat());
			//var=m_pObject->GetPosY();
			return XTRUE;
		case _SYSID(_xmouse):
			return XTRUE;
		case _SYSID(_ymouse):
			return XTRUE;
		case _SYSID(_xscale):
			m_pObject->SetScaleX(var.ToFloat());
			//var=m_pObject->GetScaleX();
			return XTRUE;
		case _SYSID(_yscale):
			m_pObject->SetScaleY(var.ToFloat());
			//var=m_pObject->GetScaleY();
			return XTRUE;
		case _SYSID(getDepth):
			return XTRUE;
		}
	}
	return XXObject::SysSetMember(id,pCnt,var);
}

void XXObjectButton::InitObject()
{
	_SYSCALL(_alpha);//_strAlpha,XXFUNOBJ(alphaGet),XXFUNOBJ(alphaSet));
	_SYSCALL(blendMode);
	_SYSCALL(cacheAsBitmap);//_strEnable,XXFUNOBJ(enableGet),XXFUNOBJ(enableSet));
	_SYSCALL(enabled);//_strName,XXFUNOBJ(nameGet),XXFUNOBJ(nameSet));
	_SYSCALL(filters);//_strParent,XXFUNOBJ(parentGet),XNULL);
	_SYSCALL(_focusrect);//_strRotate,XXFUNOBJ(rotateGet),XXFUNOBJ(rotateSet));
	_SYSCALL(_height);//_strTarget,XXFUNOBJ(targetGet),XNULL);
	_SYSCALL(_highquality);//_strURL,XXFUNOBJ(urlGet),XNULL);
	_SYSCALL(menu);//_strVisiable,XXFUNOBJ(visiableGet),XXFUNOBJ(visiableSet));
	_SYSCALL(_name);//_strWidth,XXFUNOBJ(widthGet),XXFUNOBJ(widthSet));
	_SYSCALL(_parent);//_strHeight,XXFUNOBJ(heightGet),XXFUNOBJ(heightSet));
	_SYSCALL(_quality);//_strX,XXFUNOBJ(xGet),XXFUNOBJ(xSet));
	_SYSCALL(_rotation);//_strY,XXFUNOBJ(yGet),XXFUNOBJ(ySet));
	_SYSCALL(scale9Grid);//_strXMouse,XXFUNOBJ(xMouseGet),XNULL);
	_SYSCALL(_soundbuftime);//_strYMouse,XXFUNOBJ(yMouseGet),XNULL);
	_SYSCALL(tabEnabled);//_strXScale,XXFUNOBJ(xScaleGet),XXFUNOBJ(xScaleSet));
	_SYSCALL(tabIndex);//_strYScale,XXFUNOBJ(yScaleGet),XXFUNOBJ(yScaleSet));
	_SYSCALL(_target);
	_SYSCALL(trackAsMenu);
	_SYSCALL(_url);
	_SYSCALL(useHandCursor);
	_SYSCALL(_visible);
	_SYSCALL(_width);
	_SYSCALL(_height);
	_SYSCALL(_x);
	_SYSCALL(_y);
	_SYSCALL(_xmouse);
	_SYSCALL(_ymouse);
	_SYSCALL(_xscale);
	_SYSCALL(_yscale);
	_SYSCALL(getDepth);
}


//DEL XBOOL XXObjectButton::GetMousePos(XSWFCONTEXT&cnt,int &x, int &y)
//DEL {
//DEL 	//XNEEDOBJ(m_pObject);
//DEL 	/*XSWFMatrix*pMatrix=m_pObject->m_pDisp->m_pPlaceMatrix;
//DEL 	if(pMatrix==XNULL) return XFALSE;
//DEL 	x=cnt.nPointX;
//DEL 	y=cnt.nPointY;
//DEL 	x<<=ANTIZOOM;
//DEL 	y<<=ANTIZOOM;
//DEL 	pMatrix->RevTrans(x,y);
//DEL 	
//DEL 	return cnt.nPointType;*/
//DEL 	return cnt.pPlayer->GetMousePos(m_pObject,x,y);
//DEL }

XBOOL XXObjectButton::GetClipOf(XPCTSTR strName, XXVar&var)
{
	if(!m_pObject) return XFALSE;
	SObject*pObj=m_pObject->GetChildOf(strName,XTRUE);
	if(pObj==XNULL) return XFALSE;
	var=pObj->pObject;
	return XTRUE;
	//XNEEDOBJ(m_pObject);
	//return m_pObject->GetClipOf(strName,*pVar);
}

//DEL XBOOL XXObjectButton::InstallEvent(XPCWSTR strName, XXObject *pFun)
//DEL {
//DEL    XSortConstStr16 sort((XPCWSTR*)_strEvents,XOBJEVENT_UNLOAD+1);
//DEL    int id=sort.Index((void*)strName);
//DEL    if(id<0) return XFALSE;
//DEL    if(id>=(int)m_events.GetSize())
//DEL    {
//DEL 	   int s=m_events.GetSize();
//DEL 	   m_events.SetSize(id+1);
//DEL 	   for(int i=s;i<id+1;i++)
//DEL 		   m_events[i]=XNULL;
//DEL    }
//DEL    m_events[id]=pFun;
//DEL    //m_events.InsertAt(id,pFun);
//DEL    return XTRUE;
//DEL }

//DEL XBOOL XXObjectButton::RemoveEvent(XPCWSTR strName)
//DEL {
//DEL    XSortConstStr16 sort((XPCWSTR*)_strEvents,XOBJEVENT_UNLOAD+1);
//DEL    int id=sort.Index((void*)strName);
//DEL    if(id<0||id>(int)m_events.GetSize()) return XFALSE;
//DEL    m_events[id]=XNULL;	
//DEL    return XTRUE;
//DEL }

//DEL XBOOL XXObjectButton::DoEvent(XSWFCONTEXT *pCnt, XU32 id)
//DEL {
//DEL    if(id>=m_events.GetSize()) return XFALSE;
//DEL    if(m_events[id]==XNULL) return XFALSE;
//DEL    XU8 bThis=!(m_bBase&1)&&pCnt->GetThis()!=this;
//DEL    XDTSpriteDisp*pTarget=pCnt->pContext;
//DEL    pCnt->pContext=GetTarget(pCnt);
//DEL    if(pCnt->pCaller)
//DEL    {
//DEL 	   bThis=XTRUE;
//DEL 	   pCnt->thises.Add(pCnt->pCaller);
//DEL    }
//DEL    else if(bThis)
//DEL 	   pCnt->thises.Add(this);
//DEL    XXVARLIST list;
//DEL    XXObject*pObj=m_events[id]->RefObject();
//DEL    pObj->Construct(pCnt,&list,pCnt->pCaller?pCnt->pCaller:this);
//DEL    pObj->FreeObject();
//DEL    pCnt->pStack->FreePop();
//DEL    if(bThis) pCnt->thises.RemoveLast();
//DEL    pCnt->pContext=pTarget;
//DEL    return XTRUE;
//DEL }

XXObject* XXObjectButton::GetParent()
{
	if(m_pObject==XNULL) return XNULL;//eturn m_pContext;

	if(m_pObject->parent==XNULL) return XNULL;
	return m_pObject->parent->pObject;

	//XDTSpriteDisp*p=m_pObject->GetParent();
	//if(p) return p->GetObject();
	//else  return m_pParent;
}

XXObject* XXObjectButton::CreateObject(XSWFCONTEXT*pCnt,XXVARLIST&list,XBOOL bObj)
{
	pCnt->pStack->PushConst(XOCT_NULL);
	return XNULL;
}

//DEL void XXObjectButton::removeMovieClip(XSWFCONTEXT &cnt, XXVARLIST &list)
//DEL {
//DEL 	if(!m_pObject) return;
//DEL 	m_pObject->parent->thread->RemoveDepth(m_pObject->depth);
//DEL }
