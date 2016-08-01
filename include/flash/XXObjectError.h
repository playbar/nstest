// XXObjectError.h: interface for the XXObjectError class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTERROR_H__0D0A9AB0_F5D3_4206_A936_7E783859EF76__INCLUDED_)
#define AFX_XXOBJECTERROR_H__0D0A9AB0_F5D3_4206_A936_7E783859EF76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectError : public XXObject  
{
	XDEC_XOBJECT(XXObjectError)
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);	
	virtual XBOOL SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	//void nameGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void nameSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void messageGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void messageSet(XSWFCONTEXT&cnt,XXVARLIST&list);
public:
	XXObjectError(ScriptPlayer*pRoot);
	virtual ~XXObjectError();
	virtual XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*)
	{
		return (XU32)CreateObject(pCnt,list,XFALSE);
	}
	virtual XU32 Construct(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*)
	{
		if(list.GetSize())
		{
			list[0].ToString(XFALSE);
			m_strMessage=list[0];//.strData;
		}
		pCnt->pStack->Push(this);
		return (XU32)this;
	}

	virtual XU32 GetObjectType()
	{
		return XXOBJ_ERROR;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
	virtual void GetString(XXVar&var)
	{
		var=m_strMessage;
	}
protected:
	//XString8 m_strMessage,m_strName;
	XXVar m_strName,m_strMessage;
	void InitObject( );
};

#endif // !defined(AFX_XXOBJECTERROR_H__0D0A9AB0_F5D3_4206_A936_7E783859EF76__INCLUDED_)
