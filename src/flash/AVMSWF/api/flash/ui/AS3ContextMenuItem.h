#ifndef _AS3ContextMenuItem_
#define _AS3ContextMenuItem_

#include "AS3EventDispatcher.h"

namespace avmplus{namespace NativeID{
class ContextMenuItemClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ContextMenuItemObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
	Stringp caption;
	int		enabled;
	int		separatorBefore;
	int		visible;
//Declare your MEMBER AS3 slots here!!!
private:};
}}

#define AS3MENUITEMDATA avmplus::NativeID::ContextMenuItemObjectSlots

namespace avmshell{
	class ContextMenuItemObject;

	class ContextMenuItemClass : public ClassClosure
	{
	public:
		ContextMenuItemClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		//new adds
		ContextMenuItemObject* AS3_systemClearMenuItem();
		ContextMenuItemObject* AS3_systemCopyLinkMenuItem();
		ContextMenuItemObject* AS3_systemCopyMenuItem();
		ContextMenuItemObject* AS3_systemCutMenuItem();
		ContextMenuItemObject* AS3_systemOpenLinkMenuItem();
		ContextMenuItemObject* AS3_systemPasteMenuItem();
		ContextMenuItemObject* AS3_systemSelectAllMenuItem();
		//new adds end
		
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ContextMenuItemClassSlots m_slots_ContextMenuItemClass;
};
class ContextMenuItemObject : public EventDispatcherObject
{
	public:
		ContextMenuItemObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		//virtual int MapSystemEvent(Stringp strName,int&nMax);
	public:
		AS3MENUITEMDATA& GetData(){return m_slots_ContextMenuItemObject;}

		bool AS3_visible_get();
		void AS3_visible_set( bool b );
	private:
		bool mbVisible;
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ContextMenuItemObjectSlots m_slots_ContextMenuItemObject;
};}
#endif