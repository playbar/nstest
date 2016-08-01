#ifndef _AS3TextLineCreationResult_
#define _AS3TextLineCreationResult_
namespace avmplus{namespace NativeID{
class TextLineCreationResultClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//COMPLETE : String = "complete"
//EMERGENCY : String = "emergency"
//INSUFFICIENT_WIDTH : String = "insufficientWidth"
//SUCCESS : String = "success"

	Stringp COMPLETE;
	Stringp EMERGENCY;
	Stringp INSUFFICIENT_WIDTH;
	Stringp SUCCESS;

private:};
class TextLineCreationResultObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TextLineCreationResultClass : public ClassClosure
	{
	public:
		TextLineCreationResultClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextLineCreationResultClassSlots m_slots_TextLineCreationResultClass;
};
class TextLineCreationResultObject : public ScriptObject
{
	public:
		TextLineCreationResultObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextLineCreationResultObjectSlots m_slots_TextLineCreationResultObject;
};}
#endif