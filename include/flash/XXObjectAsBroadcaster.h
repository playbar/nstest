// XXObjectAsBroadcaster.h: interface for the XXObjectAsBroadcaster class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTASBROADCASTER_H__F76B976F_E90A_473C_9794_208BBAAC16E6__INCLUDED_)
#define AFX_XXOBJECTASBROADCASTER_H__F76B976F_E90A_473C_9794_208BBAAC16E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"


class XXObjectAsBroadcaster : public XXObject  
{
	XDEC_XOBJECT(XXObjectAsBroadcaster)
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	XXObjectAsBroadcaster(ScriptPlayer*pRoot);
	virtual ~XXObjectAsBroadcaster();
	virtual XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*)
	{
		return (XU32)CreateObject(pCnt,list,XFALSE);
	}
	virtual XU32 GetObjectType()
	{
		return XXOBJ_CASTER;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
protected:
	void InitObject();
};

#endif // !defined(AFX_XXOBJECTASBROADCASTER_H__F76B976F_E90A_473C_9794_208BBAAC16E6__INCLUDED_)
