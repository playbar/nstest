// ObjectArgs.h: interface for the ObjectArgs class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTARGS_H__F68061BD_1B81_4E4F_9F27_14A33C395D0F__INCLUDED_)
#define AFX_OBJECTARGS_H__F68061BD_1B81_4E4F_9F27_14A33C395D0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObjectArray.h"

class XXObjectArgs:public XXObjectArray
{
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	//void calleeGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void callerGet(XSWFCONTEXT&cnt,XXVARLIST&list);
//	void argLenGet(XSWFCONTEXT&cnt,XXVARLIST&list);
//	void argLenSet(XSWFCONTEXT&cnt,XXVARLIST&list){}
/*	struct ARGDATA
	{
		XU8		   nRegCode;
		XString8  strParam;
		XXVar	   var;
	};*/
	XXObjectArgs(ScriptPlayer*pRoot,XXObject*pSuper,XU8 bInit);
	//XU32 GetSize(){return m_args.GetSize();}
	/*ARGDATA* NewArg();
	ARGDATA* operator [](int id)
	{ return m_args[id];}*/
	//operator XVector<XARGDATA*>&(){return &m_args}
	//XVector<ARGDATA*> m_args;
	//static XXObjectArgs m_baseArgs;
	XXObjectArgs(ScriptPlayer*pRoot,XXObject*pCallee):XXObjectArray(pRoot)
	{
//		m_pCallee=XNULL;
	//	m_pCaller=XNULL;
		m_pCallee=pCallee;
	}
	~XXObjectArgs()
	{
		//Release(0);
	}
	/*void Ref(XXObjectArgs&args)
	{
		for(XU32 i=0;i<args.m_args.GetSize();i++)
		{
			ARGDATA*p=new ARGDATA;
			p->nRegCode=args[i]->nRegCode;
			p->var=args[i]->var;
			p->strParam=args[i]->strParam;
			m_args.Add(p);
		}
		//m_args.Append(args.m_args);
	}*/
	/*void Release(XU8 bClone)
	{
		//for(XU32 i=0;i<m_args.GetSize();i++)
		//	FREEOBJ(m_args[i]->pVar);
		//if(bClone)
		//	m_args.RemoveAll();
		//else
		m_args.FreeAll();
	}*/
protected:
	//void InitObject();
	virtual XU32 GetObjectType()
	{
		return XXOBJ_ARGUMENT;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}


public:
	XXObject*m_pCallee;
//	XXObject*m_pCaller;
//	XXObject*m_pCallee;
};

#endif // !defined(AFX_OBJECTARGS_H__F68061BD_1B81_4E4F_9F27_14A33C395D0F__INCLUDED_)
