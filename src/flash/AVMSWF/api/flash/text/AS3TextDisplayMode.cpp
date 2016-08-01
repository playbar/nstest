#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3TextDisplayMode.h"
namespace avmshell{
TextDisplayModeClass::TextDisplayModeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())TextDisplayModeObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* TextDisplayModeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextDisplayModeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TextDisplayModeObject::TextDisplayModeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}