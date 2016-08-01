// XXObjectRectangle.cpp: implementation of the XXObjectRectangle class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XXObjectRectangle.h"
#include "splayer.h"
#include "XXObjectPoint.h"
#include "ObjectCreator.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XXObjectRectangle::XXObjectRectangle(ScriptPlayer*pRoot):XXObject(pRoot)
{
   left=0;
   width=0;
   top=0;
   height=0;
}

XXObjectRectangle::~XXObjectRectangle()
{

}

XXObject* XXObjectRectangle::CreateObject(XSWFCONTEXT *pCnt, XXVARLIST &list,XBOOL)
{
   XXObjectRectangle*pObj=Clone();
   pCnt->pStack->Push((pObj));
   if(pObj)
   {
	   pObj->SetSuper(this,XFALSE);
	   pObj->m_pSuperObj=GetSuperObject();
	   //pObj->m_pSuperObject=GetSuperObject();
	   int ns=list.GetSize();
	   if(ns>0)
		   pObj->left=list[0].ToFloat();
	   if(ns>1)
		   pObj->top=list[1].ToFloat();
	   if(ns>2)
		   pObj->width=list[2].ToFloat();
	   if(ns>3)
		   pObj->height=list[3].ToFloat();
	   pObj->m_pSuperObj=GetSuperObject();
   }
   return pObj;
}

XXObjectRectangle* XXObjectRectangle::Clone()
{
	XXObjectRectangle*pObj=new XXObjectRectangle(m_pRoot);
	if(pObj)
	{
		pObj->SetSuper(m_bBase?this:m_pSuper,XFALSE);
		pObj->m_pSuperObj=m_pSuperObj;
		pObj->left=left;
		pObj->width=width;
		pObj->top=top;
		pObj->height=height;
	}
	return pObj;
}


XBOOL XXObjectRectangle::SysGetMember(int id, XSWFCONTEXT *pCnt,XXVar &var)
{
	switch(id)
	{
	case _SYSID(bottom):
		 var=top+height;return XTRUE;
	case _SYSID(bottomRight):
		{
			 XXObjectPoint*pt=m_pRoot->m_pGlobal->CreatePoint();
			if(pt)
			{
				pt->xPos=left+width;
				pt->yPos=top+height;
				var=pt;
			}
		}
		 return XTRUE;
	case _SYSID(height):
		 var=height;return XTRUE;
	case _SYSID(left):
		 var=left;return XTRUE;
	case _SYSID(right):
		 var=left+width;return XTRUE;
	case _SYSID(size):
		{
			XXObjectPoint*pt=m_pRoot->m_pGlobal->CreatePoint();
			if(pt)
			{
				pt->xPos=width;
				pt->yPos=height;
				var=pt;
			}
		}return XTRUE;
	case _SYSID(top):  
		var=top;return XTRUE;
	case _SYSID(topLeft):
		{
			XXObjectPoint*pt=m_pRoot->m_pGlobal->CreatePoint();
			if(pt)
			{
				pt->xPos=left;
				pt->yPos=top;
				var=pt;
			}
		}return XTRUE;
	case _SYSID(width):
		var=width;return XTRUE;
	case _SYSID(x):
		var=left;return XTRUE;
	case _SYSID(y):
		var=top;return XTRUE;
	case _SYSID(clone):
	case _SYSID(contains)://CONTAINS);
	case _SYSID(containsPoint):
	case _SYSID(containsRectangle):
	case _SYSID(equals):
	case _SYSID(inflate):
	case _SYSID(inflatePoint):
	case _SYSID(intersection):
	case _SYSID(intersects):
	case _SYSID(isEmpty):
	case _SYSID(offset):
	case _SYSID(offsetPoint):
	case _SYSID(setEmpty):
	case _SYSID(union):
		 var=this;
		 var.ToFunction(id);
		 return XTRUE;
	}
    return XXObject::SysGetMember(id,pCnt,var);;
}
XBOOL XXObjectRectangle::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{
	switch(id)
	{
	case _SYSID(clone):
		 pCnt->pStack->Push(Clone());
		 return XTRUE;
	case _SYSID(contains):
		 contains(*pCnt,list);
		 return XTRUE;
	case _SYSID(containsPoint):
		 containsPoint(*pCnt,list);
		 return XTRUE;
	case _SYSID(containsRectangle):
		 containsRectangle(*pCnt,list);
		 return XTRUE;
	case _SYSID(equals):
		 equalsRect(*pCnt,list);
		 return XTRUE;
	case _SYSID(inflate):
		 inflate(*pCnt,list);
		 return XTRUE;
	case _SYSID(inflatePoint):
		 inflatePoint(*pCnt,list);
		 return XTRUE;
	case _SYSID(intersection):
		 intersection(*pCnt,list);
		 return XTRUE;
	case _SYSID(intersects):
		 intersects(*pCnt,list);return XTRUE;
	case _SYSID(isEmpty):
		 pCnt->pStack->PushBool((width==0&&height==0));
		 return XTRUE;
	case _SYSID(offset):
		 offsetRect(*pCnt,list);
		 return XTRUE;
	case _SYSID(offsetPoint):
		 offsetPoint(*pCnt,list);
		 return XTRUE;
	case _SYSID(setEmpty):
		 left=top=0;
		 width=height=0;
		 pCnt->pStack->PushConst(XOCT_UNDEFINE);
		 return XTRUE;
	case _SYSID(toString):
		{
			XXVar var;
			GetString(var);
			pCnt->pStack->Push(var);
		}return XTRUE;
	case _SYSID(union):
		unionRect(*pCnt,list);
		return XTRUE;
	}
	return XXObject::SysCallMethod(id,pCnt,list);
}
XBOOL XXObjectRectangle::SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	switch(id)
	{
	case _SYSID(bottom):
		 height=var.ToFloat()-top;
		 return XTRUE;
	case _SYSID(bottomRight):
		 if(var.IsObject(XXOBJ_POINT))
			{
			    XXObjectPoint*pt=(XXObjectPoint*)var.pObject;//m_pRoot->m_pGlobal->CreatePoint();
				if(pt)
				{
					width=pt->xPos-left;
					height=pt->yPos-top;
				}
			}
		 return XTRUE;
	case _SYSID(height):
		 height=var.ToFloat();return XTRUE;
	case _SYSID(left):
		 left=var.ToFloat();return XTRUE;
	case _SYSID(right):
		 width=var.ToFloat()-left;return XTRUE;
	case _SYSID(size):
		 if(var.IsObject(XXOBJ_POINT))
		{
			XXObjectPoint*pt=(XXObjectPoint*)var.pObject;
			if(pt)
			{
				width=pt->xPos;
				height=pt->yPos;
			}
		}return XTRUE;
	case _SYSID(top):
		top=var.ToFloat();return XTRUE;
	case _SYSID(topLeft):
		if(var.IsObject(XXOBJ_POINT))
		{
			XXObjectPoint*pt=(XXObjectPoint*)var.pObject;//m_pRoot->m_pGlobal->CreatePoint();
			if(pt)
			{
				left=pt->xPos;
				top=pt->yPos;
			}
		}return XTRUE;
	case _SYSID(width):
		width=var.ToFloat();return XTRUE;
	case _SYSID(x):
		left=var.ToFloat();return XTRUE;
	case _SYSID(y):
		top=var.ToFloat();return XTRUE;
	}
	return XXObject::SysSetMember(id,pCnt,var);
}


void XXObjectRectangle::InitObject( )
{
	_SYSCALL(clone);
	_SYSCALL(contains);//CONTAINS);
	_SYSCALL(containsPoint);
	_SYSCALL(containsRectangle);
	_SYSCALL(equals);
	_SYSCALL(inflate);
	_SYSCALL(inflatePoint);
	_SYSCALL(intersection);
	_SYSCALL(intersects);
	_SYSCALL(isEmpty);
	_SYSCALL(offset);
	_SYSCALL(offsetPoint);
	_SYSCALL(setEmpty);
	_SYSCALL(union);
	_SYSCALL(bottom);
	_SYSCALL(bottomRight);
	_SYSCALL(height);
	_SYSCALL(left);
	_SYSCALL(right);
	_SYSCALL(size);
	_SYSCALL(top);
	_SYSCALL(topLeft);
	_SYSCALL(width);
	_SYSCALL(x);
	_SYSCALL(y);
	/*AddMember(XXVar(_strClone),XXVar(XXFUNOBJ(cloneRect)));
	AddMember(XXVar(_strContains),XXVar(XXFUNOBJ(contains)));
	AddMember(XXVar(_strContainsPoint),XXVar(XXFUNOBJ(containsPoint)));
	AddMember(XXVar(_strContainsRectangle),XXVar(XXFUNOBJ(containsRectangle)));
	AddMember(XXVar(_strEquals),XXVar(XXFUNOBJ(equalsRect)));
	AddMember(XXVar(_strInflate),XXVar(XXFUNOBJ(inflate)));
	AddMember(XXVar(_strInflatePoint),XXVar(XXFUNOBJ(inflatePoint)));
	AddMember(XXVar(_strIntersection),XXVar(XXFUNOBJ(intersection)));
	AddMember(XXVar(_strIntersects),XXVar(XXFUNOBJ(intersects)));
	AddMember(XXVar(_strIsEmpty),XXVar(XXFUNOBJ(isEmpty)));
	AddMember(XXVar(_strOffset),XXVar(XXFUNOBJ(offsetRect)));
	AddMember(XXVar(_strOffsetPoint),XXVar(XXFUNOBJ(offsetPoint)));
	AddMember(XXVar(_strSetEmpty),XXVar(XXFUNOBJ(setEmpty)));
	AddMember(XXVar(_strUnion),XXVar(XXFUNOBJ(unionRect)));

	AddProperty(XXVar(_strBottom),XXFUNOBJ(bottomGet),XXFUNOBJ(bottomSet));
	AddProperty(XXVar(_strBottomRight),XXFUNOBJ(bottomRightGet),XXFUNOBJ(bottomRightSet));
	AddProperty(XXVar(_strHeight),XXFUNOBJ(heightGet),XXFUNOBJ(heightSet));
	AddProperty(XXVar(_strLeft),XXFUNOBJ(leftGet),XXFUNOBJ(leftSet));
	AddProperty(XXVar(_strRight),XXFUNOBJ(rightGet),XXFUNOBJ(rightSet));
	AddProperty(XXVar(_strSize),XXFUNOBJ(sizeGet),XXFUNOBJ(sizeSet));
	AddProperty(XXVar(_strTop),XXFUNOBJ(topGet),XXFUNOBJ(topSet));
	AddProperty(XXVar(_strTopLeft),XXFUNOBJ(topLeftGet),XXFUNOBJ(topLeftSet));
	AddProperty(XXVar(_strWidth),XXFUNOBJ(widthGet),XXFUNOBJ(widthSet));
	AddProperty(XXVar(_strXX),XXFUNOBJ(leftGet),XXFUNOBJ(leftSet));
	AddProperty(XXVar(_strYY),XXFUNOBJ(topGet),XXFUNOBJ(topSet));*/
}


void XXObjectRectangle::cloneRect(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	cnt.pStack->Push((Clone()));
}

void XXObjectRectangle::contains(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XXVar*pVar=XXVar::CreateBool(XFALSE);
	XXVar var;
	var.ToLogic();
	if(list.GetSize()>=2)
	{
		double x=list[0].ToFloat();
		double y=list[1].ToFloat();
		if(x>=left&&y>=top&&
		   x<=left+width&&y<=top+height)
		   var.iData32=XTRUE;
	}
	cnt.pStack->Push(var);
}
void XXObjectRectangle::containsPoint(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	XXVar pVar;//*pVar=XXVar::CreateBool(XFALSE);
	pVar.ToLogic();
	if(list.GetSize()>=1&&list[0].IsObject())
	{
		XXObjectPoint*pt=(XXObjectPoint*)list[0].pObject;
		if(pt->IsObject(XXOBJ_POINT))
		{
			double x=pt->xPos;
			double y=pt->yPos;
			if(x>=left&&y>=top&&
			 x<=left+width&&y<=top+height)
			pVar.iData32=XTRUE;
		}
	}
	cnt.pStack->Push(pVar);
}
void XXObjectRectangle::containsRectangle(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	XXVar pVar;//=XXVar::CreateBool(XFALSE);
	pVar.ToLogic();
	if(list.GetSize()>=1&&list[0].IsObject())
	{
		XXObjectRectangle*p=(XXObjectRectangle*)list[0].pObject;
		if(p->IsObject(XXOBJ_RECTANGLE))
		{
			double right=left+width;
			double bottom=top+height;
			double r1=p->left+p->width;
			double b1=p->top+p->height;
			if(p->left>=left&&p->left<=right&&
			   p->top>=top&&p->top<=bottom&&
			   r1>=left&&r1<=right&&
			   b1>=top&&b1<=bottom )
			  pVar.iData32=XTRUE;
		}
	}
	cnt.pStack->Push(pVar);
}
void XXObjectRectangle::equalsRect(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	XXVar pVar;//=XXVar::CreateBool(XFALSE);
	pVar.ToLogic();
	if(list.GetSize()>=1&&list[0].IsObject())
	{
		XXObjectRectangle*p=(XXObjectRectangle*)list[0].pObject;
		if(p->IsObject(XXOBJ_RECTANGLE))
		{
			if(p->left==left&&p->top==top&&
			   p->width==width&&p->height==height)
			pVar.iData32=XTRUE;
		}
	}
	cnt.pStack->Push(pVar);
}
void XXObjectRectangle::inflate(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	if(list.GetSize()>0)
	{
		double dx=list[0].ToFloat();
		width+=dx*2;
		left-=dx;
	}
	if(list.GetSize()>1)
	{
		double dy=list[1].ToFloat();
		top-=dy;
		height+=dy*2;
	}
}
void XXObjectRectangle::inflatePoint(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	if(list.GetSize()<=0||!list[0].IsObject()) return;
	XXObjectPoint*pt=(XXObjectPoint*)list[0].pObject;
	if(!pt->IsObject(XXOBJ_POINT)) return;
	left-=pt->xPos;
	width+=pt->xPos*2;
	top-=pt->yPos;
	height+=pt->yPos*2;
}


void XXObjectRectangle::GetRect(XRECT&rect,XU8 bZoom)
{
	double l=left;
	double t=top;
	if(bZoom)
	{
		l=l*ANTISTEP;
		t=t*ANTISTEP;
	}
	rect.left=XROUND(l);
	rect.top=XROUND(t);
	double r=left+width;
	double b=top+height;
	if(bZoom)
	{
		r=r*ANTISTEP;
		t=t*ANTISTEP;
	}
	rect.right=XROUND(r);
	rect.bottom=XROUND(b);
}
void XXObjectRectangle::intersection(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	XXObjectRectangle*pRect=m_pRoot->m_pGlobal->CreateRectangle();
	if(pRect&&list.GetSize()>0&&list[0].IsObject())
	{
		XXObjectRectangle*pr=(XXObjectRectangle*)list[0].pObject;
		if(pr->IsObject(XXOBJ_RECTANGLE))
		{
			double r=left+width;
			double b=top+height;
			double r1=pr->left+pr->width;
			double b1=pr->top+pr->height;
			double l=XMAX(left,pr->left);
			double t=XMAX(top,pr->top);
			r=XMIN(r,r1);
			b=XMIN(b,b1);
			if(r>l&&b>t)
			{
				pRect->left=l;
				pRect->top=t;
				pRect->width=r-l;
				pRect->height=b-t;
			}
		}
	}
	cnt.pStack->Push((pRect));
}
void XXObjectRectangle::intersects(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	XXVar pVar;//=XXVar::CreateBool(XFALSE);
	pVar.ToLogic();
	if(list.GetSize()>0&&list[0].IsObject())
	{
		XXObjectRectangle*pr=(XXObjectRectangle*)list[0].pObject;
		if(pr->IsObject(XXOBJ_RECTANGLE))
		{
		    double r=left+width;
			double b=top+height;
			double r1=pr->left+pr->width;
			double b1=pr->top+pr->height;
			double l=XMAX(left,pr->left);
			double t=XMAX(top,pr->top);
			r=XMIN(r,r1);
			b=XMIN(b,b1);
			if(r>l&&b>t)
			   pVar.iData32=XTRUE;
		}
	}
	cnt.pStack->Push(pVar);
}
void XXObjectRectangle::isEmpty(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	cnt.pStack->PushBool((width==0&&height==0));
}
void XXObjectRectangle::setEmpty(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	left=top=0;
	width=height=0;
}
void XXObjectRectangle::offsetRect(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	if(list.GetSize()>0)
		left+=list[0].ToFloat();
	if(list.GetSize()>1)
		top+=list[1].ToFloat();
}
void XXObjectRectangle::offsetPoint(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	if(list.GetSize()<=0||!list[0].IsObject()) return;
	XXObjectPoint*p=(XXObjectPoint*)list[0].pObject;
	if(!p->IsObject(XXOBJ_POINT)) return;
	left+=p->xPos;
	top+=p->yPos;
}
void XXObjectRectangle::unionRect(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	XXObjectRectangle*pRect=m_pRoot->m_pGlobal->CreateRectangle();
	if(pRect&&list.GetSize()>0&&list[0].IsObject())
	{
		XXObjectRectangle*pr=(XXObjectRectangle*)list[0].pObject;
		if(pr->IsObject(XXOBJ_RECTANGLE))
		{
		    double r=left+width;
			double b=top+height;
			double r1=pr->left+pr->width;
			double b1=pr->top+pr->height;
			double l=XMIN(left,pr->left);
			double t=XMIN(top,pr->top);
			r=XMAX(r,r1);
			b=XMAX(b,b1);
			pRect->left=l;
			pRect->top=t;
			pRect->width=r-l;
			pRect->height=b-t;
			//if(r>l&&b>t)
			//   pVar->iData32=XTRUE;
		}
	}
	cnt.pStack->Push((pRect));
}

void XXObjectRectangle::GetString(XXVar&var)//XString8 &str)
{
	var.SetString("(x=",3);
	XString8 tmp;
	tmp.FromFloat(left);
	var.StringAdd(tmp);
	var.StringAdd(",y=");
	tmp.FromFloat(top);
	var.StringAdd(tmp);
	var.StringAdd(",w=");
	tmp.FromFloat(width);
	var.StringAdd(tmp);
	var.StringAdd(",h=");
	tmp.FromFloat(height);
	var.StringAdd(tmp);
	var.AddChar(')');
	//str+=(")");
	//return str.GetLength();
}
