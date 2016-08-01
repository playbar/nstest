#ifndef _AS3InteractiveObject_
#define _AS3InteractiveObject_

#include "AS3DisplayObject.h"
#include "AS3AccessibilityImplementation.h"
#include "AS3Rectangle.h"

namespace avmplus{namespace NativeID{
class InteractiveObjectClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class InteractiveObjectObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ContextMenuObject;
	class InteractiveObjectClass : public ClassClosure//DisplayObjectClass
	{
	public:
		InteractiveObjectClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::InteractiveObjectClassSlots m_slots_InteractiveObjectClass;
};
class InteractiveObjectObject : public  DisplayObjectObject
{
	public:
		InteractiveObjectObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		DRCWB(ContextMenuObject*) m_pContextMenu;
		XBOOL					  m_bDoubleClickEnabled;
		XBOOL					  m_bMouseEnabled;
		//virtual int MapSystemEvent(Stringp strName,int&nMax);
	public:
		ContextMenuObject* AS3_contextMenu_get();
		void AS3_contextMenu_set(ContextMenuObject*);
		XBOOL AS3_doubleClickEnabled_get();
		void AS3_doubleClickEnabled_set(bool);
		XBOOL AS3_focusRect_get();
		void AS3_focusRect_set(bool);
		XBOOL AS3_mouseEnabled_get();
		void AS3_mouseEnabled_set(bool);
		XBOOL AS3_tabEnabled_get();
		void AS3_tabEnabled_set(bool);
		int AS3_tabIndex_get();
		void AS3_tabIndex_set(int i);
		void AS3_constructor();

		//new adds
		AccessibilityImplementationObject* AS3_accessibilityImplementation_get();
		void AS3_accessibilityImplementation_set(AccessibilityImplementationObject *pAccessibilityImplementation);
		bool AS3_needsSoftKeyboard_get();
		void AS3_needsSoftKeyboard_set(bool bNeedsSoftKeyboard);
		RectangleObject* AS3_softKeyboardInputAreaOfInterest_get();
		void AS3_softKeyboardInputAreaOfInterest_set(RectangleObject *pSoftKeyboardInputAreaOfInterest);

		bool AS3_requestSoftKeyboard();
		//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::InteractiveObjectObjectSlots m_slots_InteractiveObjectObject;
};}
#endif