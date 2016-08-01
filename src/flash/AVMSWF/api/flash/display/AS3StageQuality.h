#ifndef _AS3StageQuality_
#define _AS3StageQuality_
namespace avmplus{namespace NativeID{
class StageQualityClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp BEST = "best";
	//Stringp HIGH = "high";
	//Stringp LOW = "low";
	//Stringp MEDIUM = "medium";

 	//new adds 11

 	//HIGH_16X16:String = "16x16"
 	//HIGH_16X16_LINEAR:String = "16x16linear"
 	//HIGH_8X8:String = "8x8"
 	//HIGH_8X8_LINEAR:String = "8x8linear"

 	//new adds 11 end


	Stringp BEST;
	Stringp HIGH;
	Stringp LOW;
	Stringp MEDIUM; 
	Stringp HIGH_16X16;
	Stringp HIGH_16X16_LINEAR;
	Stringp HIGH_8X8;
	Stringp HIGH_8X8_LINEAR;
private:};
class StageQualityObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

	 
private:};
}}
namespace avmshell{
	class StageQualityClass : public ClassClosure
	{
	public:
		StageQualityClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageQualityClassSlots m_slots_StageQualityClass;
	public:
		inline Stringp getSlotBEST(){return m_slots_StageQualityClass.BEST;}
		inline Stringp getSlotHIGH(){return m_slots_StageQualityClass.HIGH;}
		inline Stringp getSlotLOW(){return m_slots_StageQualityClass.LOW;}
		inline Stringp getSlotMEDIUM(){return m_slots_StageQualityClass.MEDIUM;}
		inline Stringp getSlotHIGH_16X16(){return m_slots_StageQualityClass.HIGH_16X16;}
		inline Stringp getSlotHIGH_16X16_LINEAR(){return m_slots_StageQualityClass.HIGH_16X16_LINEAR;}
		inline Stringp getSlotHIGH_8X8(){return m_slots_StageQualityClass.HIGH_8X8;}
		inline Stringp getSlotHIGH_8X8_LINEAR(){return m_slots_StageQualityClass.HIGH_8X8_LINEAR;}
};
class StageQualityObject : public ScriptObject
{
	public:
		StageQualityObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageQualityObjectSlots m_slots_StageQualityObject;
};}
#endif