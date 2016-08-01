#include "StdAfxflash.h"
//#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;

#include "../avm2.h"
//#include "../AS3/TotalInclude.h"

#include "AS3AsyncErrorEvent.h"
namespace avmshell{
AsyncErrorEventClass::AsyncErrorEventClass(VTable* cvtable):ClassClosure(cvtable)//ErrorEventClass(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())AsyncErrorEventObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* AsyncErrorEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) AsyncErrorEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

AsyncErrorEventObject* AsyncErrorEventClass::CreateEventObject(int id, Stringp pText, ErrorObject *pError)
{
	Stringp strType = IDToStringType(id);
	if (!strType)
	{
		return NULL;
	}

//	Atom atoms[] = {strType->atom(), false, false, pText->atom(), pError->atom()};

	AsyncErrorEventObject *pObj = (AsyncErrorEventObject *)createInstance(ivtable(), prototype);
	pObj->AS3_constructor(strType, false, false, pText, pError);
	pObj->nEventID=id;
	return pObj;
}

Stringp AsyncErrorEventClass::IDToStringType(int id)
{
	switch (id)
	{
	case XAS3_EVENT_asyncError:
		return this->getSlotASYNC_ERROR();
	default:
		{
			ErrorEventClass*pClass=((ShellToplevel*)toplevel())->getErrorEventClass();
			return pClass->IDToStringType(id);
		}
		//return ErrorEventClass::IDToStringType(id);
	}
}

AsyncErrorEventObject::AsyncErrorEventObject(VTable *vtable, ScriptObject* proto, int capacity): ErrorEventObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_pError = NULL;
}
//////////////////////////////////////////////////////////
//Native Method start...
ErrorObject* AsyncErrorEventObject::AS3_error_get()
{
	//Add your act code here...
	return m_pError;
}

void AsyncErrorEventObject::AS3_error_set(ErrorObject *pError)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};

	m_pError = pError;
}

//AS3 contructor function..
void AsyncErrorEventObject::AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, Stringp text, ErrorObject *pError)
{
	//Add your act code here...
	ErrorEventObject::AS3_constructor(type, bBubbles, bCancelable, text);

	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject
	//	, argoff2 = argoff1 + AvmThunkArgSize_AvmString
	//	, argoff3 = argoff2 + AvmThunkArgSize_AvmBool32
	//	, argoff4 = argoff3 + AvmThunkArgSize_AvmBool32
	//	, argoff5 = argoff4 + AvmThunkArgSize_AvmString
	//};

	m_pError = pError;
}

EventObject* AsyncErrorEventObject::Clone(EventObject *pObj)
{
	if (NULL == pObj)
	{
		ClassClosure *pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_AsyncErrorEvent);
		pObj = (EventObject *)pClass->createInstance(pClass->ivtable(), pClass->prototype);
	}

	register AsyncErrorEventObject *pEvent = (AsyncErrorEventObject *)pObj;

	pEvent->m_pError = m_pError;

	return ErrorEventObject::Clone(pObj);
}

AsyncErrorEventObject* AsyncErrorEventObject::AS3_clone()
{
	//Add your act code here...
	return (AsyncErrorEventObject *)Clone(NULL);
}

Stringp AsyncErrorEventObject::AS3_toString()
{
	//Add your act code here...
	return EventObject::AS3_toString();
}

}