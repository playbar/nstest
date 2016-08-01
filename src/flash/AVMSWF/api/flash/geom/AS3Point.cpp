#include "StdAfxflash.h"

#include <math.h>
#include "flashgeom.h"
#include "ShellCore.h"

using namespace avmplus;
#include "AS3Point.h"
namespace avmshell{

inline double FastSQRT(double v)
{
#ifdef __FPU__
	return sqrt(v);	
#else
	XS64 i;
    double x2, y;
    const double threehalfs = 1.5F;

    x2 = v * 0.5F;
    y  = v;
    i  = * ( XS64 * ) &y;
    i  = 0x5fe6ec85e7de30da - ( i >> 1 ); 
    y  = * ( double * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) ); // 1st iteration
    y  = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed
    return y;
#endif
}
inline double FastLength(double x,double y)
{
#ifdef __FPU__
	return sqrt(x*x+y*y);
#else
	SPOINT pt={DoubleToFixed(x),DoubleToFixed(y)};
	return FixedToDouble(::PointLength(&pt));
#endif
}

PointClass::PointClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())PointObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};

Atom PointClass::CreatePoint(double x,double y)
{
	//struct
	//{ Atom a;double x,y;}atoms={0,x,y};
	PointObject*pt=(PointObject*)createInstance(ivtable(),prototype);
	pt->constructor(x,y);
	//pt->x=x;
	//pt->y=y;
	//pt->Modify();
	//return pt->atom();
	//return construct(2,(Atom*)&atoms);
	return pt->atom();
}

ScriptObject* PointClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) PointObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
PointObject::PointObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	l=0;
	bModify=true;
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
double PointObject::AS3_distance(PointObject*p1,PointObject*p2)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	double dx=p2->getX()-p1->getX();
	double dy=p2->getY()-p1->getY();
	return FastLength(dx,dy);
}

/*vmBox PointObject::AS3_interpolate(PointObject*p1,PointObject*p2,double f)
{
	//Add your act code here...
	PointObject*pNew=(PointObject*)AvmCore::atomToScriptObject(p1->AS3_clone());
	pNew->m_slots_PointObject.x+=(p2->getX()-p1->getX())*f;
	pNew->m_slots_PointObject.y+=(p2->getY()-p1->getY())*f;
	return pNew->atom();
	//return kAvmThunkUndefined;//Modify this please!
}*/

AvmBox PointClass::AS3_polar(double l,double r)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	int cosv,sinv;
	::CalcRotate(r,sinv,cosv,XFALSE);

	double x=(XFDOUBLE)(l*cosv/fixed_1);
	double y=(XFDOUBLE)(l*sinv/fixed_1);
	return CreatePoint(x,y);
	//return kAvmThunkUndefined;//Modify this please!
}

double PointObject::AS3_length_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(bModify)
	{
		l=FastLength(getX(),getY());//(x*x+y*y);
	}
	return l;
	//return kAvmThunkUndefined;//Modify this please!
}

/*ouble PointObject::AS3_x_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return x;
	//return kAvmThunkUndefined;//Modify this please!
}

void PointObject::AS3_x_set(double v)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	bModify=x!=v;
	x=v;
	//return kAvmThunkUndefined;//Modify this please!
}

double PointObject::AS3_y_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return y;
	//return kAvmThunkUndefined;//Modify this please!
}

void PointObject::AS3_y_set(double v)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	bModify=y!=v;
	y=v;
	//return kAvmThunkUndefined;//Modify this please!
}

//AS3 contructor function..
void PointObject::AS3_constructor(double nx,double ny)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	x=nx;
	y=ny;
	l=0;
	bModify=true;
	//return kAvmThunkUndefined;//Modify this please!
}*/

/*vmBox PointObject::AS3_add(PointObject*pt)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	PointObject*pNew=(PointObject*)AvmCore::atomToScriptObject(AS3_clone());
	pNew->m_slots_PointObject.x+=pt->getX();
	pNew->m_slots_PointObject.y+=pt->getY();
	return pNew->atom();
	//return kAvmThunkUndefined;//Modify this please!
}*/

PointObject* PointObject::clone()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	ShellToplevel*t=(ShellToplevel*)toplevel();
	Atom a=t->getPointClass()->CreatePoint(getX(),getY());
	PointObject*p=(PointObject*)AvmCore::atomToScriptObject(a);
	if(!bModify)
	{
		
		p->l=l;
		p->bModify=bModify;
	}
	return p;
	//return kAvmThunkUndefined;//Modify this please!
}

/*bool PointObject::AS3_equals(PointObject*pt)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	return m_slots_PointObject.x==pt->m_slots_PointObject.x&&
		   m_slots_PointObject.y==pt->m_slots_PointObject.y;
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
}*/

void PointObject::AS3_normalize(double t)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(!t)
	{
		m_slots_PointObject.x=0;
		m_slots_PointObject.y=0;
		l=0;
		bModify=false;
	}
	else
	{
		if(bModify) l=FastLength(m_slots_PointObject.x,m_slots_PointObject.y);
		if(l==0)
		{
			m_slots_PointObject.x=t;
			m_slots_PointObject.y=0;
		}
		else
		{
			m_slots_PointObject.x=t*m_slots_PointObject.x/l;
			m_slots_PointObject.y=t*m_slots_PointObject.y/l;
		}
		l=t;
		bModify=false;
	}
	//return kAvmThunkUndefined;//Modify this please!
}

/*void PointObject::AS3_offset(double dx,double dy)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	m_slots_PointObject.x+=dx;
	m_slots_PointObject.y+=dy;
	//return kAvmThunkUndefined;//Modify this please!
}

AvmBox PointObject::AS3_subtract(PointObject*pt)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	PointObject*pNew=(PointObject*)AvmCore::atomToScriptObject(AS3_clone());
	pNew->x-=pt->x;
	pNew->y-=pt->y;
	return pNew->atom();
	//return kAvmThunkUndefined;//Modify this please!
}*/

Stringp PointObject::AS3_toString()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	ShellCore* c=(ShellCore*)core();
	Stringp str=ScriptObject::formatString(c->kSpString,c->kEqualString,c->kQuotString,true,true,true);
	str=str->concatStrings(c->newConstantStringLatin1("("),str);
	str=str->concatStrings(str,c->newConstantStringLatin1(")"));
	return str;
	//return kAvmThunkUndefined;//Modify this please!
}

//new adds 11
void PointObject::AS3_copyFrom(PointObject* sourcePoint)
{
	if( sourcePoint == this ) return;
	avmplus::NativeID::PointObjectSlots& source = GetData();
	m_slots_PointObject.x = source.x;
	m_slots_PointObject.y = source.y;
	bModify = true;
	//LOGWHERE();
}
void PointObject::AS3_setTo(double xa, double ya)
{	
	m_slots_PointObject.x = xa;
	m_slots_PointObject.y = ya;
	bModify = true;
	//LOGWHERE();
}
//new adds 11 end

}
