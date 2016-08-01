// ObjectClip.h: interface for the ObjectClip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTCLIP_H__3108B138_2CEE_4704_8E38_14557503443F__INCLUDED_)
#define AFX_OBJECTCLIP_H__3108B138_2CEE_4704_8E38_14557503443F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XDTSpriteDisp;

class XXObjectClip:public XXObject//Event
{
	XDEC_XOBJECT(XXObjectClip);
public:
	/*virtual XBOOL FreeObject()
	{
		XBOOL bOK=XXObject::FreeObject();
		if(m_bDestroy)
			XXObject::Release();
		return bOK;
	}*/
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	virtual XBOOL SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	void ReleaseClip();
	virtual ScriptThread* GetTarget()
	{
		if(!m_pObject) return XNULL;
		return m_pObject->thread;
	}

public:
	//XBOOL m_bDestroy;
	XXVar m_strPath;
//	XString8 m_strPath;
//	XXObjectFunction*m_pFuns;
//	XString8 m_strPath;
	virtual void Destroy()
	{
		if(m_pObject&&m_pRoot)
			m_pRoot->splayer->GetTargetPath(m_pObject,m_strPath);
		XXObject::FreeObject();
		//m_bDestroy=XTRUE;
		//XXObject::Destroy();
		ReleaseClip();
	}
	/*virtual void Release()
	{
		if(m_pObject&&m_pRoot)
			m_pRoot->splayer->GetTargetPath(m_pObject,m_strPath);
		XXObject::Release();
		//m_list.Release(); 
		ReleaseClip();
	}*/
	virtual void Destruct()
	{
		ReleaseClip();
		FREEOBJECT(m_pSuper);
	}

protected:	
	void GetString(XXVar&str);//XString8&str);
	virtual XXObject* GetParent();
	virtual XBOOL GetClipOf(XPCTSTR strName,XXVar&var);
	virtual XU32 GetObjectType()
	{
		return XXOBJ_CLIP;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
	void InitObject();
	XXObjectClip(ScriptPlayer*pRoot):XXObject(pRoot)
	{
#ifdef _DEBUG
		nObjCount[XXOBJ_CLIP]++;
#endif
		m_strPath=_strEmpty;
		m_pObject=XNULL;
		
		m_nDataType=XODT_MOVIECLIP;
		m_bNoCase=XTRUE;
		//m_bDestroy=XFALSE;
//		m_pFuns=XNULL;
	}
	
public:	 
	//void RemoveFunction(XXObjectFunction*pFun);
	//void AddFunction(XXObjectFunction*pFun);
	void SetPixelRect(SRECT&rect);
	void GetTotalMatrix(MATRIX&mat);
	void GetPixelRect(SRECT&rect);
	void SetMatrix(MATRIX&mat);
	void GetMatrix(MATRIX&mat);
	void SetCXForm(XCXForm&form);
	void GetCXForm(XCXForm&form);
	void SetForeColor(XU32 color);
	XU32 GetForeColor();
	void SetObject(SObject*p)
	{
		//if(p)
		//{
		//	m_strPath.SetString(XNULL,64);
		//	m_pRoot->splayer->GetTargetPath(p,m_strPath);
		//}
		m_pObject=p;
	}
	~XXObjectClip();
//	SObject* m_pObject;
	//ScriptThread*m_pThread;
	XU32 GetIndex(XXVar*pVar,XU32 id,XXVar*pName);
//DEL 	static XXObjectClip* CreateClass(XXObject*pSuper,
//DEL 									 XXObject*pParent,
//DEL 									 XDTSpriteDisp*pDisp,XU8 bInit)
//DEL 	{
//DEL 		XXObjectClip*pObj=new XXObjectClip(pDisp);
//DEL 		if(pObj)
//DEL 		{
//DEL 			if(bInit)
//DEL 				pObj->InitObject();
//DEL 			pObj->m_bBase=bInit;
//DEL 			pObj->SetParent(pParent);
//DEL 			pObj->SetSuper(pSuper,bInit);
//DEL 		}
//DEL 		return pObj;
//DEL 	}
	void GetTargetPath(XSWFCONTEXT&cnt,XXVar&var);
private:
	void beginGradientFill(XSWFCONTEXT&cnt,XXVARLIST&list);
	void globalToLocal(XSWFCONTEXT&cnt,XXVARLIST&list);
	void localToGlobal(XSWFCONTEXT&cnt,XXVARLIST&list);
	XBOOL hitTest(XSWFCONTEXT&cnt,XXVARLIST&list);
};

#endif // !defined(AFX_OBJECTCLIP_H__3108B138_2CEE_4704_8E38_14557503443F__INCLUDED_)
