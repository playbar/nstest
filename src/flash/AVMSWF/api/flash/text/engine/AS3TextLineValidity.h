#ifndef _AS3TextLineValidity_
#define _AS3TextLineValidity_
namespace avmplus{namespace NativeID{
class TextLineValidityClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//INVALID : String = "invalid"
//POSSIBLY_INVALID : String = "possiblyInvalid"
//STATIC : String = "static"
//VALID : String = "valid"

	Stringp INVALID;
	Stringp POSSIBLY_INVALID;
	Stringp STATIC;
	Stringp VALID;

private:};
class TextLineValidityObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TextLineValidityClass : public ClassClosure
	{
	public:
		TextLineValidityClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		Stringp getINVALID(){return m_slots_TextLineValidityClass.INVALID;}
		Stringp getPOSSIBLY_INVALID(){return m_slots_TextLineValidityClass.POSSIBLY_INVALID;}
		Stringp getSTATIC(){return m_slots_TextLineValidityClass.STATIC;}
		Stringp getVALID(){return m_slots_TextLineValidityClass.VALID;}
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextLineValidityClassSlots m_slots_TextLineValidityClass;
};
class TextLineValidityObject : public ScriptObject
{
	public:
		TextLineValidityObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextLineValidityObjectSlots m_slots_TextLineValidityObject;
};}
#endif