#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SpaceJustifier.h"
namespace avmshell{
SpaceJustifierClass::SpaceJustifierClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* SpaceJustifierClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SpaceJustifierObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

SpaceJustifierObject* SpaceJustifierClass::CreateObject()
{
	Atom atoms=this->atom();
	SpaceJustifierObject*pObj=(SpaceJustifierObject*)this->construct(0,&atoms);
		//createInstance(ivtable(),prototype);
	
	//pObj->construct(0,&atoms);
	return pObj;
}

SpaceJustifierObject::SpaceJustifierObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
bool SpaceJustifierObject::AS3_letterSpacing_get()
{
	//Add your act code here...
	return false;//Modify this please!
}

void SpaceJustifierObject::AS3_letterSpacing_set(bool letterSpacing)
{
	//Add your act code here...
}

double SpaceJustifierObject::AS3_maximumSpacing_get()
{
	//Add your act code here...
	return 0.0;//Modify this please!
}

void SpaceJustifierObject::AS3_maximumSpacing_set(double maximumSpacing)
{
	//Add your act code here...
}

double SpaceJustifierObject::AS3_minimumSpacing_get()
{
	//Add your act code here...
	return 0.0;//Modify this please!
}

void SpaceJustifierObject::AS3_minimumSpacing_set(double minimumSpacing)
{
	//Add your act code here...
}

double SpaceJustifierObject::AS3_optimumSpacing_get()
{
	//Add your act code here...
	return 0.0;//Modify this please!
}

void SpaceJustifierObject::AS3_optimumSpacing_set(double optimumSpacing)
{
	//Add your act code here...
}

void SpaceJustifierObject::AS3_cloneSpacing(SpaceJustifierObject*)
{

}

//AS3 contructor function..
//AvmBox SpaceJustifierObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox SpaceJustifierObject::AS3_clone(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}

}