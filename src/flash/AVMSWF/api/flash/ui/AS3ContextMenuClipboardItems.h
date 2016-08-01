#ifndef _AS3ContextMenuClipboardItems_
#define _AS3ContextMenuClipboardItems_
namespace avmplus{namespace NativeID{
class ContextMenuClipboardItemsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ContextMenuClipboardItemsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	XBOOL _cut;
	XBOOL _copy;
	XBOOL _paste;
	XBOOL _clear;
	XBOOL _selectAll;
private:};
}}
namespace avmshell{
	class ContextMenuClipboardItemsObject;
	class ContextMenuClipboardItemsClass : public ClassClosure
	{
	public:
		ContextMenuClipboardItemsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		ContextMenuClipboardItemsObject* CreateObject();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ContextMenuClipboardItemsClassSlots m_slots_ContextMenuClipboardItemsClass;
};
class ContextMenuClipboardItemsObject : public ScriptObject
{
	public:
		ContextMenuClipboardItemsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//bool AS3_clear_get();
		//void AS3_clear_set(bool clear);
		//bool AS3_copy_get();
		//void AS3_copy_set(bool copy);
		//bool AS3_cut_get();
		//void AS3_cut_set(bool cut);
		//bool AS3_paste_get();
		//void AS3_paste_set(bool paste);
		//bool AS3_selectAll_get();
		//void AS3_selectAll_set(bool selectAll);

		//void AS3_constructor();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ContextMenuClipboardItemsObjectSlots m_slots_ContextMenuClipboardItemsObject;
};}
#endif