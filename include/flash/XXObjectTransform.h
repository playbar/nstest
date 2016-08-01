// XXObjectTransform.h: interface for the XXObjectTransform class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTTRANSFORM_H__A3A32F07_AB5E_4619_986C_DDCD4CD823F7__INCLUDED_)
#define AFX_XXOBJECTTRANSFORM_H__A3A32F07_AB5E_4619_986C_DDCD4CD823F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"
class XXObjectClip;

class XXObjectTransform : public XXObject  
{
	XDEC_XOBJECT(XXObjectTransform);
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	//void colorTransformGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void colorTransformSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void concatenatedColorTransformGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void matrixGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void matrixSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void concatenatedMatrixGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void pixelBoundsGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void pixelBoundsSet(XSWFCONTEXT&cnt,XXVARLIST&list);	
public:
	void Destruct();
	virtual XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*)
	{
		return (XU32)CreateObject(pCnt,list);
	}
	virtual XU32 Construct(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*);
	XXObjectClip*GetClip(){return m_pClip;}
	void SetClip(XXObjectClip*p);
	XXObjectTransform(ScriptPlayer*pRoot);
	virtual ~XXObjectTransform();
	virtual XU32 GetObjectType()
	{
		return XXOBJ_TRANSFORM;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
protected:
	void InitObject();
	XXObjectClip* m_pClip;
};

#endif // !defined(AFX_XXOBJECTTRANSFORM_H__A3A32F07_AB5E_4619_986C_DDCD4CD823F7__INCLUDED_)
