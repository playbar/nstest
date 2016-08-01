// XXObjectError.cpp: implementation of the XXObjectError class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XXObjectError.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XXObjectError::XXObjectError(ScriptPlayer*pRoot):XXObject(pRoot)
{
	m_bNoCase=XTRUE;
	m_strName=_strEmpty;
	m_strMessage=_strEmpty;
}

XXObjectError::~XXObjectError()
{

}

XBOOL XXObjectError::SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	switch(id)
	{
	case _SYSID(name):
		m_strName=var;
		return XTRUE;
	case _SYSID(message):
		m_strMessage=var;
		return XTRUE;
	}
	return XXObject::SysGetMember(id,pCnt,var);
}
XBOOL XXObjectError::SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	switch(id)
	{
	case _SYSID(name):
		var=m_strName;
		return XTRUE;
	case _SYSID(message):
		var=m_strMessage;
		return XTRUE;
	}
	return XXObject::SysSetMember(id,pCnt,var);
}


void XXObjectError::InitObject( )
{
	_SYSCALL(name);
	_SYSCALL(message);
	//AddProperty(XXVar(_strName),XXFUNOBJ(nameGet),XXFUNOBJ(nameSet));
	//AddProperty(XXVar(_strMessage),XXFUNOBJ(nameGet),XXFUNOBJ(nameSet));
}




XXObject* XXObjectError::CreateObject(XSWFCONTEXT *pCnt, XXVARLIST &list,XBOOL)
{
	XXObjectError*pObj=CreateClass(this,XFALSE,m_pRoot);
	if(pObj)
	{
		pObj->m_pSuperObj=GetSuperObject();
		if(list.GetSize())
		{
			list[0].ToString(XFALSE);
			m_strMessage=list[0];//.strData;
		}
	}
	pCnt->pStack->Push(pObj);//new XXVar(pObj));
	return pObj;
}
