#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "flashgeom.h"
#include "AS3Matrix.h"


namespace avmshell{
MatrixClass::MatrixClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())MatrixObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* MatrixClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) MatrixObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

MatrixObject* MatrixClass::CreateMatrix(void*m)
{
	MatrixObject*pObj=(MatrixObject*)createInstance(ivtable(),prototype);
	pObj->SetMatrix(m);
	return pObj;
}

void MatrixObject::SetMatrix(void*m)
{
	register MATRIX&mat=*(MATRIX*)m;
	m_slots_MatrixObject.a=FixedToDouble(mat.a);
	m_slots_MatrixObject.b=FixedToDouble(mat.b);
	m_slots_MatrixObject.c=FixedToDouble(mat.c);
	m_slots_MatrixObject.d=FixedToDouble(mat.d);
	m_slots_MatrixObject.tx=(mat.tx/20.0F);
	m_slots_MatrixObject.ty=(mat.ty/20.0F);
}

void MatrixObject::GetMatrix(void*m,bool bTrip)
{
	register MATRIX&mat=*(MATRIX*)m;
	mat.a=DoubleToFixed(m_slots_MatrixObject.a);
	mat.b=DoubleToFixed(m_slots_MatrixObject.b);
	mat.c=DoubleToFixed(m_slots_MatrixObject.c);
	mat.d=DoubleToFixed(m_slots_MatrixObject.d);
	if(bTrip)
	{
		mat.tx=(int)(m_slots_MatrixObject.tx*20+0.5);
		mat.ty=(int)(m_slots_MatrixObject.ty*20+0.5);
	}
	else
	{
		mat.tx=(int)(m_slots_MatrixObject.tx);
		mat.ty=(int)(m_slots_MatrixObject.ty);
	}
}
MatrixObject::MatrixObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{	
	//Add your construct code here...
}


void MatrixObject::AS3_invert()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	/*MATRIX mat={DoubleToFixed(m_slots_MatrixObject.a),
				DoubleToFixed(m_slots_MatrixObject.b),
				DoubleToFixed(m_slots_MatrixObject.c),
				DoubleToFixed(m_slots_MatrixObject.d),
				(m_slots_MatrixObject.tx*20),
				(m_slots_MatrixObject.ty*20)};
	::MatrixInvert(&mat,&mat);
	m_slots_MatrixObject.a=FixedToDouble(mat.a);
	m_slots_MatrixObject.b=FixedToDouble(mat.b);
	m_slots_MatrixObject.c=FixedToDouble(mat.c);
	m_slots_MatrixObject.d=FixedToDouble(mat.d);
	m_slots_MatrixObject.tx=FixedToDouble(mat.tx/20);
	m_slots_MatrixObject.ty=FixedToDouble(mat.ty/20);*/
	avmplus::NativeID::MatrixObjectSlots&m=m_slots_MatrixObject;
	//float div=((scaleX*scaleY)-rotateSkew1*rotateSkew0);
	double div=m.a*m.d-m.c*m.d;
	if(div==0) return;

	double sx=m.d/div;//(scaleY)/div;
	double sy=m.a/div;//(scaleX)/div;
	double r0=-m.b/div;//(rotateSkew0)/div;
	double r1=-m.c/div;//(rotateSkew1)/div;
	double tx=(m.ty*m.c-m.tx*m.d)/div;//((translateY*rotateSkew1-translateX*scaleY))/div;
	double ty=(m.tx*m.b-m.ty*m.a)/div;//((translateX*rotateSkew0-translateY*scaleX))/div;
	
	m.tx=tx;
	m.ty=ty;
	m.a=sx;
	m.d=sy;
	m.b=r0;
	m.c=r1;
	/*	translateX=tx;
		translateY=ty;
		scaleX=sx;
		scaleY=sy;
		rotateSkew0=r0;
		rotateSkew1=r1;
		CalcFlags();*/
}



Stringp MatrixObject::AS3_toString()
{
	//Add your act code here...
	ShellCore*c=(ShellCore*)core();
	Stringp str=ScriptObject::formatString(c->kSpString,c->kEqualString,c->kQuotString,true,true,true);
	str=str->concatStrings(c->newStringLatin1("("),str);
	str=str->concatStrings(str,c->newStringLatin1(")"));
	return str;
	//return kAvmThunkUndefined;//Modify this please!
}


//new adds 11
void MatrixObject::AS3_copyColumnFrom(uint32_t column, Vector3DObject* vector3D){}
void MatrixObject::AS3_copyColumnTo(uint32_t column, Vector3DObject* vector3D){}
void MatrixObject::AS3_copyFrom(MatrixObject* sourceMatrix)
{
	AS3MATRIXDATA& thisData = GetData();
	AS3MATRIXDATA& sourceData = sourceMatrix->GetData();
	thisData.a =  sourceData.a;
	thisData.b =  sourceData.b;
	thisData.c =  sourceData.c;
	thisData.d =  sourceData.d;
	thisData.tx =  sourceData.tx;
	thisData.ty =  sourceData.ty;
}
void MatrixObject::AS3_copyRowFrom(uint32_t row, Vector3DObject* vector3D){}
void MatrixObject::AS3_copyRowTo(uint32_t row, Vector3DObject* vector3D){}
void MatrixObject::AS3_setTo(double aa, double ba, double ca, double da, double txa, double tya)
{
	AS3MATRIXDATA& thisData = GetData();
	thisData.a =  aa;
	thisData.b =  ba;
	thisData.c =  ca;
	thisData.d =  da;
	thisData.tx =  txa;
	thisData.ty =  tya;
}
//new adds 11 end	
}