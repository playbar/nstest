#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3Vector3D.h"
namespace avmshell{
Vector3DClass::Vector3DClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* Vector3DClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) Vector3DObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

Vector3DObject* Vector3DClass::CreateObject(double x,double y,double z)
{
	Vector3DObject*pObj=(Vector3DObject*)createInstance(ivtable(),prototype);
	pObj->SetX(x);
	pObj->SetY(y);
	pObj->SetZ(z);	
	pObj->SetW(0);
	return pObj;
}

Vector3DObject::Vector3DObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//AvmBox Vector3DObject::AS3_angleBetween(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_distance(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_length_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_lengthSquared_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_w_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_w_set(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_x_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_x_set(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_y_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_y_set(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_z_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_z_set(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
////AS3 contructor function..
//AvmBox Vector3DObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_add(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_clone(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_crossProduct(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_decrementBy(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_dotProduct(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_equals(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_incrementBy(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_nearEquals(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_negate(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_normalize(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_project(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_scaleBy(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_subtract(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox Vector3DObject::AS3_toString(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}

		//new adds 11
void Vector3DObject::AS3_copyFrom(Vector3DObject* sourceVector3D)
{
	//LOGWHERE();
	this->SetX(sourceVector3D->GetX());
	this->SetY(sourceVector3D->GetY());
	this->SetZ(sourceVector3D->GetZ());
	this->SetW(sourceVector3D->GetW());
}
void Vector3DObject::AS3_setTo(double xa, double ya, double za)
{
	//LOGWHERE();
	this->SetX(xa);
	this->SetY(ya);
	this->SetZ(za);
	this->SetW(0);
}
        //new adds 11 end
}