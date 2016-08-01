// XXObjectMouse.h: interface for the XXObjectMouse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTMOUSE_H__F06BBAF3_3D39_4595_B809_6AC3B48CE031__INCLUDED_)
#define AFX_XXOBJECTMOUSE_H__F06BBAF3_3D39_4595_B809_6AC3B48CE031__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectMouse : public XXObject//Event  
{
	XDEC_XOBJECT(XXObjectMouse)
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	//void addListenerMouse(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void hideMouse(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void removeListenerMouse(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void showMouse(XSWFCONTEXT&cnt,XXVARLIST&list);
protected:
	void InitObject( );
	virtual XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*)
	{
		return (XU32)CreateObject(pCnt,list,XFALSE);
	}
	virtual XU32 GetObjectType()
	{
		return XXOBJ_MOUSE;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
public:
	XXObjectMouse(ScriptPlayer*pRoot);
	virtual ~XXObjectMouse();

};

#endif // !defined(AFX_XXOBJECTMOUSE_H__F06BBAF3_3D39_4595_B809_6AC3B48CE031__INCLUDED_)
