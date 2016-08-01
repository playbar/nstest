// XXObjectColorTransform.h: interface for the XXObjectColorTransform class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTCOLORTRANSFORM_H__249F54E9_7950_40CE_A21B_6CDF5F2EA6ED__INCLUDED_)
#define AFX_XXOBJECTCOLORTRANSFORM_H__249F54E9_7950_40CE_A21B_6CDF5F2EA6ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

const XPCTSTR _strColorTransformIndex[]=
{	_SYSNAME(redMultiplier),//_strRedMulti,
	_SYSNAME(greenMultiplier),//_strGreenMulti,
	_SYSNAME(blueMultiplier),//_strBlueMulti,
	_SYSNAME(alphaMultiplier),//_strAlphaMulti,
	_SYSNAME(redOffset),//_strRedOffset,
	_SYSNAME(greenOffset),//_strGreenOffset,
	_SYSNAME(blueOffset),//_strBlueOffset,
	_SYSNAME(alphaOffset),//_strAlphaOffset
};

class XXObjectColorTransform : public XXObject  
{
	XDEC_XOBJECT(XXObjectColorTransform);
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	virtual XBOOL SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	void concatCXForm(XSWFCONTEXT&cnt,XXVARLIST&list);
	
public:
	XXObjectColorTransform(ScriptPlayer*pRoot);
	virtual ~XXObjectColorTransform();
public:
	virtual XU32 Construct(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*);
	float GetIndex(int id);
	void ToCXForm(XCXForm&f){f=cxForm;}
	void FromCXForm(XCXForm&f){cxForm=f;}
	virtual XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*)
	{
		return (XU32)CreateObject(pCnt,list,XFALSE);
	}
	virtual XU32 GetObjectType()
	{
		return XXOBJ_COLORTRANSFORM;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
	//float alphaMulti,redMulti,greenMulti,blueMulti;
	//XS16  alphaAdd,redAdd,greenAdd,blueAdd;
	XCXForm cxForm;
protected:
	virtual void GetString(XXVar&var);//XString8&str);
	void InitObject( );
};

#endif // !defined(AFX_XXOBJECTCOLORTRANSFORM_H__249F54E9_7950_40CE_A21B_6CDF5F2EA6ED__INCLUDED_)
