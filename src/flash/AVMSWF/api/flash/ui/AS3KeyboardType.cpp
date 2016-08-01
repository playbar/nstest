#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3KeyboardType.h"
namespace avmshell{
KeyboardTypeClass::KeyboardTypeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* KeyboardTypeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) KeyboardTypeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
KeyboardTypeObject::KeyboardTypeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}