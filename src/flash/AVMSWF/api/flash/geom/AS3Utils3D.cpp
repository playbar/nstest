#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3Utils3D.h"
namespace avmshell{
Utils3DClass::Utils3DClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* Utils3DClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) Utils3DObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
Utils3DObject::Utils3DObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
Matrix3DObject* Utils3DObject::AS3_pointTowards(double percent, Matrix3DObject *pMat, Vector3DObject *pPos, Vector3DObject *pAt, Vector3DObject *pUp)
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Vector3DObject* Utils3DObject::AS3_projectVector(Matrix3DObject *pM, Vector3DObject *pV)
{
	//Add your act code here...
	return NULL;//Modify this please!
}

void Utils3DObject::AS3_projectVectors(Matrix3DObject *pM, DoubleVectorObject *pVerts, DoubleVectorObject *pProjectedVerts, DoubleVectorObject *pUvts)
{
	//Add your act code here...
}

}