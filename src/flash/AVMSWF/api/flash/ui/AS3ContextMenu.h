#ifndef _AS3ContextMenu_
#define _AS3ContextMenu_

namespace avmplus
{
	class ArrayObject;
};

#include "AS3EventDispatcher.h"
#include "AS3ContextMenuClipboardItems.h"
#include "AS3NativeMenuItem.h"
#include "AS3Stage.h"

namespace avmplus{namespace NativeID{
class ContextMenuClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ContextMenuObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ContextMenuBuiltInItemsObject;
	class ContextMenuClass : public ClassClosure
	{
	public:
		
		ContextMenuClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ContextMenuClassSlots m_slots_ContextMenuClass;
};
class ContextMenuObject : public NativeMenuObject
{
	public:
		ContextMenuObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		//virtual int MapSystemEvent(Stringp strName,int&nMax);
	public:

		DRCWB(ContextMenuBuiltInItemsObject*) m_pBuiltInItems;
		DRCWB(ArrayObject*)				m_pCustoms;

		//new adds
			DRCWB(ContextMenuClipboardItemsObject*)	m_pClipboardItems;
			bool	m_clipboardMenu;
			DRCWB(ArrayObject*)	m_pItems;
			DRCWB(URLRequestObject*)	m_pLink;
		//new adds end

		ContextMenuBuiltInItemsObject* AS3_builtInItems_get();
		void AS3_builtInItems_set(ContextMenuBuiltInItemsObject*);
		ArrayObject* AS3_customItems_get();
		void AS3_customItems_set(ArrayObject*);
		void AS3_constructor();
		ContextMenuObject* AS3_clone();
		void AS3_hideBuiltInItems();

		//new adds
		ContextMenuClipboardItemsObject* AS3_clipboardItems_get();
		void AS3_clipboardItems_set(ContextMenuClipboardItemsObject *pClipboardItems);
		bool AS3_clipboardMenu_get();
		void AS3_clipboardMenu_set(bool clipboardMenu);
//		bool AS3_isSupported_get();
		ArrayObject* AS3_items_get();
		void AS3_items_set(ArrayObject *items);
		URLRequestObject* AS3_link_get();
		void AS3_link_set(URLRequestObject *pLink);
		int AS3_numItems_get();
		void AS3_numItems_set(int numItems);

		NativeMenuItemObject* AS3_addItemAt(NativeMenuItemObject *pItem, int index);
		bool AS3_containsItem(NativeMenuItemObject *pItem);
		void AS3_display(StageObject *pStage, double stageX, double stageY);
		NativeMenuItemObject* AS3_getItemAt(int index);
		int AS3_getItemIndex(NativeMenuItemObject *pItem);
		void AS3_removeAllItems();
		NativeMenuItemObject* AS3_removeItemAt(int index);
		//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ContextMenuObjectSlots m_slots_ContextMenuObject;
};}
#endif