// XXObjectLoadVars.h: interface for the XXObjectLoadVars class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTLOADVARS_H__8A0F0806_4C31_4E6A_A8B4_F63E3ED58EC0__INCLUDED_)
#define AFX_XXOBJECTLOADVARS_H__8A0F0806_4C31_4E6A_A8B4_F63E3ED58EC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectLoadVars : public XXObject//Event 
{
	XDEC_XOBJECT(XXObjectLoadVars)
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	virtual XBOOL SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*)
	{
		return (XU32)CreateObject(pCnt,list,XFALSE);
	}
	virtual void Destruct()
	{
		FREEOBJECT(m_pHeaders);
		FREEOBJECT(m_pSuper);
	}
	/*virtual void Release()
	{
		XXObject::Release();
		FREEOBJECT(m_pHeaders);
	}
	virtual void Destroy()
	{
		XXObject::Destroy();
		FREEOBJECT(m_pHeaders);
	}*/
	
	virtual XU32 GetObjectType()
	{
		return XXOBJ_LOADVARS;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
	XXObjectLoadVars(ScriptPlayer*pRoot);
	virtual ~XXObjectLoadVars();
	void SetTotal(int v){m_totalBytes=v;}
	void SetLoaded(int v){m_loadedBytes=v;}
	XXVar m_target;
protected:
	int m_totalBytes,m_loadedBytes;
	XXObject* m_pHeaders;
	XBOOL m_bLoaded;
	void InitObject();
};

#endif // !defined(AFX_XXOBJECTLOADVARS_H__8A0F0806_4C31_4E6A_A8B4_F63E3ED58EC0__INCLUDED_)
