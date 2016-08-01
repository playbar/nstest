#include "StdAfxflash.h"
//#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;

#include "../avm2.h"
//#include "../AS3/TotalInclude.h"

#include "AS3HTTPStatusEvent.h"
namespace avmshell{
HTTPStatusEventClass::HTTPStatusEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())HTTPStatusEventObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* HTTPStatusEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) HTTPStatusEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

HTTPStatusEventObject* HTTPStatusEventClass::CreateEventObject(int id, int32_t status)
{
	Stringp strType = IDToStringType(id);
	if (!strType)
	{
		return NULL;
	}

//	Atom atoms[] = {strType->atom(), false, false, status};

	HTTPStatusEventObject *pObj = (HTTPStatusEventObject *)createInstance(ivtable(), prototype);
	pObj->AS3_constructor(strType, false, false, status);
	pObj->nEventID=id;
	return pObj;
}

Stringp HTTPStatusEventClass::IDToStringType(int id)
{
	switch (id)
	{
	case XAS3_EVENT_httpStatus:
		return this->getSlotHTTP_STATUS();
	case XAS3_EVENT_httpResponseStatus:
		return getSlotHTTP_RESPONSE_STATUS();
	default:
		{
			EventClass*pClass=((ShellToplevel*)toplevel())->getEventClass();
			return pClass->IDToStringType(id);
		}
		//return EventClass::IDToStringType(id);
	}
}

HTTPStatusEventObject::HTTPStatusEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_status = 0;
	m_bRedirected = false;
}
//////////////////////////////////////////////////////////
//Native Method start...
int32_t HTTPStatusEventObject::AS3_status_get()
{
	//Add your act code here...
	return m_status;
}

//AS3 contructor function..
void HTTPStatusEventObject::AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, int32_t status/*, bool redirected*/)
{
	//Add your act code here...
//	EventObject::AS3_constructor(env, argc, argv);

	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject
	//	, argoff2 = argoff1 + AvmThunkArgSize_AvmString
	//	, argoff3 = argoff2 + AvmThunkArgSize_AvmBool32
	//	, argoff4 = argoff3 + AvmThunkArgSize_AvmBool32
	//};

	//Stringp strType = AvmThunkUnbox_AvmString(argv[argoff1]);
	//bool bBubbles = AvmThunkUnbox_AvmBool32(argv[argoff2]);
	//bool bCancelable = AvmThunkUnbox_AvmBool32(argv[argoff3]);

	//EventObject::AS3_constructor(strType, bBubbles, bCancelable);

	//m_status = AvmThunkUnbox_int32_t(argv[argoff4]);

	EventObject::AS3_constructor(type, bBubbles, bCancelable);
	m_status = status;
	//m_bRedirected = redirected;
}

EventObject* HTTPStatusEventObject::Clone(EventObject*pObj)
{
	if (NULL == pObj)
	{
		ClassClosure *pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_HTTPStatusEvent);
		pObj = (EventObject *)pClass->createInstance(pClass->ivtable(), pClass->prototype);
	}

	register HTTPStatusEventObject * pEvent = (HTTPStatusEventObject *)pObj;
	pEvent->m_status = m_status;
	pEvent->m_bRedirected = m_bRedirected;

	return EventObject::Clone(pObj);
}

HTTPStatusEventObject* HTTPStatusEventObject::AS3_clone()
{
	//Add your act code here...
	return (HTTPStatusEventObject *)Clone(NULL);
}

Stringp HTTPStatusEventObject::AS3_toString()
{
	//Add your act code here...
	return EventObject::AS3_toString();
}
//new adds
ArrayObject* HTTPStatusEventObject::AS3_responseHeaders_get()
{
	return m_pResponseHeaders;
}
void HTTPStatusEventObject::AS3_responseHeaders_set(ArrayObject *pResponseHeaders)
{
	m_pResponseHeaders = pResponseHeaders;
}
Stringp HTTPStatusEventObject::AS3_responseURL_get()
{
	return m_responseURL;
}
void HTTPStatusEventObject::AS3_responseURL_set(Stringp pResponseURL)
{
	m_responseURL = pResponseURL;
}
//new adds end

//new adds 11
bool HTTPStatusEventObject::AS3_redirected_get()
{
//	LOGWHERE();
	return m_bRedirected;
}
void HTTPStatusEventObject::AS3_redirected_set(bool bRed)
{
//	LOGWHERE();
	m_bRedirected = bRed;
}
//new adds 11 end

}