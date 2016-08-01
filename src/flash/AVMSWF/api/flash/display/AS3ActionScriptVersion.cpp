#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ActionScriptVersion.h"
namespace avmshell{
ActionScriptVersionClass::ActionScriptVersionClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())ActionScriptVersionObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* ActionScriptVersionClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ActionScriptVersionObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ActionScriptVersionObject::ActionScriptVersionObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}