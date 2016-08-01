#ifndef _AS3SampleDataEvent_
#define _AS3SampleDataEvent_

#include "AS3Event.h"
#include "AS3ByteArray.h"	

namespace avmplus{namespace NativeID{

class ByteArrayObject;

class SampleDataEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//SAMPLE_DATA : String = "sampleData"

	Stringp SAMPLE_DATA;

private:};
class SampleDataEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	double m_position;
	ByteArrayObject *m_data;
private:};
}}
namespace avmshell{
	class SampleDataEventClass : public ClassClosure//EventClass
	{
	public:
		SampleDataEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SampleDataEventClassSlots m_slots_SampleDataEventClass;
};
class SampleDataEventObject : public EventObject
{
	public:
		SampleDataEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//ByteArrayObject* AS3_data_get();
		//void AS3_data_set(ByteArrayObject *pData);
		//double AS3_position_get();
		//void AS3_position_set(double position);

		//void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, double theposition, ByteArrayObject *pThedata);
		//EventObject* AS3_clone();
		//Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SampleDataEventObjectSlots m_slots_SampleDataEventObject;
};}
#endif