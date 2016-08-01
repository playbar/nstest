#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3PressAndTapGestureEvent.h"
namespace avmshell{
PressAndTapGestureEventClass::PressAndTapGestureEventClass(VTable* cvtable):ClassClosure(cvtable)//GestureEventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* PressAndTapGestureEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) PressAndTapGestureEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
PressAndTapGestureEventObject::PressAndTapGestureEventObject(VTable *vtable, ScriptObject* proto, int capacity): GestureEventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
double PressAndTapGestureEventObject::AS3_tapLocalX_get()
{
	return 0;
}
void PressAndTapGestureEventObject::AS3_tapLocalX_set(double tapLocalX)
{

}
double PressAndTapGestureEventObject::AS3_tapLocalY_get()
{
	return 0;
}
void PressAndTapGestureEventObject::AS3_tapLocalY_set(double tapLocalY)
{

}
double PressAndTapGestureEventObject::AS3_tapStageX_get()
{
	return 0;
}
double PressAndTapGestureEventObject::AS3_tapStageY_get()
{
	return 0;
}

//void PressAndTapGestureEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, Stringp pPhase, 
//													double localX, double localY, double tapLocalX, double tapLocalY, 
//													bool ctrlKey, bool altKey, bool shiftKey, bool commandKey, bool controlKey)
//{
//
//}
//EventObject* PressAndTapGestureEventObject::AS3_clone()
//{
//	return NULL;
//}
//Stringp PressAndTapGestureEventObject::AS3_toString()
//{
//	return NULL;
//}

}