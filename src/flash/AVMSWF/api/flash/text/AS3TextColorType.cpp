#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3TextColorType.h"
namespace avmshell{
TextColorTypeClass::TextColorTypeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())TextColorTypeObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* TextColorTypeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextColorTypeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TextColorTypeObject::TextColorTypeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}