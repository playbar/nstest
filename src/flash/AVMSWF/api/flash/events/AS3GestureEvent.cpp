#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3GestureEvent.h"
namespace avmshell{
GestureEventClass::GestureEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* GestureEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) GestureEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
GestureEventObject::GestureEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
bool GestureEventObject::AS3_altKey_get()
{
	return false;
}
void GestureEventObject::AS3_altKey_set(bool altKey)
{

}
bool GestureEventObject::AS3_commandKey_get()
{
	return false;
}
void GestureEventObject::AS3_commandKey_set(bool commandKey)
{

}
bool GestureEventObject::AS3_controlKey_get()
{
	return false;
}
void GestureEventObject::AS3_controlKey_set(bool controlKey)
{

}
bool GestureEventObject::AS3_ctrlKey_get()
{
	return false;
}
void GestureEventObject::AS3_ctrlKey_set(bool ctrlKey)
{

}
double GestureEventObject::AS3_localX_get()
{
	return 0;
}
void GestureEventObject::AS3_localX_set(double localX)
{

}
double GestureEventObject::AS3_localY_get()
{
	return 0;
}
void GestureEventObject::AS3_localY_set(double localY)
{

}
Stringp GestureEventObject::AS3_phase_get()
{
	return NULL;
}
void GestureEventObject::AS3_phase_set(Stringp pPhase)
{

}
bool GestureEventObject::AS3_shiftKey_get()
{
	return false;
}
void GestureEventObject::AS3_shiftKey_set(bool shiftKey)
{

}
double GestureEventObject::AS3_stageX_get()
{
	return 0;
}
double GestureEventObject::AS3_stageY_get()
{
	return 0;
}

void GestureEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
										 Stringp pPhase, double localX, double localY, bool ctrlKey, 
										 bool altKey, bool shiftKey, bool commandKey, bool controlKey)
{

}
EventObject* GestureEventObject::AS3_clone()
{
	return NULL;
}
Stringp GestureEventObject::AS3_toString()
{
	return NULL;
}
void GestureEventObject::AS3_updateAfterEvent()
{

}

}