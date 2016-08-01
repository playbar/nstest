// ObjectString.h: interface for the ObjectString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTSTRING_H__2FF0C403_52B5_4FA2_B285_97E9A9FC7CAC__INCLUDED_)
#define AFX_OBJECTSTRING_H__2FF0C403_52B5_4FA2_B285_97E9A9FC7CAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectString:public XXObject
{
	XDEC_XOBJECT(XXObjectString)
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	virtual XBOOL SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	//void strLenGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void strLenSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void charAt(XSWFCONTEXT&cnt,XXVARLIST&list);
	void charCodeAt(XSWFCONTEXT&cnt,XXVARLIST&list);
	void concatStr(XSWFCONTEXT&cnt,XXVARLIST&list);
	void fromCharCode(XSWFCONTEXT&cnt,XXVARLIST&list);
	void indexOf(XSWFCONTEXT&cnt,XXVARLIST&list);
	void lastIndexOf(XSWFCONTEXT&cnt,XXVARLIST&list);
	void sliceStr(XSWFCONTEXT&cnt,XXVARLIST&list);
	void splitStr(XSWFCONTEXT&cnt,XXVARLIST&list);
	void substr(XSWFCONTEXT&cnt,XXVARLIST&list);
	void substring(XSWFCONTEXT&cnt,XXVARLIST&list);
	void toLowerCase(XSWFCONTEXT&cnt,XXVARLIST&list);
	void toUpperCase(XSWFCONTEXT&cnt,XXVARLIST&list);
public:
	double GetNumber(){return m_strValue.ToFloat();}
	XXObjectString* Clone(XU8 bData)
	{
		XXObjectString*pClone=new XXObjectString(m_pRoot);
		if(pClone)
		{
		   pClone->SetSuper((m_bBase&1)?this:m_pSuper,XFALSE);
//		   pClone->SetParent(m_pParent);
		   if(bData)
		     pClone->m_strValue=m_strValue;
		}
		return pClone;
	}


	int Length(){return m_strValue.GetLength();}
	virtual XBOOL GetMember(XU32 id,XXVar&var,XXVar*name)
	{
		 if((int)id<m_strValue.GetLength())
		 {
			 //XWCHAR c=m_strValue.GetData()[id];
			 var.SetString(XNULL,0);
			 var.AddChar(m_strValue.strTxt[id]);
			 //*pVar=m_strValue.GetData()[id];
			 return XTRUE;
		 }
		 return XFALSE;
	}
	virtual XBOOL SetMember(XU32 id,XXVar&var)
	{
	     m_strValue.ToString(XTRUE);
		 if((int)id<m_strValue.GetLength())
		 {
			 m_strValue.strTxt[id]=(XTCHAR)var.ToInt();
			 return XTRUE;
		 }
		 return XFALSE;
	}
	virtual XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*)
	{
		if(list.GetSize())
		{
			list[0].ToString(XFALSE);
			pCnt->pStack->Push(list[0]);
		}
		else
			pCnt->pStack->Push(_strEmpty);
		return (XU32)this;
	}
	virtual XU32 Construct(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*)
	{
		if(list.GetSize())
		{
			list[0].ToString(XFALSE);
			m_strValue=list[0];
		}
		else
			m_strValue=_strEmpty;
		pCnt->pStack->Push(this);
		return (XU32)this;
	}
	virtual XU32 GetObjectType()
	{
		return XXOBJ_STRING;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}	
	virtual void GetString(XXVar&var)
	{
		var=m_strValue;
	}
	XXObjectString(ScriptPlayer*pRoot):XXObject(pRoot)
	{
	    //m_nObjType=XOOT_TYPE;
		m_nDataType=XODT_STRING;
		m_strValue.SetString(XNULL,0);
		//m_strValue="";
	}
	/*XXObjectString(XWCHAR c)
	{
		//m_nObjType=XOOT_TYPE;
		m_nDataType=XODT_STRING;
		m_strValue=c;
	}*/
	/*XXObjectString(ScriptPlayer*pRoot,XString8&str,XU8 bAttach):XXObject(pRoot)
	{
		//m_nObjType=XOOT_TYPE;
		m_nDataType=XODT_STRING;
		if(!bAttach)
		{
			m_strValue.SetString(str,str.GetLength());
			//m_strValue=str;
		}
		else
		{

			//int s=str.GetSize();
			m_strValue.AttachString(str.DataTo());
			//m_strValue.DataFrom(str.DataTo(),s);
		}
	}*/
	XXObjectString(ScriptPlayer*pRoot,XXVar&var):XXObject(pRoot)
	{
		//m_nObjType=XOOT_TYPE;
		m_nDataType=XODT_STRING;
		m_strValue=var;
	}
//	void SetString(XPCTSTR str){m_strValue=str;}
protected:
	void InitObject();
	XXObject* GetLength();
	XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list);
public:
	XXVar m_strValue;
};

#endif // !defined(AFX_OBJECTSTRING_H__2FF0C403_52B5_4FA2_B285_97E9A9FC7CAC__INCLUDED_)
