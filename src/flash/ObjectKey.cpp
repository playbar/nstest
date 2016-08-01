// ObjectKey.cpp: implementation of the XXObjectKey class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "ObjectKey.h"
#include "ObjectCreator.h"
#include "splayer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//#define XXKEYCLASS(TOKEN) XXCLASS(TOKEN,XXObjectKey,XXOBJ_KEY)

XXObjectKey::XXObjectKey(ScriptPlayer*pRoot):XXObject(pRoot)
{
	m_bNoCase=XTRUE;
}

XXObjectKey::~XXObjectKey()
{

}


XBOOL XXObjectKey::SysGetMember(int id, XSWFCONTEXT *pCnt,XXVar &var)
{
	switch(id)
	{
	case _SYSID(_listeners):
		{ XXObjectArray*pArray=m_pRoot->m_pGlobal->CreateArray();
			if(pArray)
			{
				//XFUNLIST&list=cnt.pPlayer->GetListeners(XSWFPlayer::XLS_KEY);
				_XEVENT*p=m_pEvents;
				while(p)
				//for(XU32 i=0;i<list.GetSize();i++)
				{
				   pArray->m_list.Push(p->pHandler);//new XXVar(p->pHandler));
				   p=p->pNext;
				}
			}
		    var=pArray;
		}
		 return XTRUE;
	case _SYSID(addListener):
	case _SYSID(getAscii):
	case _SYSID(getCode):
	case _SYSID(isAccessible):
	
	case _SYSID(isDown):
	case _SYSID(isToggled):
	case _SYSID(removeListener):
		 var=this;
		  var.ToFunction(id);
		  return XTRUE;
	}
    return XXObject::SysGetMember(id,pCnt,var);
}
XBOOL XXObjectKey::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{
	int nSize=list.GetSize();
	switch(id)
	{
		case _SYSID(addListener):
			 if(nSize<=0||!list[0].IsObject(XXOBJ_OBJECT)) return XTRUE;
			 if(list[0].IsObject(XXOBJ_CLIP)) return XTRUE;
			 if(list[0].IsObject(XXOBJ_BUTTON)) return XTRUE;
			 XXObject::AddListener(pCnt->baseThread,list[0].pObject);
			 return XTRUE;
		case _SYSID(getAscii):
			 pCnt->pStack->Push(pCnt->pPlayer->nLastKey);
			 return XTRUE;
		case _SYSID(getCode):
			 pCnt->pStack->Push(pCnt->pPlayer->nLastKey);
			 return XTRUE;
		case _SYSID(isAccessible):
			 pCnt->pStack->PushBool(XTRUE);
			 return XTRUE;
		case _SYSID(isDown):
			{
			 	XBOOL bOK=XFALSE;
				if(pCnt->pPlayer->nKeyStatus&&list.GetSize()>0)
				{
					list[0].ToInt();
 					if(pCnt->pPlayer->IsKeyDown(list[0].iData32))
						bOK=XTRUE;
				}
				pCnt->pStack->PushBool(bOK);
			}
			 return XTRUE;
		case _SYSID(isToggled):
			 pCnt->pStack->PushBool(XFALSE);
			 return XTRUE;
		case _SYSID(removeListener):
			 if(nSize&&list[0].IsObject())
				 XXObject::RemoveListener(list[0].pObject);
			 return XTRUE;
	}
	return XXObject::SysCallMethod(id,pCnt,list);
}

void XXObjectKey::InitObject()
{
	_SYSCALL(addListener);//(XSWFCONTEXT&cnt,XXVARLIST&list);
	_SYSCALL(getAscii);//(XSWFCONTEXT&cnt,XXVARLIST&list);
	_SYSCALL(getCode);//(XSWFCONTEXT&cnt,XXVARLIST&list);
	_SYSCALL(isAccessible);//(XSWFCONTEXT&cnt,XXVARLIST&list);
	_SYSCALL(isDown);//(XSWFCONTEXT&cnt,XXVARLIST&list);
	_SYSCALL(isToggled);//(XSWFCONTEXT&cnt,XXVARLIST&list);
	_SYSCALL(removeListener);//(XSWFCONTEXT&cnt,XXVARLIST&list);
	_SYSCALL(_listeners);//Get),XNULL);

	AddMember(_SYSVAR(BACKSPACE),XXVar(XXBACKSPACE));//	=8),
	AddMember(_SYSVAR(CAPSLOCK),XXVar(XXCAPSLOCK));//	=20),
	AddMember(_SYSVAR(CONTROL),XXVar(XXCONTROL));//	=17),
	AddMember(_SYSVAR(DELETEKEY),XXVar(XXDELETE));//	=46),
	AddMember(_SYSVAR(DOWN),XXVar(XXDOWN));//	XXDOWN		=40),
	AddMember(_SYSVAR(END),XXVar(XXEND));//		=35),
	AddMember(_SYSVAR(ENTER),XXVar(XXENTER));//		=13),
	AddMember(_SYSVAR(ESCAPE),XXVar(XXESCAPE));//	=27),
	AddMember(_SYSVAR(HOME),XXVar(XXHOME));//		=36),
	AddMember(_SYSVAR(INSERT),XXVar(XXINSERT));//	=45),
	AddMember(_SYSVAR(LEFT),XXVar(XXLEFT));//		=37),
	AddMember(_SYSVAR(PGDN),XXVar(XXPAGEDOWN));//=34),
	AddMember(_SYSVAR(PGUP),XXVar(XXPAGEUP));//	=33),
	AddMember(_SYSVAR(RIGHT),XXVar(XXRIGHT));//		=39),
	AddMember(_SYSVAR(SHIFT),XXVar(XXSHIFT));//		=16),
	AddMember(_SYSVAR(SPACE),XXVar(XXSPACE));//		=32),
	AddMember(_SYSVAR(TAB),XXVar(XXTAB));//		=9),
	AddMember(_SYSVAR(UP),XXVar(XXUP));//		=38
	
}



XXObject* XXObjectKey::CreateObject(XSWFCONTEXT *pCnt,XXVARLIST&,XBOOL)
{
   XXObjectKey*pObj=new XXObjectKey(m_pRoot);
   if(pObj)
   {
	   pObj->m_pSuperObj=GetSuperObject();
	   pObj->SetSuper(this,XFALSE);
   }
   pCnt->pStack->Push(pObj);//new XXVar(pObj));
   return pObj;

}

//DEL XXObjectKey* XXObjectKey::CreateClass(XXObject *pSuper, XU8 bInit)
//DEL {
//DEL 	XXObjectKey*pObj=new XXObjectKey();
//DEL 	if(pObj)
//DEL 	{
//DEL 		if(bInit) pObj->InitObject();
//DEL 		pObj->SetSuper(pSuper,bInit);
//DEL 		pObj->m_bBase=bInit;
//DEL 	}
//DEL 	return pObj;
//DEL }
