#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3StageQuality.h"
namespace avmshell{
StageQualityClass::StageQualityClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())StageQualityObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* StageQualityClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) StageQualityObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
StageQualityObject::StageQualityObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}