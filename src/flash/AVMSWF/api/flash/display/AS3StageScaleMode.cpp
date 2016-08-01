#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3StageScaleMode.h"
namespace avmshell{
StageScaleModeClass::StageScaleModeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())StageScaleModeObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* StageScaleModeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) StageScaleModeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
StageScaleModeObject::StageScaleModeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}