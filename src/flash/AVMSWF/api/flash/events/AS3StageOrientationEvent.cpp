#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3StageOrientationEvent.h"
namespace avmshell{
StageOrientationEventClass::StageOrientationEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* StageOrientationEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) StageOrientationEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
StageOrientationEventObject::StageOrientationEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
Stringp StageOrientationEventObject::AS3_afterOrientation_get()
{
	return NULL;
}
Stringp StageOrientationEventObject::AS3_beforeOrientation_get()
{
	return NULL;
}

void StageOrientationEventObject::AS3_constructor(String pType, bool bubbles, bool cancelable, 
												  Stringp pBeforeOrientation, Stringp pAfterOrientation)
{

}
EventObject* StageOrientationEventObject::AS3_clone()
{
	return NULL;
}
Stringp StageOrientationEventObject::AS3_toString()
{
	return NULL;
}

}