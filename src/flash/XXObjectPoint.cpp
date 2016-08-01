// XXObjectPoint.cpp: implementation of the XXObjectPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XXObjectPoint.h"
#include "splayer.h"
//#include <Math.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XXObjectPoint::XXObjectPoint(ScriptPlayer*pRoot):XXObject(pRoot)
{
	//m_point=XPoint(0,0);
	xPos=0;
	yPos=0;
}

XXObjectPoint::~XXObjectPoint()
{

}


XBOOL XXObjectPoint::SysGetMember(int id, XSWFCONTEXT *pCnt, XXVar &var)
{
	switch(id)
	{
	case _SYSID(x):
		 var=xPos;return XTRUE;
	case _SYSID(y):
		 var=yPos;return XTRUE;
	case _SYSID(length):
		{SPOINT pt;
		pt.x=DoubleToFixed(xPos);
		pt.y=DoubleToFixed(yPos);
		SFIXED l=::PointLength(&pt);
		var=((FixedToDouble(l)));
		}return XTRUE;
		_SYSCALL(add);
	case _SYSID(clone):
	case _SYSID(distance):
	case _SYSID(equals):
	case _SYSID(interpolate):
	case _SYSID(normalize):
	case _SYSID(offset):
	case _SYSID(polar):
	case _SYSID(subtract):
		 var=this;
		 var.ToFunction(id);
		 return XTRUE;
	}
    return XXObject::SysGetMember(id,pCnt,var);;
}
XBOOL XXObjectPoint::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{				
	switch(id)
	{
	case _SYSID(add):
		 addPT(*pCnt,list);return XTRUE;
	case _SYSID(clone):
		 pCnt->pStack->Push(Clone());return XTRUE;
	case _SYSID(distance):
		 distancePT(*pCnt,list);return XTRUE;
	case _SYSID(equals):
		 equalsPT(*pCnt,list);return XTRUE;
	case _SYSID(interpolate):
		 interpolate(*pCnt,list);return XTRUE;
	case _SYSID(normalize):
		 //normalizePT(*pCnt,list);
		 //return XTRUE;
		 if(list.GetSize())
			 lengthSet(*pCnt,list[0].ToFloat());
		 else
			 lengthSet(*pCnt,1);
		 break;
	case _SYSID(offset):
		 offsetPT(*pCnt,list);return XTRUE;
	case _SYSID(polar):
		 polar(*pCnt,list);return XTRUE;
	case _SYSID(subtract):
		 subtractPT(*pCnt,list);return XTRUE;
	}
	return XXObject::SysCallMethod(id,pCnt,list);
}
XBOOL XXObjectPoint::SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	switch(id)
	{
	case _SYSID(x):
		 xPos=var.ToFloat();return XTRUE;
	case _SYSID(y):
		 yPos=var.ToFloat();return XTRUE;
	case _SYSID(length):
		 lengthSet(*pCnt,var.ToFloat());
		 
	}
	return XXObject::SysSetMember(id,pCnt,var);
}

void XXObjectPoint::InitObject( )
{
	_SYSCALL(x);
	_SYSCALL(y);
	_SYSCALL(length);

	_SYSCALL(add);
	_SYSCALL(clone);
	_SYSCALL(distance);
	_SYSCALL(equals);
	_SYSCALL(interpolate);
	_SYSCALL(normalize);
	_SYSCALL(offset);
	_SYSCALL(polar);
	_SYSCALL(subtract);
	/*AddProperty(XXVar(_strXX),XXFUNOBJ(xGetPT),XXFUNOBJ(xSetPT));
	AddProperty(XXVar(_strYY),XXFUNOBJ(yGetPT),XXFUNOBJ(ySetPT));
	AddProperty(XXVar(_strLength),XXFUNOBJ(lengthGet),XXFUNOBJ(lengthSet));

	AddMember(XXVar(_strAdd),XXVar(XXFUNOBJ(addPT)));
	AddMember(XXVar(_strClone),XXVar(XXFUNOBJ(clonePT)));
	AddMember(XXVar(_strDistance),XXVar(XXFUNOBJ(distancePT)));
	AddMember(XXVar(_strEquals),XXVar(XXFUNOBJ(equalsPT)));
	AddMember(XXVar(_strInterpolate),XXVar(XXFUNOBJ(interpolate)));
	AddMember(XXVar(_strNormalize),XXVar(XXFUNOBJ(normalizePT)));
	AddMember(XXVar(_strOffset),XXVar(XXFUNOBJ(offsetPT)));
	AddMember(XXVar(_strPolar),XXVar(XXFUNOBJ(polar)));
	AddMember(XXVar(_strSubtract),XXVar(XXFUNOBJ(subtractPT)));*/
}


void XXObjectPoint::lengthSet(XSWFCONTEXT&cnt,XFDOUBLE nl)
{
	//(list.GetSize()<=0) return;
	//uble nl=list[0].ToFloat();
	if(nl==0)
	{
		xPos=0;
		yPos=0;
	}
	else
	{
		SPOINT pt;
		pt.x=DoubleToFixed(xPos);
		pt.y=DoubleToFixed(yPos);
		SFIXED l=::PointLength(&pt);
		/*float l=xPos*xPos+yPos*yPos;
		if(l==0)
		{
			xPos=nl;
			yPos=0;
		}
		else*/
		if(l==0)
		{
			xPos=nl;
			yPos=0;
		}
		else
		{
			//l=(float)sqrt(l);
			double len=FixedToDouble(l);
			xPos=xPos*nl/len;
			yPos=yPos*nl/len;
		}
	}

	//l=(float)sqrt(l);
}
void XXObjectPoint::clonePT(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	XXObjectPoint*pObj=Clone();
	cnt.pStack->Push((pObj));
}
void XXObjectPoint::addPT(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	XXObjectPoint*pObj=Clone();
	if(pObj)
	{
		if(list.GetSize()>0)
		{
			if(list[0].IsObject()&&list[0].pObject->IsObject(XXOBJ_POINT))
			{
				XXObjectPoint*pt=(XXObjectPoint*)list[0].pObject;
				pObj->xPos+=pt->xPos;
				pObj->yPos+=pt->yPos;
			}
			else
				pObj->xPos+=list[0].ToFloat();
		}
		if(list.GetSize()>1)
			pObj->yPos+=list[1].ToFloat();
	}
	cnt.pStack->Push((pObj));
}
void XXObjectPoint::distancePT(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XXVar*pVar=XXVar::CreateConst(XOCT_UNDEFINE);
	XXVar var;
	var.ToConst(XOCT_UNDEFINE);
	if(list.GetSize()>=2&&
	   list[0].IsObject()&&
	   list[1].IsObject())
	{
		XXObjectPoint*p0=(XXObjectPoint*)list[0].pObject;
		XXObjectPoint*p1=(XXObjectPoint*)list[1].pObject;
		if(p0->IsObject(XXOBJ_POINT)&&
		   p1->IsObject(XXOBJ_POINT))
		{
			SPOINT pt0,pt1;
			pt0.x=DoubleToFixed(p0->xPos);
			pt0.y=DoubleToFixed(p0->yPos);
			pt1.x=DoubleToFixed(p1->xPos);
			pt1.y=DoubleToFixed(p1->yPos);
			int l=::PointDistance(&pt0,&pt1);
			//float dx=p1->xPos-p0->xPos;
			//float dy=p1->yPos-p0->yPos;
			//float l=(float)sqrt(dx*dx+dy*dy);
			var=FixedToDouble(l);
		}
	}
	cnt.pStack->Push(var);
}
void XXObjectPoint::equalsPT(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XXVar*pVar=XXVar::CreateBool(XFALSE);
	XXVar var;
	var.ToLogic();
	if(list.GetSize()>0&&list[0].IsObject())
	{
		XXObjectPoint*pt=(XXObjectPoint*)list[0].pObject;
		if(pt->IsObject(XXOBJ_POINT))
		{
			if(xPos==pt->xPos&&yPos==pt->yPos)
			 var.iData32=XTRUE;
		}
	}
	cnt.pStack->Push(var);
}
void XXObjectPoint::interpolate(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XXVar*pVar=XXVar::CreateConst(XOCT_UNDEFINE);
	XXVar var;
	var.ToConst(XOCT_UNDEFINE);
	if(list.GetSize()>=3&&list[0].IsObject()&&list[1].IsObject())
	{
		XXObjectPoint*p0=(XXObjectPoint*)list[0].pObject;
		XXObjectPoint*p1=(XXObjectPoint*)list[1].pObject;
		if(p0->IsObject(XXOBJ_POINT)&&p1->IsObject(XXOBJ_POINT))
		{
			double f=list[2].ToFloat();
			XXObjectPoint*pt=Clone();
			pt->xPos=p0->xPos+(p1->xPos-p0->xPos)*f;
			pt->yPos=p0->yPos+(p1->yPos-p0->yPos)*f;
			var=pt;
		}
	}
	cnt.pStack->Push(var);
}
void XXObjectPoint::normalizePT(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	if(list.GetSize())
	lengthSet(cnt,list[0].ToFloat());
}
void XXObjectPoint::offsetPT(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	if(list.GetSize()<2) return;
	xPos+=list[0].ToFloat();
	yPos+=list[1].ToFloat();
}
void XXObjectPoint::polar(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	XXObjectPoint*pt=Clone();
	if(list.GetSize()>=2)
	{
		XFDOUBLE l=list[0].ToFloat();
		XFDOUBLE a=list[1].ToFloat();
		int cosv,sinv;
		::CalcRotate(a,sinv,cosv,XFALSE);

		xPos=(XFDOUBLE)(l*cosv/fixed_1);
		yPos=(XFDOUBLE)(l*sinv/fixed_1);
	}
	cnt.pStack->Push((pt));
}
void XXObjectPoint::subtractPT(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	XXObjectPoint*pt=Clone();
	if(list.GetSize()>0&&list[0].IsObject())
	{
	  XXObjectPoint*p=(XXObjectPoint*)list[0].pObject;
	  if(p->IsObject(XXOBJ_POINT))
	  {
		  pt->xPos-=p->xPos;
		  pt->yPos-=p->yPos;
	  }
	}
	cnt.pStack->Push((pt));
}

XXObject* XXObjectPoint::CreateObject(XSWFCONTEXT *pCnt, XXVARLIST &list,XBOOL)
{
    XXObjectPoint*pObj=CreateClass(this,XFALSE,m_pRoot);
	if(pObj)
	{
		pObj->m_pSuperObj=GetSuperObject();
		if(list.GetSize()>0)
			pObj->xPos=list[0].ToFloat();
		if(list.GetSize()>1)
			pObj->yPos=list[1].ToFloat();
	}
	pCnt->pStack->Push((pObj));
	return pObj;
}

void XXObjectPoint::GetString(XXVar&var)//XString8 &str)
{
    var.SetString("(x=",3);
	XString8 tmp;
	tmp.FromFloat(xPos);
	//str+=tmp;
	var.StringAdd(tmp);
	var.StringAdd(_XXKEY(",y="));
	tmp.FromFloat(yPos);
	var.StringAdd(tmp);
	//str+=tmp;
	var.StringAdd(_XXKEY(")"));
	//return str.GetLength();
}

XXObjectPoint* XXObjectPoint::Clone()
{
    XXObjectPoint*pObj=new XXObjectPoint(m_pRoot);
	if(pObj)
	{
		pObj->xPos=xPos;
		pObj->yPos=yPos;
		pObj->SetSuper(m_bBase?this:m_pSuper,XFALSE);
	}
	return pObj;
}
