#ifndef _AS3StageVideoAvailability_
#define _AS3StageVideoAvailability_
namespace avmplus{namespace NativeID{
class StageVideoAvailabilityClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//AVAILABLE : String = "available"
	//UNAVAILABLE : String = "unavailable"

	Stringp AVAILABLE;
	Stringp UNAVAILABLE;

private:};
class StageVideoAvailabilityObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class StageVideoAvailabilityClass : public ClassClosure
	{
	public:
		StageVideoAvailabilityClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageVideoAvailabilityClassSlots m_slots_StageVideoAvailabilityClass;
};
class StageVideoAvailabilityObject : public ScriptObject
{
	public:
		StageVideoAvailabilityObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageVideoAvailabilityObjectSlots m_slots_StageVideoAvailabilityObject;
};}
#endif