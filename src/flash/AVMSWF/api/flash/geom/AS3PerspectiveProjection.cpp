#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3PerspectiveProjection.h"
namespace avmshell{
PerspectiveProjectionClass::PerspectiveProjectionClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* PerspectiveProjectionClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) PerspectiveProjectionObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

PerspectiveProjectionObject* PerspectiveProjectionClass::CreateObject()
{
	PerspectiveProjectionObject*pObj=(PerspectiveProjectionObject*)createInstance(ivtable(),prototype);
	return pObj;
}

PerspectiveProjectionObject::PerspectiveProjectionObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
double PerspectiveProjectionObject::AS3_fieldOfView_get()
{
	//Add your act code here...
	return 0.0;//Modify this please!
}

void PerspectiveProjectionObject::AS3_fieldOfView_set(double fieldOfView)
{
	//Add your act code here...
}

double PerspectiveProjectionObject::AS3_focalLength_get()
{
	//Add your act code here...
	return 0.0;//Modify this please!
}

void PerspectiveProjectionObject::AS3_focalLength_set(double focalLength)
{
	//Add your act code here...
}

PointObject* PerspectiveProjectionObject::AS3_projectionCenter_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

void PerspectiveProjectionObject::AS3_projectionCenter_set(PointObject *pProjectionCenter)
{
	//Add your act code here...
}

//AS3 contructor function..
void PerspectiveProjectionObject::AS3_constructor()
{
	//Add your act code here...
}

Matrix3DObject* PerspectiveProjectionObject::AS3_toMatrix3D()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

}