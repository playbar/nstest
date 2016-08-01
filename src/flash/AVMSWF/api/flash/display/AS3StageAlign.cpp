#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3StageAlign.h"
namespace avmshell{
StageAlignClass::StageAlignClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())StageAlignObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* StageAlignClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) StageAlignObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
StageAlignObject::StageAlignObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}