#ifndef _AS3IBitmapCompressOptions_
#define _AS3IBitmapCompressOptions_
namespace avmplus{namespace NativeID{
class IBitmapCompressOptionsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class IBitmapCompressOptionsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class IBitmapCompressOptionsClass : public ClassClosure
	{
	public:
		IBitmapCompressOptionsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IBitmapCompressOptionsClassSlots m_slots_IBitmapCompressOptionsClass;
};
class IBitmapCompressOptionsObject : public ScriptObject
{
	public:
		IBitmapCompressOptionsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IBitmapCompressOptionsObjectSlots m_slots_IBitmapCompressOptionsObject;
};}
#endif