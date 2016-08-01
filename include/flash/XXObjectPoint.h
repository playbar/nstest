// XXObjectPoint.h: interface for the XXObjectPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTPOINT_H__5F57358C_A2CC_44CA_A362_FF1208E474F0__INCLUDED_)
#define AFX_XXOBJECTPOINT_H__5F57358C_A2CC_44CA_A362_FF1208E474F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectPoint : public XXObject  
{
	XDEC_XOBJECT(XXObjectPoint)
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	virtual XBOOL SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);

	/*void xGetPT(XSWFCONTEXT&cnt,XXVARLIST&list);
	void xSetPT(XSWFCONTEXT&cnt,XXVARLIST&list);
	void yGetPT(XSWFCONTEXT&cnt,XXVARLIST&list);
	void ySetPT(XSWFCONTEXT&cnt,XXVARLIST&list);
	void lengthGet(XSWFCONTEXT&cnt,XXVARLIST&list);*/
	void lengthSet(XSWFCONTEXT&cnt,XFDOUBLE l);
	void clonePT(XSWFCONTEXT&cnt,XXVARLIST&list);
	void addPT(XSWFCONTEXT&cnt,XXVARLIST&list);
	void distancePT(XSWFCONTEXT&cnt,XXVARLIST&list);
	void equalsPT(XSWFCONTEXT&cnt,XXVARLIST&list);
	void interpolate(XSWFCONTEXT&cnt,XXVARLIST&list);
	void normalizePT(XSWFCONTEXT&cnt,XXVARLIST&list);
	void offsetPT(XSWFCONTEXT&cnt,XXVARLIST&list);
	void polar(XSWFCONTEXT&cnt,XXVARLIST&list);
	void subtractPT(XSWFCONTEXT&cnt,XXVARLIST&list);
	XXObjectPoint(ScriptPlayer*pRoot);
	virtual ~XXObjectPoint();
	virtual XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*pCaller)
	{
		return (XU32)CreateObject(pCnt,list,XFALSE);
	}
	virtual XU32 Construct(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*)
	{
		if(list.GetSize()>0)
			xPos=list[0].ToFloat();
		if(list.GetSize()>1)
			yPos=list[1].ToFloat();
		pCnt->pStack->Push(this);
		return (XU32)this;
	}
	virtual XU32 GetObjectType()
	{
		return XXOBJ_POINT;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
	//XPOINT& GetPoint(){return m_point;}
	double GetX(){return xPos;}
	double GetY(){return yPos;}
	void GetPoint(XPOINT&pt)
	{
		pt.x=XROUND(xPos);
		pt.y=XROUND(yPos);
	}
protected:
	XXObjectPoint* Clone();
	virtual void GetString(XXVar&str);//XString8&str);
	void InitObject( );
	//XPoint m_point;
public:
	XFDOUBLE xPos,yPos;
};

#endif // !defined(AFX_XXOBJECTPOINT_H__5F57358C_A2CC_44CA_A362_FF1208E474F0__INCLUDED_)
