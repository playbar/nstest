#ifndef _AS3StageScaleMode_
#define _AS3StageScaleMode_
namespace avmplus{namespace NativeID{
class StageScaleModeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp EXACT_FIT = "exactFit";
	//Stringp NO_BORDER = "noBorder";
	//Stringp NO_SCALE = "noScale";
	//Stringp SHOW_ALL = "showAll";

	Stringp EXACT_FIT;
	Stringp NO_BORDER;
	Stringp NO_SCALE;
	Stringp SHOW_ALL;
private:};
class StageScaleModeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

	Stringp EXACT_FIT;
	Stringp NO_BORDER;
	Stringp NO_SCALE;
	Stringp SHOW_ALL;
private:};
}}
namespace avmshell{
	class StageScaleModeClass : public ClassClosure
	{
	public:
		StageScaleModeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageScaleModeClassSlots m_slots_StageScaleModeClass;
	public:
		inline Stringp getSlotEXACT_FIT(){return m_slots_StageScaleModeClass.EXACT_FIT;}
		inline Stringp getSlotNO_BORDER(){return m_slots_StageScaleModeClass.NO_BORDER;}
		inline Stringp getSlotNO_SCALE(){return m_slots_StageScaleModeClass.NO_SCALE;}
		inline Stringp getSlotSHOW_ALL(){return m_slots_StageScaleModeClass.SHOW_ALL;}
};
class StageScaleModeObject : public ScriptObject
{
	public:
		StageScaleModeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageScaleModeObjectSlots m_slots_StageScaleModeObject;
};}
#endif