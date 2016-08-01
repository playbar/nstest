#ifndef _AS3FileMode_
#define _AS3FileMode_
namespace avmplus{namespace NativeID{
class FileModeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//APPEND : String = "append"
//READ : String = "read"
//UPDATE : String = "update"
//WRITE : String = "write"

	Stringp APPEND;
	Stringp READ;
	Stringp UPDATE;
	Stringp WRITE;

private:};
class FileModeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class FileModeClass : public ClassClosure
	{
	public:
		FileModeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FileModeClassSlots m_slots_FileModeClass;
};
class FileModeObject : public ScriptObject
{
	public:
		FileModeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FileModeObjectSlots m_slots_FileModeObject;
};}
#endif