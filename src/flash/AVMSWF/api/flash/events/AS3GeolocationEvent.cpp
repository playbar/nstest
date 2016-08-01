#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3GeolocationEvent.h"
namespace avmshell{
GeolocationEventClass::GeolocationEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* GeolocationEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) GeolocationEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
GeolocationEventObject::GeolocationEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//double GeolocationEventObject::AS3_altitude_get()
//{
//	return 0;
//}
//void GeolocationEventObject::AS3_altitude_set(double altitude)
//{
//
//}
//double GeolocationEventObject::AS3_heading_get()
//{
//	return 0;
//}
//void GeolocationEventObject::AS3_heading_set(double heading)
//{
//
//}
//double GeolocationEventObject::AS3_horizontalAccuracy_get()
//{
//	return 0;
//}
//void GeolocationEventObject::AS3_horizontalAccuracy_set(double horizontalAccuracy)
//{
//
//}
//double GeolocationEventObject::AS3_latitude_get()
//{
//	return 0;
//}
//void GeolocationEventObject::AS3_latitude_set(double latitude)
//{
//
//}
//double GeolocationEventObject::AS3_longitude_get()
//{
//	return 0;
//}
//void GeolocationEventObject::AS3_longitude_set(double longitude)
//{
//
//}
//double GeolocationEventObject::AS3_speed_get()
//{
//	return 0;
//}
//void GeolocationEventObject::AS3_speed_set(double speed)
//{
//
//}
//double GeolocationEventObject::AS3_timestamp_get()
//{
//	return 0;
//}
//void GeolocationEventObject::AS3_timestamp_set(double timestamp)
//{
//
//}
//double GeolocationEventObject::AS3_verticalAccuracy_get()
//{
//	return 0;
//}
//void GeolocationEventObject::AS3_verticalAccuracy_set(double verticalAccuracy)
//{
//
//}
//
//void GeolocationEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, double latitude, 
//											 double longitude, double altitude, double hAccuracy, double vAccuracy, 
//											 double speed, double heading, double timestamp)
//{
//
//}
//EventObject* GeolocationEventObject::AS3_clone()
//{
//	return NULL;
//}
//Stringp GeolocationEventObject::AS3_toString()
//{
//	return NULL;
//}
}