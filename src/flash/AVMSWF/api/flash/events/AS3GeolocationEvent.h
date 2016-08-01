#ifndef _AS3GeolocationEvent_
#define _AS3GeolocationEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class GeolocationEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//UPDATE : String = "update"

	Stringp UPDATE;

private:};
class GeolocationEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	double m_latitude;
	double m_longitude;
	double m_altitude;
	double m_horizontalAccuracy;
	double m_verticalAccuracy;
	double m_speed;
	double m_heading;
	double m_timestamp;
private:};
}}
namespace avmshell{
	class GeolocationEventClass : public ClassClosure//EventClass
	{
	public:
		GeolocationEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GeolocationEventClassSlots m_slots_GeolocationEventClass;
};
class GeolocationEventObject : public EventObject
{
	public:
		GeolocationEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//double AS3_altitude_get();
		//void AS3_altitude_set(double altitude);
		//double AS3_heading_get();
		//void AS3_heading_set(double heading);
		//double AS3_horizontalAccuracy_get();
		//void AS3_horizontalAccuracy_set(double horizontalAccuracy);
		//double AS3_latitude_get();
		//void AS3_latitude_set(double latitude);
		//double AS3_longitude_get();
		//void AS3_longitude_set(double longitude);
		//double AS3_speed_get();
		//void AS3_speed_set(double speed);
		//double AS3_timestamp_get();
		//void AS3_timestamp_set(double timestamp);
		//double AS3_verticalAccuracy_get();
		//void AS3_verticalAccuracy_set(double verticalAccuracy);

		//void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, double latitude, 
		//						double longitude, double altitude, double hAccuracy, double vAccuracy, 
		//						double speed, double heading, double timestamp);
		//EventObject* AS3_clone();
		//Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GeolocationEventObjectSlots m_slots_GeolocationEventObject;
};}
#endif