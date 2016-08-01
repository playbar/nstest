#ifndef _AS3VideoEvent_
#define _AS3VideoEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class VideoEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//RENDER_STATE : Stringp = "renderState"
//RENDER_STATUS_ACCELERATED : Stringp = "accelerated"
//RENDER_STATUS_SOFTWARE : Stringp = "software"
//RENDER_STATUS_UNAVAILABLE : Stringp = "unavailable"

	Stringp RENDER_STATE;
	Stringp RENDER_STATUS_ACCELERATED;
	Stringp RENDER_STATUS_SOFTWARE;
	Stringp RENDER_STATUS_UNAVAILABLE;

private:};
class VideoEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	Stringp m_status;
	Stringp codecInfo;
private:};
}}
namespace avmshell{
	class VideoEventClass : public ClassClosure
	{
	public:
		VideoEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::VideoEventClassSlots m_slots_VideoEventClass;
};
class VideoEventObject : public EventObject
{
	public:
		VideoEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//Stringp AS3_status_get();

		//void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, Stringp status);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::VideoEventObjectSlots m_slots_VideoEventObject;
};}
#endif