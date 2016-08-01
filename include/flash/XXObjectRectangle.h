// XXObjectRectangle.h: interface for the XXObjectRectangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTRECTANGLE_H__AB27DC53_8C10_4389_A2A4_EBCACA8A1528__INCLUDED_)
#define AFX_XXOBJECTRECTANGLE_H__AB27DC53_8C10_4389_A2A4_EBCACA8A1528__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectRectangle : public XXObject  
{
	XDEC_XOBJECT(XXObjectRectangle);
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	virtual XBOOL SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	/*void bottomGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void bottomSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void bottomRightSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void bottomRightGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void heightSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void heightGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void leftGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void leftSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void rightGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void rightSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void sizeGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void sizeSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void topGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void topSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void topLeftGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void topLeftSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void widthGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void widthSet(XSWFCONTEXT&cnt,XXVARLIST&list);*/

	void cloneRect(XSWFCONTEXT&cnt,XXVARLIST&list);
	void contains(XSWFCONTEXT&cnt,XXVARLIST&list);
	void containsPoint(XSWFCONTEXT&cnt,XXVARLIST&list);
	void containsRectangle(XSWFCONTEXT&cnt,XXVARLIST&list);
	void equalsRect(XSWFCONTEXT&cnt,XXVARLIST&list);
	void inflate(XSWFCONTEXT&cnt,XXVARLIST&list);
	void inflatePoint(XSWFCONTEXT&cnt,XXVARLIST&list);
	void intersection(XSWFCONTEXT&cnt,XXVARLIST&list);
	void intersects(XSWFCONTEXT&cnt,XXVARLIST&list);
	void isEmpty(XSWFCONTEXT&cnt,XXVARLIST&list);
	void setEmpty(XSWFCONTEXT&cnt,XXVARLIST&list);
	void offsetRect(XSWFCONTEXT&cnt,XXVARLIST&list);
	void offsetPoint(XSWFCONTEXT&cnt,XXVARLIST&list);
	void unionRect(XSWFCONTEXT&cnt,XXVARLIST&list);
public:
	void GetRect(XRECT&rect,XU8 bZoom);
	XXObjectRectangle(ScriptPlayer*pRoot);
	virtual ~XXObjectRectangle();
	virtual XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*)
	{
		return (XU32)CreateObject(pCnt,list,XFALSE);
	}
	virtual XU32 Construct(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*)
	{
		int ns=list.GetSize();
	    if(ns>0)
		   left=list[0].ToFloat();
	    if(ns>1)
		   top=list[1].ToFloat();
	    if(ns>2)
		   width=list[2].ToFloat();
	    if(ns>3)
		   height=list[3].ToFloat();
		pCnt->pStack->Push(this);
		return (XU32)this;
	}
	virtual XU32 GetObjectType()
	{
		return XXOBJ_RECTANGLE;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
public:
	virtual void GetString(XXVar&var);//XString8&str);
	XXObjectRectangle* Clone();
	XFDOUBLE left,top,width,height;
protected:
	
	void InitObject( );
	
};

#endif // !defined(AFX_XXOBJECTRECTANGLE_H__AB27DC53_8C10_4389_A2A4_EBCACA8A1528__INCLUDED_)
