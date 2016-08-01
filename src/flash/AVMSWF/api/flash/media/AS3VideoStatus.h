#ifndef _AS3VideoStatus_
#define _AS3VideoStatus_
namespace avmplus{namespace NativeID{
class VideoStatusClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//ACCELERATED : Stringp = "accelerated"
//SOFTWARE : Stringp = "software"
//UNAVAILABLE : Stringp = "unavailable"

	Stringp ACCELERATED;
	Stringp SOFTWARE;
	Stringp UNAVAILABLE;

private:};
class VideoStatusObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class VideoStatusClass : public ClassClosure
	{
	public:
		VideoStatusClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::VideoStatusClassSlots m_slots_VideoStatusClass;
};
class VideoStatusObject : public ScriptObject
{
	public:
		VideoStatusObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::VideoStatusObjectSlots m_slots_VideoStatusObject;
};}
#endif