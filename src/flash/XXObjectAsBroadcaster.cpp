// XXObjectAsBroadcaster.cpp: implementation of the XXObjectAsBroadcaster class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XXObjectAsBroadcaster.h"
#include "ObjectArray.h"
#include "ObjectCreator.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XXObjectAsBroadcaster::XXObjectAsBroadcaster(ScriptPlayer*pRoot):XXObject(pRoot)
{
	m_bNoCase=XTRUE;
}

XXObjectAsBroadcaster::~XXObjectAsBroadcaster()
{

}

XBOOL XXObjectAsBroadcaster::SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	switch(id)
	{
	case _SYSID(initialize):
		var=this;
		var.ToFunction(id);
		return XTRUE;
	}
	return XXObject::SysGetMember(id,pCnt,var);
}

XBOOL XXObjectAsBroadcaster::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{
	int nSize=list.GetSize();
	switch(id)
	{
	case _SYSID(initialize):
			 //break;
			 if(nSize&&list[0].IsObject())
			 {
				 XXObject*pObj=list[0].pObject;
				 pObj->_SYSCALL(addListener);//AddMember(_strAddListener,XXFUNVAR(addListener,pObj));
				 pObj->_SYSCALL(broadcastMessage);//AddMember(_strBroadcastMessage,XXFUNVAR(broadcastMessage,pObj));
				 pObj->_SYSCALL(listeners);
				 //AddMember(_strInitialize,XXFUNVAR(initialize));
				 pObj->_SYSCALL(removeListener);//AddMember(_strRemoveListener,XXFUNVAR(removeListener,pObj));
			 }
			 return XTRUE;
	}
	return XXObject::SysCallMethod(id,pCnt,list);
}

void XXObjectAsBroadcaster::InitObject()
{
	_SYSCALL(initialize);
	//AddProperty(XXVar(_strListeners),XXFUNOBJ(listeners),XNULL);
	//AddMember(XXVar(_strAddListener),XXFUNVAR(addListener));
	//AddMember(XXVar(_strBroadcastMessage),XXFUNVAR(broadcastMessage));
	//AddMember(XXVar(_strInitialize),XXFUNVAR(initialize,this));
	//AddMember(XXVar(_strRemoveListener),XXFUNVAR(removeListener));
}



XXObject* XXObjectAsBroadcaster::CreateObject(XSWFCONTEXT *pCnt, XXVARLIST&list,XBOOL)
{
   XXObjectAsBroadcaster*pObj=new XXObjectAsBroadcaster(m_pRoot);
   if(pObj)
   {
	   pObj->SetSuper(this,XFALSE);//(m_bBase&1)?this:m_pSuper,XFALSE);
	   pObj->m_pSuperObj=GetSuperObject();
   }
   //for(XU32 i=0;i<pList->GetSize();i++)
   //   pObj->m_list.Add(new XXVar(*(*pList)[i]));
   pCnt->pStack->Push((pObj));
   return pObj;
}
