#ifndef _AS3FullScreenEvent_
#define _AS3FullScreenEvent_

#include "AS3ActivityEvent.h"

namespace avmplus{namespace NativeID{
class FullScreenEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
//	Stringp FULL_SCREEN = "fullScreen";

	Stringp FULL_SCREEN;
	//new adds 11
 	Stringp       FULL_SCREEN_INTERACTIVE_ACCEPTED;
 	//new adds 11 end
private:};
class FullScreenEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class FullScreenEventObject;

	class FullScreenEventClass : public ClassClosure//ActivityEventClass
	{
	public:
		FullScreenEventObject* CreateEventObject(int id, bool fullScreen, bool interactive);
		Stringp		 IDToStringType(int id);

	public:
		FullScreenEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FullScreenEventClassSlots m_slots_FullScreenEventClass;

	public:
		//dushixiong 2011-3-19
		Stringp getSlotFULL_SCREEN(){return m_slots_FullScreenEventClass.FULL_SCREEN;}
		Stringp getSlotFULL_SCREEN_INTERACTIVE_ACCEPTED(){return m_slots_FullScreenEventClass.FULL_SCREEN_INTERACTIVE_ACCEPTED;}
};
class FullScreenEventObject : public ActivityEventObject
{
	public:
		bool m_isFullScreen;		//dushixiong 2011-3-19
		bool m_isInteractive;
	public:
		FullScreenEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		virtual EventObject *Clone(EventObject*pObj);
	public:
		bool AS3_fullScreen_get();
		void AS3_constructor(Stringp strType, bool bBubbles, bool bCancelable, bool isFullScreen, bool isInteractive);
		FullScreenEventObject* AS3_clone();
		Stringp AS3_toString();
		//new adds 11
 		bool AS3_interactive_get();
 		//new adds 11 end
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FullScreenEventObjectSlots m_slots_FullScreenEventObject;
};}
#endif