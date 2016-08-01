#ifndef _AS3MediaType_
#define _AS3MediaType_
namespace avmplus{namespace NativeID{
class MediaTypeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//IMAGE : String = "image"
	//VIDEO : String = "video"

	Stringp IMAGE;
	Stringp VIDEO;

private:};
class MediaTypeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class MediaTypeClass : public ClassClosure
	{
	public:
		MediaTypeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MediaTypeClassSlots m_slots_MediaTypeClass;
};
class MediaTypeObject : public ScriptObject
{
	public:
		MediaTypeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MediaTypeObjectSlots m_slots_MediaTypeObject;
};}
#endif