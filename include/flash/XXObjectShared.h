// XXObjectShared.h: interface for the XXObjectShared class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTSHARED_H__A858D16A_BF76_44AB_85C2_0C05F985DC0D__INCLUDED_)
#define AFX_XXOBJECTSHARED_H__A858D16A_BF76_44AB_85C2_0C05F985DC0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectShared : public XXObject  
{
	XDEC_XOBJECT(XXObjectShared)
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	virtual XBOOL SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	//void clear(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void flush(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void getLocal(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void getSize(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void dataGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void dataSet(XSWFCONTEXT&cnt,XXVARLIST&list);
public:
	XXObjectShared(ScriptPlayer*pRoot);
	virtual ~XXObjectShared();
	virtual XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*)
	{
		return (XU32)CreateObject(pCnt,list,XFALSE);
	}
	virtual XU32 Construct(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*)
	{
	   if(list.GetSize()>0)
		{
			list[0].ToString(XFALSE);			
			m_strPath=m_pRoot->splayer->m_strWorkPath;
			XXVar sh("/Shared/");
			m_strPath.StringAdd(sh);

			int l=0;
			const char*appName=m_pRoot->GetAppName(l);
			if(appName)
				m_strPath.StringAdd(appName,l);//m_pRoot->m_strAppName);
			int is=m_strPath.GetLength();
			m_strPath.StringAdd(list[0].strTxt);
			while(m_strPath.strTxt[is])
			{
				switch(m_strPath.strTxt[is])
				{
				case '\\':
				case '/':
				case '.':
					m_strPath.strTxt[is]='_';
					break;
				}
				is++;
			}
			LoadObject();			
		}
	   pCnt->pStack->Push(this);
	   return (XU32)this;
	}
	virtual void Destruct()
	{
		SaveObject();
		FREEOBJECT(m_pData);
		FREEOBJECT(m_pSuper);
	}
    
	virtual XU32 GetObjectType()
	{
		return XXOBJ_SHAREDOBJECT;
	}
 	
protected:
	XBOOL Load(XFile&file,XXVar&var);
	int Save(XFile&file,XPCTSTR strName);
	XBOOL Load(XFile&file,XXVar&name,XU8 bName,XXVar&var);
	int Save(XFile&file,XXVar&var,XPCTSTR strName);
	int Save(XFile&file,XXObject*pObj);
	int  SaveObject();
	XXVar m_strPath;
	XBOOL LoadObject();
	void InitObject();
	XXObject* m_pData;
};

#endif // !defined(AFX_XXOBJECTSHARED_H__A858D16A_BF76_44AB_85C2_0C05F985DC0D__INCLUDED_)
