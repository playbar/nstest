// ObjectFunction.h: interface for the ObjectFunction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTFUNCTION_H__E6EEE1C3_6C54_4891_A822_5F47D3FF176D__INCLUDED_)
#define AFX_OBJECTFUNCTION_H__E6EEE1C3_6C54_4891_A822_5F47D3FF176D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"
#include "ObjectArgs.h"
class XXObjectFunction:public XXObject
{
protected:
/*	struct ARGDATA
	{
		XU8		   nRegCode;
		XString8   strParam;
		XXVar	   var;
	};
	XVector<ARGDATA*>*/
	XDEC_XOBJECT(XXObjectFunction);
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	/*virtual void Destroy()
	{
		XXObject::Destroy();
		ReleaseFunction();
	}*/
	virtual void Destruct(){ReleaseFunction();FREEOBJECT(m_pSuper);}

	virtual ScriptThread* GetTarget()
	{
		if(m_pBase) return m_pBase->GetTarget();
		else return XXObject::GetTarget();
	}
	XXObjectFunction*m_pNext;
	void SetConstant(XPCTSTR strBuf,XU16 c)
	{
		m_pConsts=strBuf;
		m_nConsts=c;
	}
	//void call(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void apply(XSWFCONTEXT&cnt,XXVARLIST&list);
public:
	void ReleaseFunction();
	/*void Destroy()
	{
		XXObject::Destroy();
		FREEOBJECT(m_pContaint);
	}*/
	XXObjectFunction*Clone();
	/*XXObjectFunction
	void SetCallBack(_HANDBACK pCall)
	{
		m_nFlags|=XCALLBACK_FLAG;
		m_pActions=(XActionScript*)pCall;
	}*/
	void SetInitFlag()
	{
		m_nFlags|=XINITCLASS;
	}
//	XXObject* GetLocalObj(){return &m_localObj;}
	XU32 Init2(XXObject*pContaint,SParser&stream,XXObject*pLocal);//XStream*pStream);
	XU32 Init(XXObject*pContaint,SParser&stream,XXObject*pLocal);//XStream*pStream);
//	XXObject* GetBlock(){return m_pBlock;}
	XXObject* GetBase(){return m_pBase;}
//	void SetLocal(XXObject*p){m_pLocal=p->RefObject();}
	//XXObject* GetContaint(){return m_pContaint;}
	XXObjectFunction(ScriptPlayer*pRoot);
	~XXObjectFunction();
protected:
	int GetSubCount(XXObject*pObj);
//	XXObject m_localObj;
	//XXObject* m_pContaint;
	void InitVariable(XSWFCONTEXT&cnt,XXObject*pCaller,XXObject*pLocalObj,XXObjectArgs*pArgs,XXVARLIST&args);
	void InitObject();
	virtual XU32 Construct(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*pCaller)
	{
		return Function(pCnt,list,pCaller);
	}
	virtual XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*pCaller);
	virtual XU32 GetObjectType()
	{
		return XXOBJ_FUNCTION;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
	//XActionScript*m_pActions;	
	XU8* m_pCodeBuf,*m_pArgs;
	int  m_nCodeLength;
	enum {XPRELOAD_PARENT=0x80,
		  XPRELOAD_ROOT=0x40,
		  XCREATE_SUPER=0x20,
		  XPRELOAD_SUPER=0x10,
		  XCREATE_ARGS=0x08,
		  XPRELOAD_ARGS=0x04,
		  XCREATE_THIS=0x02,
		  XPRELOAD_THIS=0x01,
		  XFUNCTION2=0x8000,
		  XINITCLASS=0x4000,
		  XCALLBACK_FLAG=0x000400,
		  XCLONE_FLAG  =0x000200,
		  XPRELOAD_GLOBAL=0x0100};
#pragma pack(1)
	XU16		   m_nFlags;
	XU16		   m_nConsts;
	XU8			   m_nRegCount;
	XU8			   m_nResetCount;
	XPCTSTR		   m_pConsts;
	XXVar		   m_strResetPath;
	XXObject*	   m_pBase;
	XXObject*	   m_pBlocks;
#pragma pack()

	//XXObjectArgs m_args;
};

#endif // !defined(AFX_OBJECTFUNCTION_H__E6EEE1C3_6C54_4891_A822_5F47D3FF176D__INCLUDED_)
