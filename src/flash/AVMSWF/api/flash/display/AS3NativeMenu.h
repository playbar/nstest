#ifndef _AS3NativeMenu_
#define _AS3NativeMenu_

#include "AS3EventDispatcher.h"
#include "AS3Stage.h"
#include "AS3NativeMenuItem.h"

namespace avmplus{namespace NativeID{
class NativeMenuClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class NativeMenuObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NativeMenuClass : public ClassClosure
	{
	public:
		NativeMenuClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeMenuClassSlots m_slots_NativeMenuClass;
};
class StageObject;
class NativeMenuItemObject;
class NativeMenuObject : public EventDispatcherObject
{
	public:
		NativeMenuObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//bool AS3_isSupported_get();
		//ArrayObject* AS3_items_get();
		//void AS3_items_set(ArrayObject *pItems);
		//int AS3_numItems_get();
		//NativeMenuObject* AS3_parent_get();

		//void AS3_constructor();
		//NativeMenuItemObject* AS3_addItem(NativeMenuItemObject *pItem);
		//NativeMenuItemObject* AS3_addItemAt(NativeMenuItemObject *pItem, int index);
		//NativeMenuItemObject* AS3_addSubmenu(NativeMenuObject *pSubmenu, Stringp pLabel);
		//NativeMenuItemObject* AS3_addSubmenuAt(NativeMenuObject *pSubmenu, int index, Stringp pLabel);
		//NativeMenuObject* AS3_clone();
		//bool AS3_containsItem(NativeMenuItemObject *pItem);
		//void AS3_display(StageObject *pStage, double stageX, double stageY);
		//NativeMenuItemObject* AS3_getItemAt(int index);
		//NativeMenuItemObject* AS3_getItemByName(Stringp pName);
		//int AS3_getItemIndex(NativeMenuItemObject *pItem);
		//void AS3_removeAllItems();
		//NativeMenuItemObject* AS3_removeItem(NativeMenuItemObject *pItem);
		//NativeMenuItemObject* AS3_removeItemAt(int index);
		//void AS3_setItemIndex(NativeMenuItemObject *pItem, int index);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeMenuObjectSlots m_slots_NativeMenuObject;
};}
#endif