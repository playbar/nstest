#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SpreadMethod.h"
namespace avmshell{
SpreadMethodClass::SpreadMethodClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())SpreadMethodObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* SpreadMethodClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SpreadMethodObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SpreadMethodObject::SpreadMethodObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}