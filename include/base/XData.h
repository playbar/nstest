#ifndef _XDATA_
#define _XDATA_


#include "GGType.h"

class XPoint;
class XRect;
class XSize;

class _XEXT_CLASS XSize : public _XSIZE
{
public:
	/*#ifdef WIN32
	operator SIZE&(){return *((SIZE*)this);}
	operator SizeF(){return SizeF((REAL)cx,(REAL)cy);}
	XSize(SIZE&s)
	{
		cx=s.cx;
		cy=s.cy;
	}
	#endif*/
	XRect  operator+(const XRECT&r);
	XRect  operator-(const XRECT&r);
	XPoint operator-(const XPOINT&p);
	XPoint operator+(const XPOINT&p);
	XSize() {cx=cy=0;}
	XSize(XINT x,XINT y){cx=x;cy=y;}
	XSize(const XSIZE&s) {cx=s.cx;cy=s.cy;}
	XSize(const XPOINT&p) {cx=p.x;cy=p.y;}
	XBOOL operator==(const XSIZE&s)
	{return cx==s.cx&&cy==s.cy;}
	XBOOL operator!=(const XSIZE&s)
	{return cx!=s.cy||cy!=s.cy;}
	void operator+=(const XSIZE&s){cx+=s.cx;cy+=s.cy;}
	void operator-=(const XSIZE&s){cx-=s.cy;cy-=s.cy;}
	XSize operator-(const XSIZE&s)
	{ return XSize(cx-s.cx,cy-s.cy);}
	XSize operator+(const XSIZE&s)
	{ return XSize(cx+s.cy,cy+s.cy);}
};

class _XEXT_CLASS XPoint : public _XPOINT
{
public:
	/*#ifdef WIN32
	operator POINT&()const
	{return *((POINT*)this);}
	operator PointF()const
	{return PointF((REAL)x,(REAL)y);}
	operator Point&()const
	{	return *((Point*)this);}
	XPoint(POINT&pt)
	{	x=pt.x;
		y=pt.y;	}
	#endif*/
	XPoint() {x=0;y=0;}
	XPoint(XINT ix,XINT iy){x=ix;y=iy;}
	XPoint(const XPOINT&p)
	{	x=p.x;y=p.y;	}
	void Offset(XINT ox,XINT oy)
	{ x+=ox;y+=oy;}
	void operator+=(const XPOINT&p)
	{ x+=p.x;y+=p.y;}
	void operator+=(const XSIZE&s)
	{ x+=s.cx;y+=s.cy;}
	void operator-=(const XPOINT&p)
	{ x-=p.x;y-=p.y;}
	void operator-=(const XSIZE&s)
	{ x-=s.cx;y-=s.cy;}
	XBOOL operator==(const XPOINT&p)
	{ return x==p.x&&y==p.y;}
	XBOOL operator!=(const XPOINT&p) const
	{ return x!=p.x||y!=p.y;}
	XPoint operator+(const XSIZE&s)const
	{ return XPoint(x+s.cx,y+s.cy);}
	XPoint operator-(const XSIZE&s)const
	{ return XPoint(x-s.cx,y+s.cy);}
	XPoint operator+(const XPOINT&p)const
	{ return XPoint(x+p.x,x+p.y);}
	XPoint operator-(const XPOINT&p)const
	{ return XPoint(x-p.x,x-p.y);}
	XRect operator+(const XRECT&r)const;
	XRect operator-(const XRECT&r)const;
};

#define rectEmptyFlag 0x80000000L

class _XEXT_CLASS XRect : public _XRECT  
{
public:
	#ifdef _WINDOWS
	operator RECT&()const
	{return *((RECT*)this);}
	XRect(RECT&r)
	{
		left=r.left;
		top=r.top;
		right=r.right;
		bottom=r.bottom;
	}
	#endif
	XRect(){left=right=top=bottom=0;}
	XRect(XINT l,XINT t,XINT r,XINT b)
	{ SetRect(l,t,r,b);}
	XRect(const XRECT&r)
	{
	  SetRect(r.left,r.top,r.right,r.bottom);
	}
	XRect(const XPOINT&p,const XSIZE&s)
	{ SetRect(p.x,p.y,p.x+s.cx,p.y+s.cy); }
	XRect(const XPOINT&p1,const XPOINT&p2)
	{ SetRect(p1.x,p1.y,p2.x,p2.y);}
	XINT Width()const {return right-left;}
	XINT Height()const {return bottom-top;}
	XSize Size()const {return XSize(right-left,bottom-top);}
	XPOINT& TopLeft()const {return *(XPOINT*)this;}
	XPOINT& BottomRightconst (){return *((XPOINT*)this+1);}
	XPoint CenterPoint()const {return XPoint((left+right)>>1,(top+bottom)>>1);}
	void SwapLeftRight()
	{ XINT t=left;left=right;right=t;}
	void SwapTopBottom()
	{ XINT t=top;top=bottom;bottom=t;}
	XBOOL IsRectEmpty()
	{	if((XU32)left==rectEmptyFlag) return XTRUE;
		return left>=right||top>=bottom;
	}
	XBOOL IsRectNull()
	{ return left==0&&right==0&&top==0&&bottom==0;}
	XBOOL PtInRect(const XPOINT&pt)
	{
		return pt.x>=left&&pt.x<=right&&
			   pt.y>=top&&pt.y<=bottom;
	}
	XBOOL PtInRect(XINT x,XINT y)
	{
		return x>=left&&x<=right&&
			   y>=top&&y<=bottom;
	}
	void SetRect(XINT l,XINT t,XINT r,XINT b)
	{ 	left=l;top=t;right=r;bottom=b;	}
	void GetRect(XRECT&rect)
	{
		rect.left=left;
		rect.top=top;
		rect.bottom=bottom;
		rect.right=right;
	}
	void SetRectEmpty() {left=right=top=bottom=rectEmptyFlag;}
	void InflateRect(int x,int y) { InflateRect(x,y,x,y); }
	void InflateRect(const XSIZE&s){ InflateRect(s.cx,s.cy,s.cx,s.cy);}
	void InflateRect(const XRECT&r){ InflateRect(r.left,r.top,r.right,r.bottom);}
	void InflateRect(int l,int t,int r,int b)
	{ left-=l;top-=t;right+=r;bottom+=b;}

	void DeflateRect(int x,int y) {DeflateRect(x,y,x,y);}
	void DeflateRect(const XSIZE&s){DeflateRect(s.cx,s.cy,s.cx,s.cy);}
	void DeflateRect(const XRECT&r){DeflateRect(r.left,r.top,r.right,r.bottom);}
	void DeflateRect(int l,int t,int r,int b)
	{ left+=l;top+=t;right-=r;bottom-=b;}

	void OffsetRect(int x,int y)
	{
		left+=x;right+=x;
		top+=y;bottom+=y;
	}
	void OffsetRect(const XSIZE&s)
	{	OffsetRect(s.cx,s.cy);	}
	void OffsetRect(const XPOINT&p) {OffsetRect(p.x,p.y);}
	void NormalizeRect()
	{
		if(left>right) SwapLeftRight();
		if(top>bottom) SwapTopBottom();
	}
	XBOOL IsUnion(const XRECT&rect)
	{
		if(right<rect.left||
		   bottom<rect.top||
		   left>rect.right||
		   top>rect.bottom) return XFALSE;
		return XTRUE;
	}
	//void operator=(const XRECT& srcRect);
	XBOOL operator==(const XRECT& rect)
	{
		/*return XMIN(left,right)==XMIN(rect.left,rect.right)&&
			   XMAX(left,right)==XMAX(rect.left,rect.right)&&
			   XMIN(top,bottom)==XMIN(rect.top,rect.bottom)&&
			   XMAX(top,bottom)==XMAX(rect.top,rect.bottom);*/
		return left==rect.left&&top==rect.top&&
			   right==rect.right&&bottom==rect.bottom;
	}
	XBOOL operator!=(const XRECT& rect)
	{
		return !(*this==rect);
	}
	void operator+=(const XPOINT& point){OffsetRect(point.x,point.y);}
	void operator+=(const XSIZE& size){OffsetRect(size.cx,size.cy);}
	void operator+=(const XRECT& r){InflateRect(r.left,r.top,r.right,r.bottom);}
	void operator-=(const XPOINT& point){OffsetRect(-point.x,-point.y);}
	void operator-=(const XSIZE& size){OffsetRect(-size.cx,-size.cy);}
	void operator-=(const XRECT& r){DeflateRect(r.left,r.top,r.right,r.bottom);}
	void operator&=(const XRECT& r);
	void operator|=(const XRECT& r);

// Operators returning CRect values
	XRect operator+(const XPOINT&point) const
	{
		XRect rect(*this);rect+=point;
		return rect;
	}
	XRect operator-(const XPOINT&point) const
	{
		XRect rect(*this);
		rect-=point;
		return rect;
	}
	XRect operator+(const XRECT&r) const
	{
		XRect rect(*this);
		rect+=r;
		return rect;
	}
	XRect operator+(const XSIZE&size) const
	{
   		XRect rect(*this);
		rect+=size;
		return rect;
	}
	XRect operator-(const XSIZE&size) const
	{
		XRect rect(*this);
		rect-=size;
		return *this;
	}
	XRect operator-(const XRECT&r) const
	{
		XRect rect(*this);
		rect+=r;
		return rect;
	}
	XRect operator&(const XRECT& r) const
	{
		XRect rect(*this);
		rect&=r;
		return *this;
	}
	XRect operator|(const XRECT& r) const
	{
		XRect rect(*this);
		rect|=r;
		return rect;

	}
	XBOOL IntersectRect(const XRECT&r1,const XRECT&r2)
	{
		*this=r1;
		*this&=r2;
		return !IsRectEmpty();
	}
	XBOOL UnionRect(const XRECT&r1,const XRECT&r2)
	{
		*this=r1;
		*this|=r2;
		return !IsRectEmpty();
	}
};

//#define MIXCOLOR(CC,MC,MIX) {XColor c=MC;c*=MIX;CC*=(1-MIX);CC+=c;}

class _XEXT_CLASS XColor : public XCOLOR
{
public:
	XColor& operator*=(float v)
	{
		r=(XU8)(XMIN(XMAX(r*v,0),0xff));
		g=(XU8)(XMIN(XMAX(g*v,0),0xff));
		b=(XU8)(XMIN(XMAX(b*v,0),0xff));
		return *this;
	}
	void Alpha(XU8 v){a=v;}
	operator XU32()const
	{return nColor;}
	operator XU8*()const
	{return (XU8*)&nColor;}
	XColor(XU32 c) {nColor=c;}
	void Bright(int nPec)
	{
	   r=XMAX(XMIN(r+XMAX(100,r)*nPec/100,0xff),0);
	   g=XMAX(XMIN(g+XMAX(100,g)*nPec/100,0xff),0);
	   b=XMAX(XMIN(b+XMAX(100,b)*nPec/100,0xff),0);
	}
	void Mix(const XCOLOR&c)
	{
		float v=c.a/255.0f;
		r=(XU8)XMAX(XMIN(r*(1-v)+c.r*v,0xff),0);
		g=(XU8)XMAX(XMIN(g*(1-v)+c.g*v,0xff),0);
		b=(XU8)XMAX(XMIN(b*(1-v)+c.b*v,0xff),0);
	}
	void Dark(int nPec)
	{
	   r=XMIN(XMAX(r-r*nPec/100,0),0xff);
	   g=XMIN(XMAX(g-g*nPec/100,0),0xff);
	   b=XMIN(XMAX(b-b*nPec/100,0),0xff);
	}
	void operator +=(XINT v)
	{
		r=XMAX(XMIN(r+v,0xff),0);
		g=XMAX(XMIN(g+v,0xff),0);
		b=XMAX(XMIN(b+v,0xff),0);
	}
	void operator -=(XINT v)
	{	*this+=(-v); }
	void Mix(const XCOLOR&c,float v)
	{
		r=(XU8)XMAX(XMIN(r*(1-v)+c.r*v,0xff),0);
		g=(XU8)XMAX(XMIN(g*(1-v)+c.g*v,0xff),0);
		b=(XU8)XMAX(XMIN(b*(1-v)+c.b*v,0xff),0);
	}
	void operator+=(const XCOLOR c)
	{
		r=XMAX(XMIN((int)r+c.r,0xff),0);
		g=XMAX(XMIN((int)g+c.g,0xff),0);
		b=XMAX(XMIN((int)b+c.b,0xff),0);
		//a=XMIN(XMAX((int)a-c.a,0xff),0);
	}
	void operator-=(const XCOLOR c)
	{
		r=XMAX(XMIN((int)r-c.r,0xff),0);
		g=XMAX(XMIN((int)g-c.g,0xff),0);
		b=XMAX(XMIN((int)b-c.b,0xff),0);
		//a=XMIN(XMAX((int)a-c.a,0xff),0);
	}
};

#endif 
