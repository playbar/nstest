#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3BitmapFilterType.h"
namespace avmshell{
BitmapFilterTypeClass::BitmapFilterTypeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())BitmapFilterTypeObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* BitmapFilterTypeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) BitmapFilterTypeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
BitmapFilterTypeObject::BitmapFilterTypeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}