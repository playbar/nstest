// XXObjectMatrix.cpp: implementation of the XXObjectMatrix class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XXObjectMatrix.h"
#include "splayer.h"
#include "ObjectCreator.h"
//#include "XPaintDib.h"
#include "XXObjectPoint.h"
//#include <Math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XXObjectMatrix::XXObjectMatrix(ScriptPlayer*pRoot):XXObject(pRoot)
{
	//::MatrixIdentity(&matrix);
}

XXObjectMatrix::~XXObjectMatrix()
{

}

#define translateY ty
#define translateX tx
#define scaleX		a
#define scaleY		d
#define rotateSkew0	b
#define rotateSkew1	c

XXObject* XXObjectMatrix::CreateObject(XSWFCONTEXT*pCnt, XXVARLIST &list,XBOOL)
{
	XXObjectMatrix*p=CreateClass(this,XFALSE,m_pRoot);
	if(p)
	{
		p->m_pSuperObj=GetSuperObject();
	    p->Construct(pCnt,list,this);
	}
	else 
		pCnt->pStack->Push(p);//new XXVar(p));
	return  p;
}


XFDOUBLE XXObjectMatrix::GetIndex(int id)
{
	switch(id)
	{
	default:
		return 0;
	case 0:
		return (XFDOUBLE)FixedToDouble(matrix.scaleX);///FIX16ONE;
			//XSWFMatrix::FixToFloat(matrix.scaleX);
	case 1:
		return (XFDOUBLE)FixedToDouble(matrix.rotateSkew0);///FIX16ONE;
			//XSWFMatrix::FixToFloat(matrix.rotateSkew0);
	case 2:
		return (XFDOUBLE)FixedToDouble(matrix.rotateSkew1);///FIX16ONE;
		//return XSWFMatrix::FixToFloat(matrix.rotateSkew1);
	case 3:
		return (XFDOUBLE)FixedToDouble(matrix.scaleY);///FIX16ONE;
		//return XSWFMatrix::FixToFloat(matrix.scaleY);
	case 4:
		return (XFDOUBLE)matrix.translateX/XSWFTRIP;
	case 5:
		return (XFDOUBLE)matrix.translateY/XSWFTRIP;
	}
}

void XXObjectMatrix::GetString(XXVar&var)//XString8 &str)
{
    //str+=_XXKEY("(");
	var.SetString("(",1);
	XString8 tmp;
	for(int i=0;i<6;i++)
	{
		if(i)
			var.StringAdd(",");
			//str+=_XXKEY(",");
		var.StringAdd(_strMatrixIndex[i]);
		var.StringAdd(_XXKEY("="));
		tmp.FromFloat(GetIndex(i));
		var.StringAdd(tmp);
	}
	var.StringAdd(_XXKEY(")"));
	//return var.GetLength();

}


XBOOL XXObjectMatrix::SysGetMember(int id, XSWFCONTEXT *pCnt, XXVar &var)
{
	switch(id)
	{
	case _SYSID(a):
		var=FixedToDouble(matrix.a);return XTRUE;
	case _SYSID(b):
		var=FixedToDouble(matrix.b);return XTRUE;
	case _SYSID(c):
		var=FixedToDouble(matrix.c);return XTRUE;
	case _SYSID(d):
		var=FixedToDouble(matrix.d);return XTRUE;
	case _SYSID(tx):
		var=FixedToDouble(matrix.tx);return XTRUE;
	case _SYSID(ty):
		var=FixedToDouble(matrix.ty);
		return XTRUE;
	case _SYSID(clone):
	case _SYSID(concat):
	case _SYSID(createBox):
	case _SYSID(createGradientBox):
	case _SYSID(deltaTransformPoint):
	case _SYSID(invert):
	case _SYSID(rotate):
	case _SYSID(scale):
	case _SYSID(identity):
	case _SYSID(transformPoint):
	case _SYSID(translate):
		var=this;
		var.ToFunction(id);
		return XTRUE;
	}
    return XXObject::SysGetMember(id,pCnt,var);;
}
XBOOL XXObjectMatrix::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{
	switch(id)
	{
	case _SYSID(clone):
		clone(*pCnt,list);
		return XTRUE;
	case _SYSID(concat):
		concat(*pCnt,list);
		return XTRUE;
	case _SYSID(createBox):
		CreateBox(list,XTRUE);
		 return XTRUE;
	case _SYSID(createGradientBox):
		CreateBox(list,XFALSE);
		return XTRUE;
	case _SYSID(deltaTransformPoint):
		deltaTransformPoint(*pCnt,list);
		return XTRUE;
	case _SYSID(invert):
		//invert(*pCnt,list);
		::MatrixInvert(&matrix,&matrix);
		return XTRUE;
	case _SYSID(rotate):
		rotate(*pCnt,list);
		return XTRUE;
	case _SYSID(scale):
		scale(*pCnt,list);
		return XTRUE;
	case _SYSID(transformPoint):
		transformPoint(*pCnt,list);
		return XTRUE;
	case _SYSID(translate):
		translate(*pCnt,list);
		return XTRUE;
	case _SYSID(identity):
		::MatrixIdentity(&matrix);
		return XTRUE;
	}
	return XXObject::SysCallMethod(id,pCnt,list);
}
XBOOL XXObjectMatrix::SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	switch(id)
	{case _SYSID(a):
		matrix.a=DoubleToFixed(var.ToFloat());return XTRUE;
	case _SYSID(b):
		matrix.b=DoubleToFixed(var.ToFloat());return XTRUE;
	case _SYSID(c):
		matrix.c=DoubleToFixed(var.ToFloat());return XTRUE;
		
	case _SYSID(d):
		matrix.d=DoubleToFixed(var.ToFloat());return XTRUE;
	case _SYSID(tx):
		matrix.tx=DoubleToFixed(var.ToFloat());return XTRUE;
	case _SYSID(ty):
		matrix.ty=DoubleToFixed(var.ToFloat());return XTRUE;
		return XTRUE;
	}
	return XXObject::SysSetMember(id,pCnt,var);
}

void XXObjectMatrix::InitObject( )
{
	_SYSCALL(a);
	_SYSCALL(b);
	_SYSCALL(c);
	_SYSCALL(d);
	_SYSCALL(tx);
	_SYSCALL(ty);
	_SYSCALL(createBox);
	_SYSCALL(createGradientBox);
	_SYSCALL(deltaTransformPoint);
	_SYSCALL(invert);
	_SYSCALL(rotate);
	_SYSCALL(scale);
	_SYSCALL(transformPoint);
	_SYSCALL(translate);
	_SYSCALL(identity);
	/*AddProperty(XXVar(_strA),XXFUNOBJ(aGet),XXFUNOBJ(aSet));
	AddProperty(XXVar(_strB),XXFUNOBJ(bGet),XXFUNOBJ(bSet));
	AddProperty(XXVar(_strC),XXFUNOBJ(cGet),XXFUNOBJ(cSet));
	AddProperty(XXVar(_strD),XXFUNOBJ(dGet),XXFUNOBJ(dSet));
	AddProperty(XXVar(_strTx),XXFUNOBJ(txGet),XXFUNOBJ(txSet));
	AddProperty(XXVar(_strTy),XXFUNOBJ(tyGet),XXFUNOBJ(tySet));

	AddMember(XXVar(_strCreateBox),XXVar(XXFUNOBJ(createBox)));
	AddMember(XXVar(_strCreateGradientBox),XXVar(XXFUNOBJ(createGradientBox)));
	AddMember(XXVar(_strDeltaTransformPoint),XXVar(XXFUNOBJ(deltaTransformPoint)));
	AddMember(XXVar(_strInvert),XXVar(XXFUNOBJ(invert)));
	AddMember(XXVar(_strRotates),XXVar(XXFUNOBJ(rotate)));
	AddMember(XXVar(_strScale),XXVar(XXFUNOBJ(scale)));
	AddMember(XXVar(_strTransformPoint),XXVar(XXFUNOBJ(transformPoint)));
	AddMember(XXVar(_strTranslate),XXVar(XXFUNOBJ(translate)));*/
}


void XXObjectMatrix::clone(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	XXObjectMatrix*p=m_pRoot->m_pGlobal->CreateMatrix();
	if(p)
		p->matrix=matrix;
	cnt.pStack->Push(p);//new XXVar(p));
}
void XXObjectMatrix::concat(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	if(list.GetSize()<=0||!list[0].IsObject()) return;
	XXObjectMatrix*p=(XXObjectMatrix*)list[0].pObject;
	if(!p->IsObject(XXOBJ_MATRIX)) return;
	//matrix.MultiMatrix(p->matrix);
	::MatrixConcat(&matrix,&p->matrix,&matrix);
}
void XXObjectMatrix::createBox(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	CreateBox(list,XTRUE);
}
void XXObjectMatrix::createGradientBox(XSWFCONTEXT&cnt,XXVARLIST&list)
{
    CreateBox(list,XFALSE);
}
void XXObjectMatrix::deltaTransformPoint(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	XXObjectPoint*p=m_pRoot->m_pGlobal->CreatePoint();
	if(p&&list.GetSize()>0&&list[0].IsObject())
	{
		XXObjectPoint*ps=(XXObjectPoint*)list[0].pObject;
		if(ps->IsObject(XXOBJ_POINT))
		{
			XFDOUBLE x=ps->xPos,y=ps->yPos;
			p->xPos=(x*matrix.scaleX+y*matrix.rotateSkew1)/fixed_1;
			p->yPos=(x*matrix.rotateSkew0+y*matrix.scaleY)/fixed_1;
			//(float)y/XSWFTRIP;
		}
	}
	cnt.pStack->Push(p);//new XXVar(p));
}
void XXObjectMatrix::identity(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	::MatrixIdentity(&matrix);
	//matrix.Identity();
}
void XXObjectMatrix::invert(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	::MatrixInvert(&matrix,&matrix);
	//matrix.Invert();
}
void XXObjectMatrix::rotate(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	if(list.GetSize()<=0) return;
	XFDOUBLE rf=list[0].ToFloat();//*0x1ff/_floatPI;
	//int r=XROUND(rf);
	//matrix.Rotate(r);
	::MatrixRotate(rf,&matrix);
}
void XXObjectMatrix::scale(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	if(list.GetSize()<2) return;
	/*float sx=list[0].ToFloat()*FIX16ONE;
	float sy=list[1].ToFloat()*FIX16ONE;
	int ssx=XROUND(sx);
	int ssy=XROUND(sy);
	matrix.Scale(ssx,ssy);*/
	::MatrixScale(DoubleToFixed(list[0].ToFloat()),
				  DoubleToFixed(list[1].ToFloat()),
				  &matrix);
}
void XXObjectMatrix::transformPoint(XSWFCONTEXT&cnt,XXVARLIST&list)
{
    XXObjectPoint*p=m_pRoot->m_pGlobal->CreatePoint();
	if(p&&list.GetSize()>0&&list[0].IsObject())
	{
		XXObjectPoint*ps=(XXObjectPoint*)list[0].pObject;
		if(ps->IsObject(XXOBJ_POINT))
		{
			XFDOUBLE x=ps->xPos*XSWFTRIP,y=ps->yPos*XSWFTRIP;
			//int ix=XROUND(x);
			//int iy=XROUND(y);
			SPOINT pt;
			pt.x=XROUND(x);
			pt.y=XROUND(y);
			//matrix.Translate(ix,iy);
			::MatrixTransformPoint(&matrix,&pt,&pt);
			p->xPos=(XFDOUBLE)pt.x/XSWFTRIP;
			p->yPos=(XFDOUBLE)pt.y/XSWFTRIP;
			//p->xPos=(x*matrix.scaleX+y*matrix.rotateSkew1)/FIX16ONE;
			//p->yPos=(x*matrix.rotateSkew0+y*matrix.scaleY)/FIX16ONE;
			//(float)y/XSWFTRIP;
		}
	}
	cnt.pStack->Push(p);//new XXVar(p));
}
void XXObjectMatrix::translate(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XXObjectPoint*p=m_pRoot->m_pGlobal->CreatePoint();
	int nSize=list.GetSize();
	switch(nSize)
	{
	case 2:
		 matrix.translateY+=list[1].ToInt()*XSWFTRIP;
	case 1:
		 matrix.translateX+=list[0].ToInt()*XSWFTRIP;
	}
}

#define FIX16DIV 8

void XXObjectMatrix::CreateBox(XXVARLIST &list, XU8 bBox)
{
	int nSize=list.GetSize();
	if(nSize<2) return;
	int sx,sy;
	if(!bBox)
	{
		XFDOUBLE x=list[0].ToFloat()*XSWFTRIP/FIX16DIV;
		XFDOUBLE y=list[1].ToFloat()*XSWFTRIP/FIX16DIV;
		sx=XROUND(x);
		sy=XROUND(y);
	}
	else
	{
		XFDOUBLE x=list[0].ToFloat()*fixed_1;
		XFDOUBLE y=list[1].ToFloat()*fixed_1;
		sx=XROUND(x);
		sy=XROUND(y);
	}
	int r0=0;
	int r1=0;
	int off=FIX16OFF+2;
	switch(nSize)
	{
	case 5:
		if(bBox)
			matrix.translateY=list[4].ToInt()*XSWFTRIP;
		else
			matrix.translateY=(list[4].ToInt()+(list[1].ToInt()>>1))*XSWFTRIP;
	case 4:
		if(bBox)
			matrix.translateX=list[3].ToInt()*XSWFTRIP;
		else
			matrix.translateX=(list[3].ToInt()+(list[0].ToInt()>>1))*XSWFTRIP;
	case 3:
		{
		  XFDOUBLE rf=list[2].ToFloat();//*0x1ff/_floatPI;
		  //int r=XROUND(rf);
		  int sinv,cosv;
		  CalcRotate(rf,sinv,cosv,XFALSE);
		  //XFDOUBLE sinv=sin(rf),
		  //		 cosv=cos(rf);

		  //XPaintDib::CalcRotate(r,sinv,cosv,XFALSE);
		  r0=FixedMul(sy,sinv);
			  //(sy*sinv);
		  r1=-FixedMul(sx,sinv);
			  //(-sx*sinv);
		  sx=-FixedMul(sx,cosv);//(sx*cosv);
		  sy=FixedMul(sy,cosv);//(sy*cosv);		  
		  //matrix.rotateSkew0=
		};
	}
	//matrix.hasRotate=matrix.hasScale=XTRUE;
	matrix.scaleX=sx;
	matrix.scaleY=sy;
	matrix.rotateSkew0=r0;
	matrix.rotateSkew1=r1;
}

XU32 XXObjectMatrix::Construct(XSWFCONTEXT *pCnt, XXVARLIST &list, XXObject*)
{
	XFDOUBLE v;
	int nSize=list.GetSize();
		switch(nSize)
		{
		case 6:
			//p->ty=list[5].ToFloat();
			matrix.translateY=(list[5].ToInt()*XSWFTRIP);
		case 5:
			//tx=list[4].ToFloat();
			matrix.translateX=(list[4].ToInt()*XSWFTRIP);
		case 4:
			//d=list[3].ToFloat();
			v=list[3].ToFloat();//*FIX16ONE;
			matrix.scaleY=DoubleToFixed(v);//list[3]XROUND(v);
			//matrix.hasScale=XTRUE;
		case 3:
			//c=list[2].ToFloat();
			v=list[2].ToFloat();//*FIX16ONE;
			matrix.rotateSkew1=DoubleToFixed(v);//XROUND(v);
			//matrix.hasRotate=XTRUE;
		case 2:
			//b=list[1].ToFloat();
			//matrix.hasRotate=XTRUE;
			v=list[1].ToFloat();//*FIX16ONE;
			matrix.rotateSkew0=DoubleToFixed(v);//XROUND(v);
		case 1:
			//a=list[1].ToFloat();
			//matrix.hasScale=XTRUE;
			v=list[0].ToFloat();//*FIX16ONE;
			matrix.scaleX=DoubleToFixed(v);//XROUND(v);
			break;
		}
	pCnt->pStack->Push(this);
	return (XU32)this;
}
