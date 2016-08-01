#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3Accessibility.h"
namespace avmshell{
AccessibilityClass::AccessibilityClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())AccessibilityObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* AccessibilityClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) AccessibilityObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
AccessibilityObject::AccessibilityObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
bool AccessibilityClass::AS3_active_get()
{
	//Add your act code here...
	return false;
	//return kAvmThunkUndefined;//Modify this please!
}

void AccessibilityClass::AS3_updateProperties()
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
}

void AccessibilityClass::AS3_sendEvent(DisplayObjectObject*,XU32,XU32,bool)
{
}

}