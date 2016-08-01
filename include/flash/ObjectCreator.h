// ObjectCreator.h: interface for the ObjectCreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTCREATOR_H__FD9FA56E_EF58_4FB1_BCD6_08A98041F706__INCLUDED_)
#define AFX_OBJECTCREATOR_H__FD9FA56E_EF58_4FB1_BCD6_08A98041F706__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"
#include "ObjectFunction.h"
#include "ObjectText.h"
#include "ObjectArray.h"
#include "ObjectString.h"
#include "ObjectClip.h"
#include "ObjectButton.h"
#include "ObjectMath.h"
#include "ObjectKey.h"
#include "XXObjectBitmap1.h"
#include "XXObjectPoint.h"
#include "XXObjectRectangle.h"
#include "XXObjectStage.h"
#include "XXObjectMouse.h"
#include "XXObjectColor.h"
#include "XXObjectColorTransform.h"
#include "XXObjectMatrix.h"
#include "XXObjectTransform.h"
#include "XXObjectShared.h"
#include "XXObjectSystem.h"

class XXObjectCreator:public XXObject
{
protected:	 
	struct _registerClasses
	{
		XString8 strName;
		XXObject*pObject;
		_registerClasses*pNext;
	};
public:
	_registerClasses* m_regs;
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	void updateAfterEvent(XSWFCONTEXT&cnt,XXVARLIST&list);
	void setTimeout(XSWFCONTEXT&cnt,XXVARLIST&list);
	void clearInterval(XSWFCONTEXT&cnt,XXVARLIST&list);
	void escape(XSWFCONTEXT&cnt,XXVARLIST&list);
	void getVersion(XSWFCONTEXT&cnt,XXVARLIST&list);
	void isFinite(XSWFCONTEXT&cnt,XXVARLIST&list);
	void isNaN(XSWFCONTEXT&cnt,XXVARLIST&list);
	void phaseFloat(XSWFCONTEXT&cnt,XXVARLIST&list);
	void phaseInt(XSWFCONTEXT&cnt,XXVARLIST&list);
	void setInterval(XSWFCONTEXT&cnt,XXVARLIST&list);
	void unescape(XSWFCONTEXT&cnt,XXVARLIST&list);

	~XXObjectCreator()
	{
		Release();
	}
	XXObjectCreator(ScriptPlayer*pRoot):XXObject(pRoot)
	{
		m_regs=XNULL;
		m_pArgs=XNULL;
		m_pClip=XNULL;
		m_pButton=XNULL;
		m_pText=XNULL;
		m_pSuper=XNULL;
//		m_pConst=XNULL;
		m_pStage=XNULL;
		m_pString=XNULL;
		m_pNumber=XNULL;
		m_pBool=XNULL;
		m_pClip=XNULL;
		m_pPoint=XNULL;
		m_pArray=XNULL;
		m_pNumber=XNULL;		
		m_pFunction=XNULL;
		m_pColorTransform=XNULL;
		m_pTransform=XNULL;
		m_pKey=XNULL;
		m_pMouse=XNULL;		
		m_bNoCase=XTRUE;
	}
	void Release()
	{

		XXObject::Release();
		_registerClasses*p=m_regs;
		while(p)
		{
			p->pObject->FreeObject();
			_registerClasses*pf=p;
			p=p->pNext;
			delete pf;
		}
		m_regs=XNULL;

		//m_pArgs->Destory();
		XDESTROY(m_pArgs);
		XDESTROY(m_pClip);
		XDESTROY(m_pButton);
		XDESTROY(m_pText);
//		XDESTROY(m_pConst);
		//XDELETE(m_pArgs);
		//XDELETE(m_pClip);
		//XDELETE(m_pButton);
		//XDELETE(m_pText);
		m_pArgs=XNULL;
		m_pClip=XNULL;
		m_pButton=XNULL;
		m_pText=XNULL;
		m_pSuper=XNULL;
		//m_pConst=XNULL;
		m_pString=XNULL;
		m_pNumber=XNULL;
		m_pBool=XNULL;
		m_pClip=XNULL;
		m_pPoint=XNULL;
		m_pArray=XNULL;
		m_pNumber=XNULL;		
		m_pFunction=XNULL;
		m_pColorTransform=XNULL;
		m_pMatrix=XNULL;
		m_pTransform=XNULL;
		m_pMouse=XNULL;
		m_pKey=XNULL;
	}
protected:
	virtual XU32 GetObjectType()
	{
		return XXOBJ_CREATOR;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
	
	//void Init();
	/*XBOOL CreateObject(XPCWSTR strName,XSWFCONTEXT*pCnt,XXVARLIST*pList)
	{
		return NewMethod(strName,pCnt,pList,XTRUE,XTRUE);
	}
	XBOOL CreateMethod(XPCWSTR strName,XSWFCONTEXT*pCnt,XXVARLIST*pList)
	{
		return NewMethod(strName,pCnt,pList,XFALSE,XTRUE);
	}*/
protected:
	void InitObject();
	//XXObjectConst* m_pConst;
	XXObjectBool*  m_pBool;
	XXObjectFunction*m_pFunction;
	XXObjectString*m_pString;
	XXObjectNumber*m_pNumber;
	XXObjectClip*  m_pClip;
	XXObjectArray* m_pArray;
	XXObjectArgs*  m_pArgs;
	XXObjectButton*m_pButton;
	XXObjectText*  m_pText;
	XXObjectPoint* m_pPoint;
	XXObjectRectangle*m_pRect;
	XXObjectColorTransform*m_pColorTransform;
	XXObjectMatrix*m_pMatrix;
	XXObjectTransform*m_pTransform;
public:
	XXObjectMouse* m_pMouse;
	XXObjectKey  * m_pKey;
	XXObjectStage* m_pStage;




public:
	void getGeom(XSWFCONTEXT&cnt,XXVARLIST&list);
	void loadVariables(XSWFCONTEXT&cnt,XXVARLIST&list);
	XXObject* FindRegisterClass(XPCTSTR strName);
	XBOOL RegisterClass(XSWFCONTEXT&cnt,XXVar&var,XXObject*pObj);
	void Init(XBOOL nMode);
	XXObjectTransform* CreateTransform(XXObjectClip*pm)
	{
		XXObjectTransform*p=XXObjectTransform::CreateClass(m_pTransform,XFALSE,m_pRoot);
		if(p)
		{
			p->m_pSuperObj=m_pTransform->GetSuperObject();
			p->SetClip(pm);
		}
		return p;
	}
	XXObjectPoint* CreatePoint()
	{
		XXObjectPoint*p= XXObjectPoint::CreateClass(m_pPoint,XFALSE,m_pRoot);
		if(p)
		{
			p->m_pSuperObj=m_pPoint->GetSuperObject();
		}
		return p;
	}
	XXObjectRectangle* CreateRectangle()
	{
		XXObjectRectangle*p= XXObjectRectangle::CreateClass(m_pRect,XFALSE,m_pRoot);
		if(p)
			p->m_pSuperObj=m_pRect->GetSuperObject();
		return p;
	}
	XXObjectArgs* GetArgsBase()
	{
		return m_pArgs;
	}
	XXObject* CreateObject()
	{
		return XXObject::CreateClass(m_pSuper,XFALSE,m_pRoot);
	}
	XXObjectText* CreateText(SObject*pText)
	{
		XXObjectText*p=XXObjectText::CreateClass(m_pText,XFALSE,m_pRoot);
		if(p)
		{
			p->m_pSuperObj=m_pText->GetSuperObject();
			p->SetObject(pText);
		}
		return p;
	}
	XXObjectButton* CreateButton(SObject*pBut)
	{
		XXObjectButton*p=XXObjectButton::CreateClass(m_pButton,XFALSE,m_pRoot);
		if(p) 
		{
			p->m_pSuperObj=m_pButton->GetSuperObject();
			p->SetObject(pBut);
		}
		return p;
	}
	XXObjectArray* CreateArray()
	{
		XXObjectArray*p= XXObjectArray::CreateClass(m_pArray,XFALSE,m_pRoot);
		if(p)
		{
			p->m_pSuperObj=m_pArray->GetSuperObject();
		}
		return p;
	}
	XXObjectFunction* CreateFunction(XPCTSTR c,int s)//XVector<XPCTSTR>& c)
	{
		XXObjectFunction*pFun=XXObjectFunction::CreateClass(m_pFunction,XFALSE,m_pRoot);
		if(pFun)//&&c)
		{
			pFun->m_pSuperObj=m_pFunction->GetSuperObject();
			pFun->SetConstant(c,s);//c[0],c.GetSize());
		}
		//else
		//	pFun->SetConstant(XNULL,0);
		return pFun;
	}
	XXObjectMatrix*CreateMatrix()
	{
		XXObjectMatrix*p=XXObjectMatrix::CreateClass(m_pMatrix,XFALSE,m_pRoot);
		if(p)
		{
			p->m_pSuperObj=m_pMatrix->GetSuperObject();
		}
		return p;
	}
	XXObjectColorTransform* CreateColorTransform()
	{
		XXObjectColorTransform*p=XXObjectColorTransform::CreateClass(
			m_pColorTransform,XFALSE,m_pRoot);
		if(p)
		{
			p->m_pSuperObj=m_pColorTransform->GetSuperObject();
		}
		return p;
	}
	XXObjectNumber* CreateNumber(XFDOUBLE v)
	{
		XXObjectNumber*p=XXObjectNumber::CreateClass((m_pNumber),XFALSE,m_pRoot);
		if(p) 
		{
			p->m_pSuperObj=m_pNumber->GetSuperObject();
			p->SetValue(v);
		}
		return p;
	}
	XXObjectString* CreateString(XXVar&var)//XPCTSTR pStr)
	{
		XXObjectString*p=XXObjectString::CreateClass((m_pString),XFALSE,m_pRoot);
		if(p) 
		{
			p->m_pSuperObj=m_pString->GetSuperObject();
			p->m_strValue=var;
			//p->SetString(pStr);
		}
		return p;
	}
	XXObjectBool* CreateBool(XBOOL v)
	{
		XXObjectBool*p=XXObjectBool::CreateClass((m_pBool),XFALSE,m_pRoot);
		if(p) 
		{
			p->m_pSuperObj=m_pBool->GetSuperObject();
			p->SetValue(v);
		}
		return p;
	}
	XXObjectClip* CreateClip(SObject*pThread)//,XXObjectClip*pParent)
	{
		//if(pParent==XNULL)
		//	pParent
		XXObjectClip*p=XXObjectClip::CreateClass((m_pClip),XFALSE,m_pRoot);
		if(p) 
		{
			p->m_pSuperObj=m_pClip->GetSuperObject();
			p->SetObject(pThread);
		}
		return p;
	}
	/*XXObjectConst* CreateConst(XU8 c)
	{
		XXObjectConst*p=XXObjectConst::CreateClass((m_pConst),XFALSE,m_pRoot);
		if(p) 
		{
			p->m_pSuperObj=m_pConst->GetSuperObject();
			p->SetValue(c);
		}
		return p;
	}*/
	XXObject* GetObjBase(){return m_pSuper;}
	//static XXObject* GetGlobal(){return &_global;}
	//static XXObject* GetObjectBase(){return REFOBJ(_global.m_pSuper);}
	//static void InitGlobal(XU8 nMode)
	//{
	//	_global.Init(nMode);
	//}
	//static void ReleaseGlobal()
	//{
	//	_global.Release();
	//}
	//static XXObjectCreator _global;
	//static XVector<XXObject*> _freeObjs;
	
};

#endif // !defined(AFX_OBJECTCREATOR_H__FD9FA56E_EF58_4FB1_BCD6_08A98041F706__INCLUDED_)
