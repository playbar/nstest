// XXObjectSystem.h: interface for the XXObjectSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTSYSTEM_H__7BA6794C_D64E_4135_87AF_A65CDA854EFF__INCLUDED_)
#define AFX_XXOBJECTSYSTEM_H__7BA6794C_D64E_4135_87AF_A65CDA854EFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

const XPCTSTR _strSandboxTypeStr[]=
{
	_XXKEY("remote"),
	_XXKEY("localWithFile"),
	_XXKEY("localWithNetwork"),
	_XXKEY("localTrusted")
};
enum {XSDT_REMOTE=0,XSDT_LOCALWITHFILE=1,
	  XSDT_LOCALWITHNETWORK=2,XSDT_LOCALTRUSTED};


class XXObjectSecurity:public XXObject
{
	XDEC_XOBJECT(XXObjectSecurity);
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	XXObjectSecurity(ScriptPlayer*pRoot):XXObject(pRoot)
	{
	}
	//void allowDomain(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void allowInsecureDomain(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void loadPolicyFile(XSWFCONTEXT&cnt,XXVARLIST&list);
	virtual XU32 GetObjectType()
	{
		return XXOBJ_SECURITY;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
protected:
	void InitObject();
};
class XXObjectCapabilities:public XXObject
{
	XDEC_XOBJECT(XXObjectCapabilities);
public:
	virtual XU32 GetObjectType()
	{
		return XXOBJ_CAP;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
	XXObjectCapabilities(ScriptPlayer*pRoot):XXObject(pRoot)
	{
	}
protected:
	void InitObject();
};
class XXObjectIME:public XXObject
{
	XDEC_XOBJECT(XXObjectIME);
public:
	XXObjectIME(ScriptPlayer*pRoot):XXObject(pRoot)
	{
	}
	virtual XU32 GetObjectType()
	{
		return XXOBJ_IME;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
protected:
	void InitObject();
};

class XXObjectSystem : public XXObject  
{
	XDEC_XOBJECT(XXObjectSystem);
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	//securtyGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//imeGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//capGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void setClipboard(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void showSettings(XSWFCONTEXT&cnt,XXVARLIST&list);

	//IMEGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	virtual XU32 GetObjectType()
	{
		return XXOBJ_SYSTEM;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
	XXObjectSystem(ScriptPlayer*pRoot);
	virtual ~XXObjectSystem();

protected:
	void InitObject();
};

#endif // !defined(AFX_XXOBJECTSYSTEM_H__7BA6794C_D64E_4135_87AF_A65CDA854EFF__INCLUDED_)
