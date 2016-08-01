#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3AntiAliasType.h"
namespace avmshell{
AntiAliasTypeClass::AntiAliasTypeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())AntiAliasTypeObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* AntiAliasTypeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) AntiAliasTypeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
AntiAliasTypeObject::AntiAliasTypeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}