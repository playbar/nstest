#ifndef _AS3TextFormatAlign_
#define _AS3TextFormatAlign_
namespace avmplus{namespace NativeID{
class TextFormatAlignClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp CENTER = "center";
	//Stringp JUSTIFY = "justify"; 
	//Stringp LEFT = "left";
	//Stringp RIGHT = "right"; 

//new adds 11
 	//END:String = "end"
 	//START:String = "start"
 //new adds 11 end

	Stringp CENTER;
	Stringp JUSTIFY; 
	Stringp LEFT;
	Stringp RIGHT; 
	Stringp END;
	Stringp START;
private:};
class TextFormatAlignObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

	Stringp CENTER;
	Stringp JUSTIFY; 
	Stringp LEFT;
	Stringp RIGHT; 
private:};
}}
namespace avmshell{
	class TextFormatAlignClass : public ClassClosure
	{
	public:
		TextFormatAlignClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		Stringp IDToString(short id);
		short   StringToID(Stringp s);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextFormatAlignClassSlots m_slots_TextFormatAlignClass;

	public:
		inline Stringp getSlotCENTER(){return m_slots_TextFormatAlignClass.CENTER;}
		inline Stringp getSlotJUSTIFY(){return m_slots_TextFormatAlignClass.JUSTIFY;}
		inline Stringp getSlotLEFT(){return m_slots_TextFormatAlignClass.LEFT;}
		inline Stringp getSlotRIGHT(){return m_slots_TextFormatAlignClass.RIGHT;}
		inline Stringp getSlotEND(){return m_slots_TextFormatAlignClass.END;}
		inline Stringp getSlotSTART(){return m_slots_TextFormatAlignClass.START;}
};
class TextFormatAlignObject : public ScriptObject
{
	public:
		TextFormatAlignObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextFormatAlignObjectSlots m_slots_TextFormatAlignObject;
};}
#endif