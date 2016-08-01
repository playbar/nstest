#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3TransformGestureEvent.h"
namespace avmshell{
TransformGestureEventClass::TransformGestureEventClass(VTable* cvtable):ClassClosure(cvtable)//GestureEventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* TransformGestureEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TransformGestureEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TransformGestureEventObject::TransformGestureEventObject(VTable *vtable, ScriptObject* proto, int capacity): GestureEventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//double TransformGestureEventObject::AS3_offsetX_get()
//{
//	return 0;
//}
//void TransformGestureEventObject::AS3_offsetX_set(double offsetX)
//{
//
//}
//double TransformGestureEventObject::AS3_offsetY_get()
//{
//	return 0;
//}
//void TransformGestureEventObject::AS3_offsetY_set(double offsetY)
//{
//
//}
//double TransformGestureEventObject::AS3_rotation_get()
//{
//	return 0;
//}
//void TransformGestureEventObject::AS3_rotation_set(double rotation)
//{
//
//}
//double TransformGestureEventObject::AS3_scaleX_get()
//{
//	return 0;
//}
//void TransformGestureEventObject::AS3_scaleX_set(double scaleX)
//{
//
//}
//double TransformGestureEventObject::AS3_scaleY_get()
//{
//	return 0;
//}
//void TransformGestureEventObject::AS3_scaleY_set(double scaleY)
//{
//
//}
//
//void TransformGestureEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
//												  Stringp phase, double localX, double localY, double scaleX, double scaleY, 
//												  double rotation, double offsetX, double offsetY, bool ctrlKey, 
//												  bool altKey, bool shiftKey, bool commandKey, bool controlKey)
//{
//
//}
//EventObject* TransformGestureEventObject::AS3_clone()
//{
//	return NULL;
//}
//Stringp TransformGestureEventObject::AS3_toString()
//{
//	return NULL;
//}

}