// ObjectMath.h: interface for the ObjectMath class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTMATH_H__4B46FAAC_AA2A_4EE1_A812_27C9207F4AB8__INCLUDED_)
#define AFX_OBJECTMATH_H__4B46FAAC_AA2A_4EE1_A812_27C9207F4AB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectMath:public XXObject
{
	XDEC_XOBJECT(XXObjectMath);
protected:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	void InitObject();
	virtual XU32 GetObjectType()
	{
		return XXOBJ_MATH;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
	XXObjectMath(ScriptPlayer*pRoot):XXObject(pRoot)
	{
		m_bNoCase=XTRUE;
	}
};

#endif // !defined(AFX_OBJECTMATH_H__4B46FAAC_AA2A_4EE1_A812_27C9207F4AB8__INCLUDED_)
