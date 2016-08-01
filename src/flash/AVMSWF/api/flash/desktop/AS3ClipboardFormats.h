#ifndef _AS3ClipboardFormats_
#define _AS3ClipboardFormats_
namespace avmplus{namespace NativeID{
class ClipboardFormatsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//BITMAP_FORMAT : Stringp = "air:bitmap"
//FILE_LIST_FORMAT : Stringp = "air:file list"
//FILE_PROMISE_LIST_FORMAT : Stringp = "air:file promise list"
//HTML_FORMAT : Stringp = "air:html"
//RICH_TEXT_FORMAT : Stringp = "air:rtf"
//TEXT_FORMAT : Stringp = "air:text"
//URL_FORMAT : Stringp = "air:url"

	Stringp FLASH_PREFIX;
	Stringp AIR_PREFIX;
	Stringp REFERENCE_PREFIX;
	Stringp SERIALIZATION_PREFIX;
	Stringp TEXT_FORMAT;
	Stringp HTML_FORMAT;
	Stringp RICH_TEXT_FORMAT;
	Stringp URL_FORMAT;
	Stringp FILE_LIST_FORMAT;
	Stringp BITMAP_FORMAT;
	Stringp FILE_PROMISE_LIST_FORMAT;
private:};
class ClipboardFormatsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ClipboardFormatsClass : public ClassClosure
	{
	public:
		ClipboardFormatsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ClipboardFormatsClassSlots m_slots_ClipboardFormatsClass;
};
class ClipboardFormatsObject : public ScriptObject
{
	public:
		ClipboardFormatsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ClipboardFormatsObjectSlots m_slots_ClipboardFormatsObject;
};}
#endif