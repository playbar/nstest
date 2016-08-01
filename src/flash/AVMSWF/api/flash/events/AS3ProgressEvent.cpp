#include "StdAfxflash.h"
//#include "avmplus.h"
#include "ShellCore.h"

using namespace avmplus;

#include "../avm2.h"
//#include "../AS3/TotalInclude.h"

#include "AS3ProgressEvent.h"
namespace avmshell{
ProgressEventClass::ProgressEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())ProgressEventObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* ProgressEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ProgressEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

ProgressEventObject* ProgressEventClass::CreateEventObject(int id, uint32_t bytesLoaded, uint32_t bytesTotal)
{
	Stringp strType = IDToStringType(id);
	if (!strType)
	{
		return NULL;
	}

//	Atom atoms[] = {strType->atom(), false, false, bytesLoaded, bytesTotal};

	ProgressEventObject *pObj = (ProgressEventObject *)createInstance(ivtable(), prototype);
	pObj->AS3_constructor(strType, false, false, bytesLoaded, bytesTotal);
	pObj->nEventID=id;
	return pObj;
}

Stringp ProgressEventClass::IDToStringType(int id)
{
	switch (id)
	{
	case XAS3_EVENT_progress:
		return this->getSlotPROGRESS();
	case XAS3_EVENT_socketData:
		return this->getSlotSOCKET_DATA();
	default:
		{
			EventClass*pClass=((ShellToplevel*)toplevel())->getEventClass();
			return pClass->IDToStringType(id);
		}
		//return EventClass::IDToStringType(id);
	}
}

ProgressEventObject::ProgressEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_bytesLoaded = 0;
	m_bytesToal = 0;
}
//////////////////////////////////////////////////////////
//Native Method start...
uint32_t ProgressEventObject::AS3_bytesLoaded_get()
{
	//Add your act code here...
	return m_bytesLoaded;
}

void ProgressEventObject::AS3_bytesLoaded_set(uint32_t bytesLoaded)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};

	m_bytesLoaded = bytesLoaded;
}

uint32_t ProgressEventObject::AS3_bytesTotal_get()
{
	//Add your act code here...
	return m_bytesToal;
}

void ProgressEventObject::AS3_bytesTotal_set(uint32_t bytesTotal)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};

	m_bytesToal = bytesTotal;
}

//AS3 contructor function..
void ProgressEventObject::AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, uint32_t bytesLoaded, uint32_t bytesTotal)
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
	//	, argoff5 = argoff4 + AvmThunkArgSize_uint32_t
	//};

	//Stringp strType = AvmThunkUnbox_AvmString(argv[argoff1]);
	//bool bBubbles = AvmThunkUnbox_AvmBool32(argv[argoff2]);
	//bool bCancelable = AvmThunkUnbox_AvmBool32(argv[argoff3]);

	EventObject::AS3_constructor(type, bBubbles, bCancelable);
	
	m_bytesToal = bytesTotal;
	m_bytesLoaded = bytesLoaded;
}

EventObject* ProgressEventObject::Clone(EventObject*pObj)
{
	if (NULL == pObj)
	{
		ClassClosure *pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_ProgressEvent);
		pObj = (EventObject *)pClass->createInstance(pClass->ivtable(), pClass->prototype);
	}

	register ProgressEventObject *pEvent = (ProgressEventObject *)pObj;

	pEvent->m_bytesLoaded = m_bytesLoaded;
	pEvent->m_bytesToal = m_bytesToal;

	return EventObject::Clone(pObj);
}

ProgressEventObject* ProgressEventObject::AS3_clone()
{
	//Add your act code here...
	return (ProgressEventObject *)Clone(NULL);
}

Stringp ProgressEventObject::AS3_toString()
{
	//Add your act code here...
	return EventObject::AS3_toString();
}

}