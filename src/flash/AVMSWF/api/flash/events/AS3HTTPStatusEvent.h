#ifndef _AS3HTTPStatusEvent_
#define _AS3HTTPStatusEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class HTTPStatusEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp HTTP_STATUS = "httpStatus";

	//HTTP_RESPONSE_STATUS : String = "httpResponseStatus"

	Stringp HTTP_STATUS;

	//new adds
	Stringp HTTP_RESPONSE_STATUS;
	//new adds end
private:};
class HTTPStatusEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class HTTPStatusEventObject;

	class HTTPStatusEventClass : public ClassClosure//EventClass
	{
	public:
		HTTPStatusEventObject* CreateEventObject(int id, int32_t status);
		Stringp		 IDToStringType(int id);
	public:
		HTTPStatusEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::HTTPStatusEventClassSlots m_slots_HTTPStatusEventClass;
	public:
		inline Stringp getSlotHTTP_STATUS(){return m_slots_HTTPStatusEventClass.HTTP_STATUS;}

		//new adds
		inline Stringp getSlotHTTP_RESPONSE_STATUS(){return m_slots_HTTPStatusEventClass.HTTP_RESPONSE_STATUS;}
		//new adds end
};
class HTTPStatusEventObject : public EventObject
{
	public:
		int32_t m_status;

		//new adds
		DRCWB(ArrayObject*) m_pResponseHeaders;
		DRCWB(Stringp)		m_responseURL;
		//new adds end

	public:
		HTTPStatusEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		virtual EventObject* Clone(EventObject *pObj);

		int32_t AS3_status_get();
		void AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, int32_t status/*, bool redirected = false*/);
		HTTPStatusEventObject* AS3_clone();
		Stringp AS3_toString();

		//new adds
		ArrayObject* AS3_responseHeaders_get();
		void AS3_responseHeaders_set(ArrayObject *pResponseHeaders);
		Stringp AS3_responseURL_get();
		void AS3_responseURL_set(Stringp pResponseURL);
		//new adds end

		//new adds 11
		bool m_bRedirected;
		bool AS3_redirected_get();
		void AS3_redirected_set(bool bRed);
		//new adds 11 end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::HTTPStatusEventObjectSlots m_slots_HTTPStatusEventObject;
};}
#endif