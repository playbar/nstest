#ifndef _AS3StageDisplayState_
#define _AS3StageDisplayState_
namespace avmplus{namespace NativeID{
class StageDisplayStateClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp FULL_SCREEN = "fullScreen";
	//Stringp NORMAL = "normal";

	//FULL_SCREEN_INTERACTIVE : String = "fullScreenInteractive"

	Stringp FULL_SCREEN;
	Stringp NORMAL;

	//new adds
	Stringp FULL_SCREEN_INTERACTIVE;
	//new adds end
private:};
class StageDisplayStateObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

	
private:};
}}
namespace avmshell{
	class StageDisplayStateClass : public ClassClosure
	{
	public:
		StageDisplayStateClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageDisplayStateClassSlots m_slots_StageDisplayStateClass;
	public:
		inline Stringp getSlotFULL_SCREEN(){return m_slots_StageDisplayStateClass.FULL_SCREEN;}
		inline Stringp getSlotNORMAL() {return m_slots_StageDisplayStateClass.NORMAL;}

		//new adds
		inline Stringp getSlotFULL_SCREEN_INTERACTIVE(){return m_slots_StageDisplayStateClass.FULL_SCREEN_INTERACTIVE;}
		//new adds end
};
class StageDisplayStateObject : public ScriptObject
{
	public:
		StageDisplayStateObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageDisplayStateObjectSlots m_slots_StageDisplayStateObject;
};}
#endif