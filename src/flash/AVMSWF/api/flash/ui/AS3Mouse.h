#ifndef _AS3Mouse_
#define _AS3Mouse_

#include "AS3MouseCursorData.h"

namespace avmplus{namespace NativeID{
class MouseClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class MouseObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class MouseClass : public ClassClosure
	{
	public:
		MouseClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		DRCWB(Stringp) m_cursor;
		void AS3_hide();
		void AS3_show();
		
		//new adds
		Stringp AS3_cursor_get();
		void AS3_cursor_set(Stringp cursor);
		bool AS3_supportsCursor_get();
		bool AS3_supportsNativeCursor_get();

		void AS3_registerCursor(Stringp pName, MouseCursorDataObject *pCursor);
		void AS3_unregisterCursor(Stringp pName);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MouseClassSlots m_slots_MouseClass;
};
class MouseObject : public ScriptObject
{
	public:
		MouseObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		
		//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MouseObjectSlots m_slots_MouseObject;
};}
#endif