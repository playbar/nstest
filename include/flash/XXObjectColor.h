// XXObjectColor.h: interface for the XXObjectColor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTCOLOR_H__0F000A10_32F5_4E11_86E9_71D547B206FC__INCLUDED_)
#define AFX_XXOBJECTCOLOR_H__0F000A10_32F5_4E11_86E9_71D547B206FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"
class XXObjectClip;

class XXObjectColor : public XXObject  
{
	XDEC_XOBJECT(XXObjectColor);
public:
	virtual void Destruct();
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	//virtual XBOOL SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	//void getRGB(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void setRGB(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void getTransform(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void setTransform(XSWFCONTEXT&cnt,XXVARLIST&list);
public:
	XXObjectColor(ScriptPlayer*pRoot);
	virtual ~XXObjectColor();
	virtual XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*)
	{
		return (XU32)CreateObject(pCnt,list,XFALSE);
	}
	virtual XU32 Construct(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*);
	virtual XU32 GetObjectType()
	{
		return XXOBJ_COLOR;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
protected:
	void InitObject( );
	XXObjectClip* m_pClip;
};

#endif // !defined(AFX_XXOBJECTCOLOR_H__0F000A10_32F5_4E11_86E9_71D547B206FC__INCLUDED_)
