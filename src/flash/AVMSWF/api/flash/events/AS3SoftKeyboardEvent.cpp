#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SoftKeyboardEvent.h"
namespace avmshell{
SoftKeyboardEventClass::SoftKeyboardEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* SoftKeyboardEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SoftKeyboardEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SoftKeyboardEventObject::SoftKeyboardEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//InteractiveObjectObject* SoftKeyboardEventObject::AS3_relatedObject_get()
//{
//	return NULL;
//}
//void SoftKeyboardEventObject::AS3_relatedObject_set(InteractiveObjectObject *pRelatedObject)
//{
//
//}
//Stringp SoftKeyboardEventObject::AS3_triggerType_get()
//{
//	return NULL;
//}
//
//void SoftKeyboardEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
//											  InteractiveObjectObject *pRelatedObjectVal, Stringp pTriggerTypeVal)
//{
//
//}
//EventObject* SoftKeyboardEventObject::AS3_clone()
//{
//	return NULL;
//}
//Stringp SoftKeyboardEventObject::AS3_toString()
//{
//	return NULL;
//}

}