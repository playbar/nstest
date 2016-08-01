#ifndef _AS3BitmapFilterQuality_
#define _AS3BitmapFilterQuality_
namespace avmplus{namespace NativeID{
class BitmapFilterQualityClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//int32_t HIGH = 3;
	//int32_t LOW = 1;
	//int32_t MEDIUM = 2;

	int32_t HIGH;
	int32_t LOW;
	int32_t MEDIUM;
private:};
class BitmapFilterQualityObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class BitmapFilterQualityClass : public ClassClosure
	{
	public:
		BitmapFilterQualityClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BitmapFilterQualityClassSlots m_slots_BitmapFilterQualityClass;
	public:
		inline int32_t getSlotHIGH(){return m_slots_BitmapFilterQualityClass.HIGH;}
		inline int32_t getSlotLOW(){return m_slots_BitmapFilterQualityClass.LOW;}
		inline int32_t getSlotMEDIUM(){return m_slots_BitmapFilterQualityClass.MEDIUM;}
};
class BitmapFilterQualityObject : public ScriptObject
{
	public:
		BitmapFilterQualityObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BitmapFilterQualityObjectSlots m_slots_BitmapFilterQualityObject;
};}
#endif