#ifndef _AS3TextFieldAutoSize_
#define _AS3TextFieldAutoSize_
namespace avmplus{namespace NativeID{
class TextFieldAutoSizeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp CENTER = "center";
	//Stringp LEFT = "left";
	//Stringp NONE = "none";
	//Stringp RIGHT = "right"; 

	Stringp CENTER;
	Stringp LEFT;
	Stringp NONE;
	Stringp RIGHT; 
private:};
class TextFieldAutoSizeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!	
private:};
}}
namespace avmshell{
	class TextFieldAutoSizeClass : public ClassClosure
	{
	public:
		TextFieldAutoSizeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextFieldAutoSizeClassSlots m_slots_TextFieldAutoSizeClass;
	public:
		inline Stringp getSlotCENTER(){return m_slots_TextFieldAutoSizeClass.CENTER;}
		inline Stringp getSlotLEFT(){return m_slots_TextFieldAutoSizeClass.LEFT;}
		inline Stringp getSlotNONE(){return m_slots_TextFieldAutoSizeClass.NONE;}
		inline Stringp getSlotRIGHT(){return m_slots_TextFieldAutoSizeClass.RIGHT;}
		int ToID(Stringp);
		Stringp ToString(int);
};
class TextFieldAutoSizeObject : public ScriptObject
{
	public:
		TextFieldAutoSizeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextFieldAutoSizeObjectSlots m_slots_TextFieldAutoSizeObject;
};}
#endif