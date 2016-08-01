#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3StageDisplayState.h"
namespace avmshell{
StageDisplayStateClass::StageDisplayStateClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())StageDisplayStateObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* StageDisplayStateClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) StageDisplayStateObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
StageDisplayStateObject::StageDisplayStateObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}