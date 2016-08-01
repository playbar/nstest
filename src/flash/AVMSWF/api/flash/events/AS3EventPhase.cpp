#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3EventPhase.h"
namespace avmshell{
EventPhaseClass::EventPhaseClass(VTable* cvtable):ClassClosure(cvtable)
{
	prototype = new (core()->GetGC(), cvtable->getExtraSize())EventPhaseObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* EventPhaseClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) EventPhaseObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
EventPhaseObject::EventPhaseObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}