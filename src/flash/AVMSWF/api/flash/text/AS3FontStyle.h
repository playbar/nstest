#ifndef _AS3FontStyle_
#define _AS3FontStyle_
namespace avmplus{namespace NativeID{
class FontStyleClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp BOLD = "bold";
	//Stringp BOLD_ITALIC = "boldItalic"; 
	//Stringp ITALIC = "italic";
	//Stringp REGULAR = "regular"; 

	Stringp BOLD;
	Stringp BOLD_ITALIC; 
	Stringp ITALIC;
	Stringp REGULAR; 
private:};
class FontStyleObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!	
private:};
}}
namespace avmshell{
	class FontStyleClass : public ClassClosure
	{
	public:
		FontStyleClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FontStyleClassSlots m_slots_FontStyleClass;
	public:
		inline Stringp getSlotBOLD(){return m_slots_FontStyleClass.BOLD;}
		inline Stringp getSlotBOLD_ITALIC(){return m_slots_FontStyleClass.BOLD_ITALIC;} 
		inline Stringp getSlotITALIC(){return m_slots_FontStyleClass.ITALIC;}
		inline Stringp getSlotREGULAR(){return m_slots_FontStyleClass.REGULAR;} 
};
class FontStyleObject : public ScriptObject
{
	public:
		FontStyleObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FontStyleObjectSlots m_slots_FontStyleObject;
};}
#endif