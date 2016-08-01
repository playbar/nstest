#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3EastAsianJustifier.h"
namespace avmshell{
EastAsianJustifierClass::EastAsianJustifierClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* EastAsianJustifierClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) EastAsianJustifierObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

EastAsianJustifierObject* EastAsianJustifierClass::CreateObject()
{
	Atom atoms=this->atom();
	EastAsianJustifierObject*pObject=(EastAsianJustifierObject*)this->construct(0,&atoms);//createInstance(ivtable(),prototype);
	
	//pObject->construct(0,&atoms);
	return pObject;
}

EastAsianJustifierObject::EastAsianJustifierObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}


//////////////////////////////////////////////////////////
//Native Method start...
Stringp EastAsianJustifierObject::AS3_justificationStyle_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

void EastAsianJustifierObject::AS3_justificationStyle_set(Stringp justificationStyle)
{
	//Add your act code here...
}

//AS3 contructor function..
//AvmBox EastAsianJustifierObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox EastAsianJustifierObject::AS3_clone(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}

}