#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SWFVersion.h"
namespace avmshell{
SWFVersionClass::SWFVersionClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())SWFVersionObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* SWFVersionClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SWFVersionObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SWFVersionObject::SWFVersionObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}