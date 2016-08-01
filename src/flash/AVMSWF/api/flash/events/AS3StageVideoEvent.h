#ifndef _AS3StageVideoEvent_
#define _AS3StageVideoEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class StageVideoEventClassSlots{
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
class StageVideoEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	Stringp m_status;
	Stringp m_colorSpace;
	Stringp codecInfo;
private:};
}}
namespace avmshell{
	class StageVideoEventClass : public ClassClosure//EventClass
	{
	public:
		StageVideoEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageVideoEventClassSlots m_slots_StageVideoEventClass;
};
class StageVideoEventObject : public EventObject
{
	public:
		StageVideoEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//Stringp AS3_colorSpace_get();
		//Stringp AS3_status_get();

		//void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, Stringp pStatus, Stringp pColorSpace);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageVideoEventObjectSlots m_slots_StageVideoEventObject;
};}
#endif