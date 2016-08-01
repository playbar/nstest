// XData.cpp: implementation of the XRect class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XData.h"
#include "XFastAlloc.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XPoint XSize::operator +(const XPOINT &p)
{ return XPoint(cx+p.x,cy+p.y); }

XPoint XSize::operator -(const XPOINT &p)
{ return XPoint(cx-p.x,cy-p.y); }

XRect XSize::operator +(const XRECT &rect)
{  return XRect(rect.left+cx,rect.top+cy,rect.right+cx,rect.bottom+cy);}

XRect XSize::operator -(const XRECT &rect)
{  return XRect(rect.left-cx,rect.top-cy,rect.right-cx,rect.bottom-cy);}


XRect XPoint::operator +(const XRECT &r)const
{ return XRect(r.left+x,r.top+y,r.right+x,r.bottom+y);}


XRect XPoint::operator -(const XRECT &r)const
{ return XRect(r.left-x,r.top-y,r.right-x,r.bottom-y);}

///////////////////////////////////////////////////////////////
void XRect::operator&=(const XRECT& r)
{
	NormalizeRect();
	left=XMAX(left,XMIN(r.left,r.right));
	right=XMIN(right,XMAX(r.left,r.right));
	top=XMAX(top,XMIN(r.top,r.bottom));
	bottom=XMIN(bottom,XMAX(r.top,r.bottom));
	if(left>=right||top>=bottom)
		SetRectEmpty();
}

void XRect::operator|=(const XRECT& r)
{
	if(XRect(r).IsRectEmpty()) return;
	NormalizeRect();
	if(IsRectEmpty()) {*this=r;}
	else
	{
		left=XMIN(left,XMIN(r.left,r.right));
		right=XMAX(right,XMAX(r.left,r.right));
		top=XMIN(top,XMIN(r.top,r.bottom));
		bottom=XMAX(bottom,XMAX(r.top,r.bottom));
		if(left>=right||top>=bottom)
			SetRectEmpty();
	}
}
/////////////////////////////////////////////////////////////

