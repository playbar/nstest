#ifndef _AS3StageAlign_
#define _AS3StageAlign_
namespace avmplus{namespace NativeID{
class StageAlignClassSlots{
friend class SlotOffsetsAndAsserts;
public:
	Stringp BOTTOM; 
	Stringp BOTTOM_LEFT;
	Stringp BOTTOM_RIGHT;
	Stringp LEFT;
	Stringp RIGHT;
	Stringp TOP;
	Stringp TOP_LEFT;
	Stringp TOP_RIGHT;
private:};
class StageAlignObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!


private:};
}}
namespace avmshell{
	class StageAlignClass : public ClassClosure
	{
	public:
		StageAlignClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageAlignClassSlots m_slots_StageAlignClass;
	public:
		inline Stringp getSlotBOTTOM(){return m_slots_StageAlignClass.BOTTOM;}
		inline Stringp getSlotBOTTOM_LEFT(){return m_slots_StageAlignClass.BOTTOM_LEFT;}
		inline Stringp getSlotBOTTOM_RIGHT(){return m_slots_StageAlignClass.BOTTOM_RIGHT;}
		inline Stringp getSlotLEFT(){return m_slots_StageAlignClass.LEFT;}
		inline Stringp getSlotRIGHT(){return m_slots_StageAlignClass.RIGHT;}
		inline Stringp getSlotTOP(){return m_slots_StageAlignClass.TOP;}
		inline Stringp getSlotTOP_LEFT(){return m_slots_StageAlignClass.TOP_LEFT;}
		inline Stringp getSlotTOP_RIGHT(){return m_slots_StageAlignClass.TOP_RIGHT;}
};
class StageAlignObject : public ScriptObject
{
	public:
		StageAlignObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageAlignObjectSlots m_slots_StageAlignObject;
};}
#endif