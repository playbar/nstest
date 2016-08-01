#ifndef _AS3NetStreamAppendBytesAction_
#define _AS3NetStreamAppendBytesAction_
namespace avmplus{namespace NativeID{
class NetStreamAppendBytesActionClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//END_SEQUENCE : String = "endSequence"
//RESET_BEGIN : String = "resetBegin"
//RESET_SEEK : String = "resetSeek"

	Stringp END_SEQUENCE;
	Stringp RESET_BEGIN;
	Stringp RESET_SEEK;

private:};
class NetStreamAppendBytesActionObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NetStreamAppendBytesActionClass : public ClassClosure
	{
	public:
		NetStreamAppendBytesActionClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetStreamAppendBytesActionClassSlots m_slots_NetStreamAppendBytesActionClass;
};
class NetStreamAppendBytesActionObject : public ScriptObject
{
	public:
		NetStreamAppendBytesActionObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetStreamAppendBytesActionObjectSlots m_slots_NetStreamAppendBytesActionObject;
};}
#endif