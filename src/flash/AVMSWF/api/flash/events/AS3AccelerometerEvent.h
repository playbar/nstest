#ifndef _AS3AccelerometerEvent_
#define _AS3AccelerometerEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class AccelerometerEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//UPDATE : String = "update"

	Stringp UPDATE;

private:};
class AccelerometerEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	double m_accelerationX;
	double m_accelerationY;
	double m_accelerationZ;
	double m_timestamp;
private:};
}}
namespace avmshell{
	class AccelerometerEventClass : public ClassClosure//EventClass
	{
	public:
		AccelerometerEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AccelerometerEventClassSlots m_slots_AccelerometerEventClass;
};
class AccelerometerEventObject : public EventObject
{
	public:
		AccelerometerEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//double AS3_accelerationX_get();
		//void AS3_accelerationX_set(double accelerationX);
		//double AS3_accelerationY_get();
		//void AS3_accelerationY_set(double accelerationY);
		//double AS3_accelerationZ_get();
		//void AS3_accelerationZ_set(double accelerationZ);
		//double AS3_timestamp_get();
		//void AS3_timestamp_set(double timestamp);

		//void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
		//						double timestamp, double accelerationX, double accelerationY, double accelerationZ);
		//Stringp AS3_toString();
		//EventObject* AS3_clone();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AccelerometerEventObjectSlots m_slots_AccelerometerEventObject;
};}
#endif