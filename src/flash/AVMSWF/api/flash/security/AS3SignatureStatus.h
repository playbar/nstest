#ifndef _AS3SignatureStatus_
#define _AS3SignatureStatus_
namespace avmplus{namespace NativeID{
class SignatureStatusClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//INVALID : String = "invalid"
//UNKNOWN : String = "unknown"
//VALID : String = "valid"

	Stringp INVALID;
	Stringp UNKNOWN;
	Stringp VALID;

private:};
class SignatureStatusObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SignatureStatusClass : public ClassClosure
	{
	public:
		SignatureStatusClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SignatureStatusClassSlots m_slots_SignatureStatusClass;
};
class SignatureStatusObject : public ScriptObject
{
	public:
		SignatureStatusObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SignatureStatusObjectSlots m_slots_SignatureStatusObject;
};}
#endif