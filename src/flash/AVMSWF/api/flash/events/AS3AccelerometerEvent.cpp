#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3AccelerometerEvent.h"
namespace avmshell{
AccelerometerEventClass::AccelerometerEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* AccelerometerEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) AccelerometerEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
AccelerometerEventObject::AccelerometerEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//double AccelerometerEventObject::AS3_accelerationX_get()
//{
//	return 0;
//}
//void AccelerometerEventObject::AS3_accelerationX_set(double accelerationX)
//{
//
//}
//double AccelerometerEventObject::AS3_accelerationY_get()
//{
//	return 0;
//}
//void AccelerometerEventObject::AS3_accelerationY_set(double accelerationY)
//{
//
//}
//double AccelerometerEventObject::AS3_accelerationZ_get()
//{
//	return 0;
//}
//void AccelerometerEventObject::AS3_accelerationZ_set(double accelerationZ)
//{
//
//}
//double AccelerometerEventObject::AS3_timestamp_get()
//{
//	return 0;
//}
//void AccelerometerEventObject::AS3_timestamp_set(double timestamp)
//{
//
//}
//
//void AccelerometerEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
//											   double timestamp, double accelerationX, double accelerationY, double accelerationZ)
//{
//
//}
//Stringp AccelerometerEventObject::AS3_toString()
//{
//	return NULL;
//}
//EventObject* AccelerometerEventObject::AS3_clone()
//{
//	return NULL;
//}

}
