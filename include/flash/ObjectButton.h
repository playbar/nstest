// ObjectButton.h: interface for the ObjectButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTBUTTON_H__32120EEE_4197_4F2A_AB8E_7123CD96FC26__INCLUDED_)
#define AFX_OBJECTBUTTON_H__32120EEE_4197_4F2A_AB8E_7123CD96FC26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"



class XXObjectButton:public XXObject//Event
{
	XDEC_XOBJECT(XXObjectButton);
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	virtual XBOOL SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	
	void SetObject(SObject*p){m_pObject=p;}
public:
	virtual XXObject* GetParent();
	/*static XXObjectButton* CreateClass(XXObject*pSuper,XDTDefineButtonDisp*pBut,XU8 bInit)
	{
		XXObjectButton*pObj=new XXObjectButton(pBut);
		if(pObj)
		{
			if(bInit)
				pObj->InitObject();
			pObj->m_bBase=bInit;
			pObj->SetSuper(pSuper,bInit);
		}
		return pObj;
	}*/
	XXObjectButton(ScriptPlayer*pRoot):XXObject(pRoot)
	{
		m_pObject=XNULL;
#ifdef _DEBUG
	nObjCount[XXOBJ_BUTTON]++;
#endif
		m_bNoCase=XTRUE;
	}
	~XXObjectButton()
	{
#ifdef _DEBUG
	nObjCount[XXOBJ_BUTTON]--;
#endif
	}
protected:
	virtual XBOOL GetClipOf(XPCTSTR strName,XXVar&var);//*pVar);
	void InitObject();
	virtual XU32 GetObjectType()
	{
		return XXOBJ_BUTTON;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
//	SObject* m_pObject;
	//XDTDefineButtonDisp*m_pButton;
protected:

//	XOBJLIST m_events;
};

#endif // !defined(AFX_OBJECTBUTTON_H__32120EEE_4197_4F2A_AB8E_7123CD96FC26__INCLUDED_)
