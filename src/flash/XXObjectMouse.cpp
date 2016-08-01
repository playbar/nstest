// XXObjectMouse.cpp: implementation of the XXObjectMouse class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XXObjectMouse.h"
#include "splayer.h"
#include "ObjectKey.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XXObjectMouse::XXObjectMouse(ScriptPlayer*pRoot):XXObject(pRoot)
{
   m_bNoCase=XTRUE;
}

XXObjectMouse::~XXObjectMouse()
{

}

XBOOL XXObjectMouse::SysGetMember(int id, XSWFCONTEXT *pCnt, XXVar &var)
{
	switch(id)
	{
	case _SYSID(addListener):
	case _SYSID(removeListener):
	case _SYSID(show):
	case _SYSID(hide):
		 var=this;
		 var.ToFunction(id);
		 return XTRUE;
	}
    return XXObject::SysGetMember(id,pCnt,var);;
}
XBOOL XXObjectMouse::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{
	int nSize=list.GetSize();
	switch(id)
	{
	case _SYSID(addListener):
		 if(nSize&&list[0].IsObject())
			 XXObject::AddListener(pCnt->baseThread,list[0].pObject);
		 pCnt->pStack->PushBool(XTRUE);
		 return XTRUE;
	case _SYSID(removeListener):
		 if(nSize&&list[0].IsObject())
			 XXObject::RemoveListener(list[0].pObject);
		 pCnt->pStack->PushBool(XTRUE);
		 return XTRUE;
	case _SYSID(show):
		 pCnt->pStack->Push(pCnt->pPlayer->cursorShow);
		 pCnt->pPlayer->ShowCursor(XTRUE);
		 return XTRUE;
	case _SYSID(hide):
		 pCnt->pStack->Push(pCnt->pPlayer->cursorShow);
		 pCnt->pPlayer->ShowCursor(XFALSE);
		 return XTRUE;
	}
	return XXObject::SysCallMethod(id,pCnt,list);
}


void XXObjectMouse::InitObject( )
{
	_SYSCALL(addListener);
	_SYSCALL(removeListener);
	_SYSCALL(show);
	_SYSCALL(hide);
	//AddMember(XXVar(_strAddListener),XXVar(XXFUNOBJ(addListenerMouse)));
	//AddMember(XXVar(_strRemoveListener),XXVar(XXFUNOBJ(removeListenerMouse)));
	//AddMember(XXVar(_strShow),XXVar(XXFUNOBJ(showMouse)));
	//AddMember(XXVar(_strHide),XXVar(XXFUNOBJ(hideMouse)));
}



XXObject* XXObjectMouse::CreateObject(XSWFCONTEXT *pCnt, XXVARLIST &list,XBOOL)
{
	XXObjectMouse*pObj=CreateClass(this,XFALSE,m_pRoot);
	if(pObj)
		pObj->m_pSuperObj=GetSuperObject();
	pCnt->pStack->Push((pObj));
	return pObj;
}
