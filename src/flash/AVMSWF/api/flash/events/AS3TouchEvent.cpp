#include "StdAfxflash.h"
#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3TouchEvent.h"
#include "AS3TouchEventIntent.h"
namespace avmshell{
TouchEventClass::TouchEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* TouchEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TouchEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TouchEventObject::TouchEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
bool TouchEventObject::AS3_altKey_get()
{
	return false;
}
void TouchEventObject::AS3_altKey_set(bool altKey)
{

}
bool TouchEventObject::AS3_commandKey_get()
{
	return false;
}
void TouchEventObject::AS3_commandKey_set(bool commandKey)
{

}
bool TouchEventObject::AS3_controlKey_get()
{
	return false;
}
void TouchEventObject::AS3_controlKey_set(bool controlKey)
{

}
bool TouchEventObject::AS3_ctrlKey_get()
{
	return false;
}
void TouchEventObject::AS3_ctrlKey_set(bool ctrlKey)
{

}
bool TouchEventObject::AS3_isPrimaryTouchPoint_get()
{
	return false;
}
void TouchEventObject::AS3_isPrimaryTouchPoint_set(bool isPrimaryTouchPoint)
{

}
bool TouchEventObject::AS3_isRelatedObjectInaccessible_get()
{
	return false;
}
void TouchEventObject::AS3_isRelatedObjectInaccessible_set(bool isRelatedObjectInaccessible)
{

}
double TouchEventObject::AS3_localX_get()
{
	return 0;
}
void TouchEventObject::AS3_localX_set(double localX)
{

}
double TouchEventObject::AS3_localY_get()
{
	return 0;
}
void TouchEventObject::AS3_localY_set(double localY)
{

}
double TouchEventObject::AS3_pressure_get()
{
	return 0;
}
void TouchEventObject::AS3_pressure_set(double pressure)
{

}
InteractiveObjectObject* TouchEventObject::AS3_relatedObject_get()
{
	return 0;
}
void TouchEventObject::AS3_relatedObject_set(InteractiveObjectObject *pRelatedObject)
{

}
bool TouchEventObject::AS3_shiftKey_get()
{
	return false;
}
void TouchEventObject::AS3_shiftKey_set(bool shiftKey)
{

}
double TouchEventObject::AS3_sizeX_get()
{
	return 0;
}
void TouchEventObject::AS3_sizeX_set(double sizeX)
{

}
double TouchEventObject::AS3_sizeY_get()
{
	return 0;
}
void TouchEventObject::AS3_sizeY_set(double sizeY)
{

}
double TouchEventObject::AS3_stageX_get()
{
	return 0;
}
double TouchEventObject::AS3_stageY_get()
{
	return 0;
}
int TouchEventObject::AS3_touchPointID_get()
{
	return 0;
}
void TouchEventObject::AS3_touchPointID_set(int touchPointID)
{

}

void TouchEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
									   int touchPointID, bool isPrimaryTouchPoint, double localX, 
									   double localY, double sizeX, double sizeY, double pressure, 
									   InteractiveObjectObject *pRelatedObject, bool ctrlKey, bool altKey, 
									   bool shiftKey, bool commandKey, bool controlKey, double timstamp,
									   Stringp touchIntent, bool isTouchPointCanceled)
{

}
EventObject* TouchEventObject::AS3_clone()
{
	return NULL;
}
Stringp TouchEventObject::AS3_toString()
{
	return NULL;
}
void TouchEventObject::AS3_updateAfterEvent()
{

}

//new adds 11
bool TouchEventObject::AS3_isTouchPointCanceled_get()
{
	LOGWHERE();
	return false;
}
void TouchEventObject::AS3_isTouchPointCanceled_set(bool b)
{
	LOGWHERE();

}
double TouchEventObject::AS3_timestamp_get()
{
	LOGWHERE();
	return 0; //NAN
}
void TouchEventObject::AS3_timestamp_set(double d)
{
	LOGWHERE();

}
Stringp TouchEventObject::AS3_touchIntent_get()
{
	LOGWHERE();
	return ((ShellToplevel*)toplevel())->getTouchEventIntentClass()->getSlotUNKNOWN();//NULL;
}
void TouchEventObject::AS3_touchIntent_set(Stringp s)
{
	LOGWHERE();

}
//new adds 11 end 

//new adds 11
uint32_t TouchEventObject::AS3_getSamples(ByteArrayObject* buffer, bool append)
{
	LOGWHERE();
	return 0;
}
bool TouchEventObject::AS3_isToolButtonDown(int index)
{
	LOGWHERE();
	return false;
}
//new adds 11 end
}