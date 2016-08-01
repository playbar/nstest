#ifndef _AS3ContextMenuEvent_
#define _AS3ContextMenuEvent_

#include "../../MMgc/WriteBarrier.h"
#include "AS3InteractiveObject.h"

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class ContextMenuEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp MENU_ITEM_SELECT = "menuItemSelect";
	//Stringp MENU_SELECT = "menuSelect";

	Stringp MENU_ITEM_SELECT;
	Stringp MENU_SELECT;
private:};
class ContextMenuEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ContextMenuEventObject;

	class ContextMenuEventClass : public ClassClosure//EventClass
	{
	public:
		ContextMenuEventObject* CreateEventObject(int id, InteractiveObjectObject *pMouseTarget, InteractiveObjectObject *pContextMenuOwner);
		Stringp		 IDToStringType(int id);
	public:
		ContextMenuEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ContextMenuEventClassSlots m_slots_ContextMenuEventClass;
	public:
			Stringp getSlotMENU_ITEM_SELECT(){return m_slots_ContextMenuEventClass.MENU_ITEM_SELECT;}
			Stringp getSlotMENU_SELECT(){return m_slots_ContextMenuEventClass.MENU_SELECT;}
};
class ContextMenuEventObject : public EventObject
{
	public:
		//DRCWB(InteractiveObjectObject *)m_pContextMenuOwner;
		//DRCWB(InteractiveObjectObject *)m_pMouseTarget;
		InteractiveObjectObject *m_pContextMenuOwner;
		InteractiveObjectObject *m_pMouseTarget;
		bool	m_isMouseTargetInaccessible;
	public:
		ContextMenuEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		virtual EventObject* Clone(EventObject *pObj);

		InteractiveObjectObject* AS3_contextMenuOwner_get();
		void AS3_contextMenuOwner_set(InteractiveObjectObject *pContextMenuOwner);
		InteractiveObjectObject* AS3_mouseTarget_get();
		void AS3_mouseTarget_set(InteractiveObjectObject *pMouseTarget);
		void AS3_constructor(Stringp strType, bool bBubbles, bool bCancelable, InteractiveObjectObject *pContextMenuOwner, InteractiveObjectObject *pMouseTarget);
		ContextMenuEventObject* AS3_clone();
		Stringp AS3_toString();

		//new adds
		bool AS3_isMouseTargetInaccessible_get();
		void AS3_isMouseTargetInaccessible_set(bool isMouseTargetInaccessible);
		//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ContextMenuEventObjectSlots m_slots_ContextMenuEventObject;
};}
#endif