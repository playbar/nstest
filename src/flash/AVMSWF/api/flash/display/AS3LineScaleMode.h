#ifndef _AS3LineScaleMode_
#define _AS3LineScaleMode_
namespace avmplus{namespace NativeID{
class LineScaleModeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp HORIZONTAL = "horizontal";
	//Stringp NONE = "none";
	//Stringp NORMAL = "normal"; 
	//Stringp VERTICAL = "vertical"; 

	Stringp HORIZONTAL;
	Stringp NONE;
	Stringp NORMAL; 
	Stringp VERTICAL; 
private:};
class LineScaleModeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

private:};
}}
namespace avmshell{
	class LineScaleModeClass : public ClassClosure
	{
	public:
		LineScaleModeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LineScaleModeClassSlots m_slots_LineScaleModeClass;
	public:
		inline Stringp getSlotHORIZONTAL(){return m_slots_LineScaleModeClass.HORIZONTAL;}
		inline Stringp getSlotNONE(){return m_slots_LineScaleModeClass.NONE;}
		inline Stringp getSlotNORMAL(){return m_slots_LineScaleModeClass.NORMAL;}
		inline Stringp getSlotVERTICAL(){return m_slots_LineScaleModeClass.VERTICAL;}
};
class LineScaleModeObject : public ScriptObject
{
	public:
		LineScaleModeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LineScaleModeObjectSlots m_slots_LineScaleModeObject;
};}
#endif