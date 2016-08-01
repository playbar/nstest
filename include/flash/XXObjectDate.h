// XXObjectDate.h: interface for the XXObjectDate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTDATE_H__5447CAEF_6ED0_48D1_9017_142C59D1233C__INCLUDED_)
#define AFX_XXOBJECTDATE_H__5447CAEF_6ED0_48D1_9017_142C59D1233C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"


class XXObjectDate : public XXObject  
{
	XDEC_XOBJECT(XXObjectDate)
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	virtual XU32 Construct(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*);
	
	
	XXObjectDate(ScriptPlayer*pRoot);
	virtual ~XXObjectDate();
	virtual XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*pCaller)
	{
		return (XU32)CreateObject(pCnt,list,XFALSE);
	}
	virtual void ValueOf(XSWFCONTEXT*pCnt)
	{
		pCnt->pStack->Push(m_time.GetTime());
	}
	virtual XU32 GetObjectType()
	{
		return XXOBJ_DATE;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
protected:
	virtual void GetString(XXVar&var);//XString8&str);
	void InitObject( );
	XTime m_time;
	XU16  m_nTickCount;
};

#endif // !defined(AFX_XXOBJECTDATE_H__5447CAEF_6ED0_48D1_9017_142C59D1233C__INCLUDED_)
