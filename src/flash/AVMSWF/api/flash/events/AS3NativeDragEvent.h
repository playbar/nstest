#ifndef _AS3NativeDragEvent_
#define _AS3NativeDragEvent_

#include "AS3Event.h"
#include "AS3MouseEvent.h"
#include "AS3NativeDragOptions.h"
#include "AS3Clipboard.h"
#include "AS3InteractiveObject.h"

namespace avmplus{namespace NativeID{
class NativeDragEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//NATIVE_DRAG_COMPLETE : String = "nativeDragComplete"
//NATIVE_DRAG_DROP : String = "nativeDragDrop"
//NATIVE_DRAG_ENTER : String = "nativeDragEnter"
//NATIVE_DRAG_EXIT : String = "nativeDragExit"
//NATIVE_DRAG_OVER : String = "nativeDragOver"
//NATIVE_DRAG_START : String = "nativeDragStart"
//NATIVE_DRAG_UPDATE : String = "nativeDragUpdate"

	Stringp NATIVE_DRAG_COMPLETE;
	Stringp NATIVE_DRAG_DROP;
	Stringp NATIVE_DRAG_ENTER;
	Stringp NATIVE_DRAG_EXIT;
	Stringp NATIVE_DRAG_OVER;
	Stringp NATIVE_DRAG_START;
	Stringp NATIVE_DRAG_UPDATE;

private:};
class NativeDragEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NativeDragEventClass : public ClassClosure//MouseEventClass
	{
	public:
		NativeDragEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeDragEventClassSlots m_slots_NativeDragEventClass;
};
class NativeDragEventObject : public MouseEventObject
{
	public:
		NativeDragEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		NativeDragOptionsObject* AS3_allowedActions_get();
		void AS3_allowedActions_set(NativeDragOptionsObject *pAllowedActions);
		ClipboardObject* AS3_clipboard_get();
		void AS3_clipboard_set(ClipboardObject *pClipboard);
		Stringp AS3_dropAction_get();
		void AS3_dropAction_set(Stringp pDropAction);

		void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
								double localX, double localY, InteractiveObjectObject *pRelatedObject, 
								ClipboardObject *pClipboard, NativeDragOptionsObject *pAllowedActions, Stringp pDropAction, 
								bool controlKey, bool altKey, bool shiftKey, bool commandKey);
		EventObject* AS3_clone();
		Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeDragEventObjectSlots m_slots_NativeDragEventObject;
};}
#endif