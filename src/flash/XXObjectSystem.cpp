// XXObjectSystem.cpp: implementation of the XXObjectSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XXObjectSystem.h"
#include "ObjectCreator.h"
#include "splayer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XXObjectSystem::XXObjectSystem(ScriptPlayer*pRoot):XXObject(pRoot)
{

}

XXObjectSystem::~XXObjectSystem()
{

}

XBOOL XXObjectSystem::SysGetMember(int id, XSWFCONTEXT *pCnt, XXVar &var)
{
	switch(id)
	{
	case _SYSID(exactSettings):
	case _SYSID(useCodepage):
		 var.ToLogic();
		 var=XTRUE;
		 return XTRUE;
	case _SYSID(setClipboard):
	case _SYSID(showSettings):
		 var=this;
		 var.ToFunction(id);
		 return XTRUE;
	}
    return XXObject::SysGetMember(id,pCnt,var);;
}
XBOOL XXObjectSystem::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{
	switch(id)
	{
	case _SYSID(setClipboard):
	case _SYSID(showSettings):
		 pCnt->pStack->PushConst(XOCT_UNDEFINE);
		 return XTRUE;
	}
	return XXObject::SysCallMethod(id,pCnt,list);
}

XXObject* XXObjectSystem::CreateObject(XSWFCONTEXT*pCnt,XXVARLIST&list,XBOOL bObj)
{
	pCnt->pStack->PushConst(XOCT_NULL);
	return XNULL;
}

void XXObjectSystem::InitObject()
{
	XXObject*pBase=m_pRoot->m_pGlobal->GetObjBase();
	XXObject*ps=XXObjectSecurity::CreateClass(pBase,XTRUE,m_pRoot);
	AddMember(_SYSVAR(security),XXVar(ps));
	ps=XXObjectIME::CreateClass(pBase,XTRUE,m_pRoot);
	AddMember(_SYSVAR(IME),XXVar(ps));
	ps=XXObjectCapabilities::CreateClass(pBase,XTRUE,m_pRoot);
	AddMember(_SYSVAR(capabilities),XXVar(ps));
	XXVar var(XTRUE);
	var.ToLogic();
	AddMember(_SYSVAR(exactSettings),var);
	AddMember(_SYSVAR(useCodepage),var);
	_SYSCALL(setClipboard);
	_SYSCALL(showSettings);
	//AddMember(XXVar(_strSetClipboard),XXVar(XXFUNOBJ(setClipboard)));
	//AddMember(XXVar(_strShowSettings),XXVar(XXFUNOBJ(showSettings)));
}								   


void XXObjectCapabilities::InitObject()
{

}

XXObject* XXObjectCapabilities::CreateObject(XSWFCONTEXT*pCnt,XXVARLIST&list,XBOOL bObj)
{
	pCnt->pStack->PushConst(XOCT_NULL);
	return XNULL;
}

XXObject* XXObjectSecurity::CreateObject(XSWFCONTEXT*pCnt,XXVARLIST&list,XBOOL bObj)
{
	pCnt->pStack->PushConst(XOCT_NULL);
	return XNULL;
}

XBOOL XXObjectSecurity::SysGetMember(int id, XSWFCONTEXT *pCnt, XXVar &var)
{
	switch(id)
	{
	case _SYSID(sandboxType):
		 var=_strSandboxTypeStr[XSDT_LOCALWITHFILE];return XTRUE;
	case _SYSID(allowDomain):
	case _SYSID(allowInsecureDomain):
	case _SYSID(loadPolicyFile):
		 var=this;
		 var.ToFunction(id);
		 return XTRUE;
	}
    return XXObject::SysGetMember(id,pCnt,var);;
}
XBOOL XXObjectSecurity::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{
	switch(id)
	{
	case _SYSID(allowDomain):
	case _SYSID(allowInsecureDomain):
	case _SYSID(loadPolicyFile):
		 pCnt->pStack->PushConst(XOCT_UNDEFINE);
		 return XTRUE;
	}
	return XXObject::SysCallMethod(id,pCnt,list);
}

void XXObjectSecurity::InitObject()
{
	_SYSCALL(sandboxType);
	_SYSCALL(allowDomain);
	_SYSCALL(allowInsecureDomain);
	_SYSCALL(loadPolicyFile);
	//AddMember(XXVar(_strSandboxType),XXVar(_strSandboxTypeStr[XSDT_LOCALWITHFILE/*XSDT_LOCALWITHNETWORK*/]));
	//AddMember(XXVar(_strAllowDomain),XXVar(XXFUNOBJS(allowDomain)));
	//AddMember(XXVar(_strLoadPolicyFile),XXVar(XXFUNOBJS(loadPolicyFile)));
	//AddMember(XXVar(_strAllowInsecureDomain),XXVar(XXFUNOBJS(allowInsecureDomain)));
}

///////////////////////////////////////////////////
void XXObjectIME::InitObject()
{

}

XXObject* XXObjectIME::CreateObject(XSWFCONTEXT*pCnt,XXVARLIST&list,XBOOL bObj)
{
	pCnt->pStack->PushConst(XOCT_NULL);
	return XNULL;
}
