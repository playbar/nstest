// XActionScript.h: interface for the XActionScript class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XACTIONSCRIPT_H__BE9C6EEA_5DD3_4E4C_8DCD_02A132851675__INCLUDED_)
#define AFX_XACTIONSCRIPT_H__BE9C6EEA_5DD3_4E4C_8DCD_02A132851675__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "splayer.h"
#include "XXObject.h"
#include "ObjectClip.h"
extern S32 Random(S32 range);

#define MAX_CACHES 256

class XActionScript  
{
#ifdef _VARCACHE_
public:
	//int		  m_nCacheCount;
    _VARCACHE m_caches[MAX_CACHES];
	inline XBOOL Cache(XXVar&var,_VARCACHE&c)
	{
		if(var.nStringType!=STRING_CONST||var.nRefID>=MAX_CACHES) return XFALSE;
		if(var.IsUndefine()) return XFALSE;
		//	return XTRUE;
		//if(c.pObject==XNULL) return XFALSE;
		m_caches[var.nRefID]=c;
		//var.nRefID=m_nCacheCount;
		//if(var.nConstID!=XXVar::XINVALID_REF)
		//	cs[var.nConstID]=var.nRefID;
		//m_nCacheCount++;
		return XTRUE;
	}
	inline XBOOL CacheRestore(XXVar&name,_VARCACHE&c)//,XXVar&var)
	{
		if(name.nStringType!=STRING_CONST||name.nRefID>=MAX_CACHES) 
			return XFALSE;
		if(m_caches[name.nRefID].pObject==XNULL) return XFALSE;
		c=m_caches[name.nRefID];
		
		return XTRUE;
	}
	inline XBOOL CacheGetVariable(XXVar&name,XSWFCONTEXT&cnt,XXVar&var);
#endif
public:

	XActionScript*m_pNext;
	enum {XV_STRING=0,XV_FLOAT=1,XV_NULL=2,XV_UNDEFINE=3,
		  XV_REGISTER=4,XV_BOOLEAN=5,XV_DOUBLE=6,XV_DWORD=7,XV_C8=8,XV_C16=9};
	XBOOL GetLocal(XPCTSTR strName,XXVar&var,XBOOL bLowCase);
	XBOOL SetLocal(XPCTSTR strName,XXVar&var,XBOOL bLowCase,XBOOL bAdd);
	XActionScript* Clone();
	XBOOL DoAction(XSWFCONTEXT&cnt,XXObject*pLocalObj,XBOOL bBlock,XBOOL bInit=XFALSE);
	XActionScript(XU8* pCode,int nSize=-1)
	{
//		m_pCaller=pAction;
#ifdef _VARCACHE_
		//m_nCacheCount=0;
		for(int i=0;i<MAX_CACHES;i++)
			m_caches[i].pObject=XNULL;
#endif
		m_nCodeLength=nSize;
		m_pCodeBuf=pCode;
//		m_pThread=XNULL;
		m_bInit=XFALSE;
//		m_bBlock=XFALSE;
		m_bTell=XFALSE;
		m_bBlock=XFALSE;
		m_pNext=XNULL;
		m_baseThread=XNULL;
	}
	virtual ~XActionScript();
	void SetCodeBuf(XU8*pBuf,XU32 nLength)
	{
		m_nCodeLength=nLength;
		m_pCodeBuf=pBuf;
	}
public:
	SObject* GetWithObject()
	{
		if(m_pWith) 
			return m_pWith->m_pObject;
			//return m_pWith->m_pObject;
			//return m_pWith->GetTarget()->rootObject;
		return XNULL;
	}
	XXObject* GetCaller()
	{
		if(m_pCaller) return m_pCaller->m_thread;
			//return m_pCaller->m_thread->rootObject->pObject;
		else return m_thread;
			
			//return m_thread->rootObject->pObject;
	}
	/*XXObject* GetOpt()
	{
		return m_bTell?m_thread:(m_pThis?m_pThis:m_thread);
	}
	SObject* GetOptObject(XBOOL bMust)
	{
		XXObject*pObj=GetOpt();
		if(!bMust||pObj) return pObj->m_pObject;
		else 
		{
			if(m_thread->m_pObject) return m_thread->m_pObject;
			else if(m_baseThread->m_pObject)
				return m_baseThread->m_pObject;
			else
				return m_baseThread->m_pRoot->rootObject;
		}
	}*/
	SObject* GetThisObject(XBOOL bMust)
	{
		XXObject*pObj=GetThis();
		if(!bMust||pObj) return pObj->m_pObject;
		else 
		{
			if(m_thread->m_pObject) return m_thread->m_pObject;
			else if(m_baseThread->m_pObject)
				return m_baseThread->m_pObject;
			else
				return m_baseThread->m_pRoot->rootObject;
		}
		//if(pObj==XNULL) pObj=m_thread->rootObject;
		//return pObj;
	}
	XXObject* GetThis()
	{
		return m_bTell?m_thread:(m_pThis?m_pThis:m_thread);
		//XXObject*pObj=(m_pThis&&m_thread==m_baseThread)?m_pThis:m_thread;
		//if(pObj==XNULL) pObj=m_thread;//->rootObject->pObject;
		//return pObj;
		//return m_pThis?m_pThis:m_thread->rootObject->pObject;
	}
protected:
	XBOOL ObjectOf(XXVar&var);//,SObject*pObject);//ScriptThread*pThread);
	//XXObject* ObjectOf(XXVar*pVar);//,SObject*pObject);//ScriptThread*pThread);
	void CheckStack(int s);
	void DoThrow();
	int  DoTry(SParser&parser);
	void ImplementsOp();
	int DefineFunction2(SParser&parser);
	int DoWith(SParser&parser);
	void TargetPath();
	void SetMember();
	void NewMethod();
	void NewObject();
	void InitObject();
	void InitArray();
	void GetMember();
	void Enumerate();
	void DefineLocal(XBOOL bCnt);
	int DefineFunction(SParser&parser);
	void CallMethod();
	void CallFunction();
	void Push(SParser&parser);//XStream&stream);
	XU8* m_pCodeBuf;
//	XXTOKENS	m_locals;
	XU32		m_nCodeLength;
//	XBOOL		m_bBlock;
public:
	XBOOL ToObject(XXVar&var);
	void InitContext(XXObject*base,XXObject*thread,
					  XXObject*pThis,XXObject*pObject,
					  XXObject*pWith,XActionScript*pCaller)
	{
		//m_pPlayer=pPlayer;
		m_pCaller=pCaller;
		m_baseThread=REFOBJECT(base);//->RefObject();
		//m_root=base->player;
		m_pWith=REFOBJECT(pWith);//->RefObject();
		m_pThis=REFOBJECT(pThis);//->RefObject();//!=XNULL?pThis:base;
		m_pObject=REFOBJECT(pObject);
		m_pContext=XNULL;
		m_pStack=XNULL;
		m_thread=REFOBJECT(thread);
	}
protected:
	XBOOL NeedLowCase();
	XActionScript*m_pCaller;
	XSWFCONTEXT*  m_pContext;
	XSWFPlayer*   m_pPlayer;
	XXStack*	  m_pStack;
	//ScriptPlayer* m_root;
	XXObject*	  m_pLocalObj;
	XXObject*	  m_thread,
				 *m_baseThread;
	XBOOL		  m_bTell,m_bBlock,m_bInit;
	XXObject*	  m_pThis,*m_pObject;
	XXObject*	  m_pWith;
//	XXObject*	  m_pBlock;
//	SObject *	 m_pObject;
};

#endif // !defined(AFX_XACTIONSCRIPT_H__BE9C6EEA_5DD3_4E4C_8DCD_02A132851675__INCLUDED_)
