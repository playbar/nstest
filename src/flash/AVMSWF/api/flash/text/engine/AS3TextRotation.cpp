#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3TextRotation.h"
namespace avmshell{
TextRotationClass::TextRotationClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* TextRotationClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextRotationObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TextRotationObject::TextRotationObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}