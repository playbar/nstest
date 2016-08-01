#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SoftKeyboardTrigger.h"
namespace avmshell{
SoftKeyboardTriggerClass::SoftKeyboardTriggerClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* SoftKeyboardTriggerClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SoftKeyboardTriggerObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SoftKeyboardTriggerObject::SoftKeyboardTriggerObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}