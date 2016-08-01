#ifndef _AS3ProgressEvent_
#define _AS3ProgressEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class ProgressEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp PROGRESS = "progress";
	//Stringp SOCKET_DATA = "socketData"; 

	//STANDARD_ERROR_DATA : String = "standardErrorData"
	//STANDARD_INPUT_PROGRESS : String = "standardInputProgress"
	//STANDARD_OUTPUT_DATA : String = "standardOutputData"

	Stringp PROGRESS;
	Stringp SOCKET_DATA; 

	//new adds
	Stringp STANDARD_ERROR_DATA;
	Stringp STANDARD_INPUT_PROGRESS;
	Stringp STANDARD_OUTPUT_DATA;
	//new adds end
private:};
class ProgressEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ProgressEventObject;

	class ProgressEventClass : public ClassClosure//EventClass
	{
	public:
		ProgressEventObject* CreateEventObject(int id, uint32_t bytesLoaded, uint32_t bytesTotal);
		Stringp		 IDToStringType(int id);
	public:
		ProgressEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ProgressEventClassSlots m_slots_ProgressEventClass;
	public:
			inline Stringp getSlotPROGRESS(){return m_slots_ProgressEventClass.PROGRESS;}
			inline Stringp getSlotSOCKET_DATA(){return m_slots_ProgressEventClass.SOCKET_DATA;}

			//new adds
			inline Stringp getSlotSTANDARD_ERROR_DATA(){return m_slots_ProgressEventClass.STANDARD_ERROR_DATA;}
			inline Stringp getSlotSTANDARD_INPUT_PROGRESS(){return m_slots_ProgressEventClass.STANDARD_INPUT_PROGRESS;}
			inline Stringp getSlotSTANDARD_OUTPUT_DATA(){return m_slots_ProgressEventClass.STANDARD_OUTPUT_DATA;}
			//new adds end
};
class ProgressEventObject : public EventObject
{
	public:
		uint32_t m_bytesLoaded;
		uint32_t m_bytesToal;
	public:
		ProgressEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		virtual EventObject* Clone(EventObject*pObj);

		uint32_t AS3_bytesLoaded_get();
		void AS3_bytesLoaded_set(uint32_t bytesLoaded);
		uint32_t AS3_bytesTotal_get();
		void AS3_bytesTotal_set(uint32_t bytesTotal);
		void AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, uint32_t bytesLoaded, uint32_t bytesTotal);
		ProgressEventObject* AS3_clone();
		Stringp AS3_toString();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ProgressEventObjectSlots m_slots_ProgressEventObject;
};}
#endif