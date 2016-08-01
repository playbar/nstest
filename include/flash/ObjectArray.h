// ObjectArray.h: interface for the ObjectArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTARRAY_H__E1BD2AC4_7E5B_43B1_AC3F_7863691BF402__INCLUDED_)
#define AFX_OBJECTARRAY_H__E1BD2AC4_7E5B_43B1_AC3F_7863691BF402__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectArray:public XXObject
{
	XDEC_XOBJECT(XXObjectArray);
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	virtual XBOOL SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	class XSortList:public XSort
	{		  
	public:
	enum{CASEINSENSITIVE=0x01,
		 DESCENDING=0x02,
		 UNIQUESORT=0x04,
		 RETURNINDEXEDARRAY=0x08,
		 NUMERIC=0x10};
	public:
		XBOOL GetKey(XXVar&key,int index)
		{
		   if(m_pKey->IsObject(XXOBJ_ARRAY))
		   	   return m_pKey->pObject->GetMember(index,key,XNULL);
		   else if(index==0) 
		   {
			   key=*m_pKey;
			   return XTRUE;
		   }
		   return XFALSE;
		}
		XSortList(XSWFCONTEXT*pContext,XU8 f,XXVar&method,int n,XXVar*pKey):list(n)//,trans(n)
		{
			m_pContext=pContext;
			m_nFlag=f; 
			m_method=method;
			m_pKey=pKey;
			//m_pMethod=pMethod;
		}
		~XSortList()
		{
			//trans.Release();
		}
		XU32 GetCount()
		{
			return list.GetSize();
			//return trans.GetSize();
		}
		XBOOL Add(XSWFCONTEXT&cnt,XXVar&var)//XPCTSTR strKey=XNULL)
		{
			/*XXVar pv;//*pv;
			if(key==XNULL)
				pv=var;//new XXVar(*pVar);
			else
			{
				//pv=new XXVar;
				if(var.IsObject())
					var.pObject->GetMember(cnt,strKey,pv,XTRUE);
				else
					pv=var;
					//pVar->pObject->GetMember(cnt,strKey,*pv,XTRUE);
			}*/
			int mi;
			int id=Index(&var,mi);
			/*if(m_nFlag&NUMERIC) 
			{
				pv->ToFloat();
				id=IndexF(pv->fData,mi);
			}
			else
			{
				pv->ToString();
				id=IndexW(pv->strData,mi,(m_nFlag&CASEINSENSITIVE)?XSS_NOCASE:XSS_NORMAL);
			}*/
			list.InsertAt(mi,var);
			//trans.InsertAt(mi,pv);
		    return id>=0;
		}
		void* GetData(XU32 i){return &list[i];}
		//XPCWSTR GetWData(XU32 i){return trans[i]->strData;}
	public:
		XINT Compare(void*iSrt,void*iDst,XU8 nMode);
		XSWFCONTEXT*m_pContext;
		XXVar m_method;
		XXVar*m_pKey;
		XU8 m_nFlag;
		XXStack list;
		//XXStack trans;
	protected:
		XINT CompareData(XXVar&var1,XXVar&var2);
	};
public:
	void arrayLenSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void arrayLenGet(XSWFCONTEXT&cnt,XXVARLIST&list);
public:
	void Concat(XSWFCONTEXT&cnt,XXVARLIST&list);
	void Pop(XSWFCONTEXT&cnt,XXVARLIST&list);
	void Join(XSWFCONTEXT&cnt,XXVARLIST&list);
	void Push(XSWFCONTEXT&cnt,XXVARLIST&list);
	void Reverse(XSWFCONTEXT&cnt,XXVARLIST&list);
	void Shift(XSWFCONTEXT&cnt,XXVARLIST&list);
	void Slice(XSWFCONTEXT&cnt,XXVARLIST&list);
	void Sort(XSWFCONTEXT&cnt,XXVARLIST&list);
	void SortOn(XSWFCONTEXT&cnt,XXVARLIST&list);
	void Splice(XSWFCONTEXT&cnt,XXVARLIST&list);
	void UnShift(XSWFCONTEXT&cnt,XXVARLIST&list);

	XXObjectArray* Clone(XU8 bData)
	{
		XXObjectArray*pClone=new XXObjectArray(m_pRoot);
		if(pClone)
		{
		   pClone->SetSuper((m_bBase&1)?this:m_pSuper,XFALSE);
		   if(bData)
		   for(XU32 i=0;i<m_list.GetSize();i++)
			   pClone->m_list.Push(m_list[i]);//Add(new XXVar(*m_list[i]));
		}
		return pClone;
	}
	virtual void Release()
	{
		XXObject::Release();
		m_list.Release();
	}
	virtual void Destroy()
	{
		XXObject::Destroy();
		m_list.Release();
	}

protected:
	void GetLength(XXVar&var)
	{
		var=(int)m_list.GetSize();
	}
	void InitObject( );
	virtual XBOOL GetMember(XU32 id,XXVar&var,XXVar*name)
	{
	//	 if(id==5)
	//		 int v=0;
		 if(id<m_list.GetSize())
		 {
			 var=m_list[id];			 
			 return XTRUE;
		 }
		 return XFALSE;
	}
	virtual XBOOL SetMember(XU32 id,XXVar&var)
	{
		 if(id>=XMAXARRAY) return XFALSE;
		 if(id>=m_list.GetSize())
		 {
			 int s=m_list.GetSize();
			 m_list.SetSize(id+1);
			 int sn=m_list.GetSize();
			 for(int i=s;i<sn;i++)
				 m_list[i].ToConst(XOCT_UNDEFINE);//=XXVar::CreateConst(XOCT_UNDEFINE);
		 }
		 if(id<m_list.GetSize())
		 {
			 m_list[id]=var;
			
		 }
		 return XTRUE;
	}
	XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*pCaller)
	{
		return (XU32)CreateObject(pCnt,list);
	}
	XU32 Construct(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*)
	{
		int nSize=list.GetSize();
		if(nSize==1&&list[0].IsNum3())
		{
			nSize=list[0].ToInt();
			m_list.SetSize(nSize);
			for(int i=0;i<nSize;i++)
				m_list[i].ToConst(XOCT_UNDEFINE);
		}
		else if(nSize)
		{
			m_list.SetSize(nSize);
			for(int i=0;i<nSize;i++)
				m_list[i]=(list[i]);
		}
		pCnt->pStack->Push(this);
		return (XU32)this;
		//return (XU32)CreateObject(pCnt,list,XFALSE);
	}
	virtual XU32 IsArray(){return XTRUE;}
	virtual XU32 GetObjectType()
	{
		return XXOBJ_ARRAY;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
	//virtual void GetString(XXVar&var);//XString8&str)	
	virtual void GetString(XXVar&str)//XString8&str)
	{
		str.SetString("",0);
		//XXVar strSp(_strSplitQ);
		for(XU32 i=0;i<m_list.GetSize();i++)
		{
			XXVar var=m_list[i];
			//if(i==22&&m_list[i].IsObject())
			//	int v=0;
			var.ToString(XFALSE);
			if(!str.IsNull())//IsEmpty())
				str.StringAdd(_strSplitQ);
				//str+=;
			str.StringAdd(var);
			//str+=var.strTxt;
		}
	}
	XXObjectArray(ScriptPlayer*pRoot):XXObject(pRoot),m_list(32)
	{
#ifdef _DEBUG
	nObjCount[XXOBJ_ARRAY]++;
#endif
		m_bNoCase=XTRUE;
	}
	~XXObjectArray()
	{
#ifdef _DEBUG
	nObjCount[XXOBJ_ARRAY]--;
#endif
		m_list.Release();
	}
public:
	void GetData(XU32Array&datas);
	void GetData(XIntArray&datas);
	void GetData(XU8Array&datas);
	XXStack m_list;
};

#endif // !defined(AFX_OBJECTARRAY_H__E1BD2AC4_7E5B_43B1_AC3F_7863691BF402__INCLUDED_)
