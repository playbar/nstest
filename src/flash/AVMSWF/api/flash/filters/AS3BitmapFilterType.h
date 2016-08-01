#ifndef _AS3BitmapFilterType_
#define _AS3BitmapFilterType_
namespace avmplus{namespace NativeID{
class BitmapFilterTypeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
//	Stringp FULL = "full";
//	Stringp INNER = "inner";
//	Stringp OUTER = "outer";

	Stringp FULL;
	Stringp INNER;
	Stringp OUTER;
private:};
class BitmapFilterTypeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

	
private:};
}}
namespace avmshell{
	class BitmapFilterTypeClass : public ClassClosure
	{
	public:
		BitmapFilterTypeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BitmapFilterTypeClassSlots m_slots_BitmapFilterTypeClass;
	public:
		inline Stringp getSlotFULL(){return m_slots_BitmapFilterTypeClass.FULL;}
		inline Stringp getSlotINNER(){return m_slots_BitmapFilterTypeClass.INNER;}
		inline Stringp getSlotOUTER(){return m_slots_BitmapFilterTypeClass.OUTER;}
};
class BitmapFilterTypeObject : public ScriptObject
{
	public:
		BitmapFilterTypeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BitmapFilterTypeObjectSlots m_slots_BitmapFilterTypeObject;
};}
#endif