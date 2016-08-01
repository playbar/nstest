#ifndef _AS3IOErrorEvent_
#define _AS3IOErrorEvent_

#include "AS3ErrorEvent.h"

namespace avmplus{namespace NativeID{
class IOErrorEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
//	Stringp IO_ERROR = "ioError";

//STANDARD_ERROR_IO_ERROR : String = "standardErrorIoError"
//STANDARD_INPUT_IO_ERROR : String = "standardInputIoError"
//STANDARD_OUTPUT_IO_ERROR : String = "standardOutputIoError"

	Stringp IO_ERROR;

	//new adds
	Stringp STANDARD_ERROR_IO_ERROR;
	Stringp STANDARD_INPUT_IO_ERROR;
	Stringp STANDARD_OUTPUT_IO_ERROR;
	//new adds end
private:};
class IOErrorEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class IOErrorEventObject;

	class IOErrorEventClass : public ClassClosure//ErrorEventClass
	{
	public:
		IOErrorEventObject* CreateEventObject(int id, Stringp pText);
		Stringp		 IDToStringType(int id);
	public:
		IOErrorEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IOErrorEventClassSlots m_slots_IOErrorEventClass;
	public:
		inline Stringp getSlotIO_ERROR(){return m_slots_IOErrorEventClass.IO_ERROR;}

		//new adds
		inline Stringp getSlotSTANDARD_ERROR_IO_ERROR(){return m_slots_IOErrorEventClass.STANDARD_ERROR_IO_ERROR;}
		inline Stringp getSlotSTANDARD_INPUT_IO_ERROR(){return m_slots_IOErrorEventClass.STANDARD_INPUT_IO_ERROR;}
		inline Stringp getSlotSTANDARD_OUTPUT_IO_ERROR(){return m_slots_IOErrorEventClass.STANDARD_OUTPUT_IO_ERROR;}
		//new adds end
};
class IOErrorEventObject : public ErrorEventObject
{
	public:
		IOErrorEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		virtual EventObject* Clone(EventObject *pObj);

		void AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, Stringp pText);
		IOErrorEventObject* AS3_clone();
		Stringp AS3_toString();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IOErrorEventObjectSlots m_slots_IOErrorEventObject;
};}
#endif