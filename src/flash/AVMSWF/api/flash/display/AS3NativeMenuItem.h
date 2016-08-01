#ifndef _AS3NativeMenuItem_
#define _AS3NativeMenuItem_

#include "AS3NativeMenu.h"
#include "AS3EventDispatcher.h"

namespace avmplus{namespace NativeID{
class NativeMenuItemClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class NativeMenuItemObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NativeMenuItemClass : public ClassClosure
	{
	public:
		NativeMenuItemClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeMenuItemClassSlots m_slots_NativeMenuItemClass;
};
class NativeMenuObject;
class NativeMenuItemObject : public EventDispatcherObject
{
	public:
		NativeMenuItemObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		bool AS3_checked_get();
		void AS3_checked_set(bool checked);
		Atom AS3_data_get();
		void AS3_data_set(Atom pData);
		bool AS3_enabled_get();
		void AS3_enabled_set(bool enabled);
		bool AS3_isSeparator_get();
		Stringp AS3_keyEquivalent_get();
		void AS3_keyEquivalent_set(Stringp pKeyEquivalent);
		ArrayObject* AS3_keyEquivalentModifiers_get();
		void AS3_keyEquivalentModifiers_set(ArrayObject *pKeyEquivalentModifiers);
		Stringp AS3_label_get();
		void AS3_label_set(Stringp pLabel);
		NativeMenuObject* AS3_menu_get();
		int AS3_mnemonicIndex_get();
		void AS3_mnemonicIndex_set(int mnemonicIndex);
		Stringp AS3_name_get();
		void AS3_name_set(Stringp pName);
		NativeMenuObject* AS3_submenu_get();
		void AS3_submenu_set(NativeMenuObject *pSubmenu);

//		void AS3_constructor(Stringp pLabel, bool isSeparator);

		NativeMenuItemObject* AS3_clone();
		Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeMenuItemObjectSlots m_slots_NativeMenuItemObject;
};}
#endif