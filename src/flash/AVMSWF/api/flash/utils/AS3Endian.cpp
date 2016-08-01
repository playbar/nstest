#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3Endian.h"
namespace avmshell{
EndianClass::EndianClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())EndianObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* EndianClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) EndianObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
EndianObject::EndianObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}